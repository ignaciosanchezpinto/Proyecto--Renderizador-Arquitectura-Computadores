#include "vector.hpp"
#include <algorithm>
#include <atomic>
#include <camara.hpp>
#include <cmath>
#include <configuracion.hpp>
#include <cstddef>
#include <cstdint>
#include <escenario.hpp>
#include <imagen.hpp>
#include <material.hpp>
#include <random>
#include <rayo.hpp>
#include <renderizador.hpp>
#include <string>
#include <vector>
// TBB headers específicos
#include <oneapi/tbb/blocked_range.h>               // ← tbb::blocked_range
#include <oneapi/tbb/enumerable_thread_specific.h>  // ← tbb::enumerable_thread_specific
#include <oneapi/tbb/parallel_for.h>                // ← tbb::parallel_for
#include <oneapi/tbb/partitioner.h>  // ← tbb::static_partitioner, simple_partitioner, auto_partitioner
#include <oneapi/tbb/task_arena.h>  // ← tbb::this_task_arena::max_concurrency

namespace render {

  std::string renderizador::particionador = "auto";
  std::size_t renderizador::grano         = 1;

  void renderizador::configurar(std::string const & tipo, std::size_t g) {
    particionador = tipo;
    grano         = g;
  }

  namespace {

    struct ContextoRender {
      configuracion const * conf;
      camara * cam;
      escenario const * escena;
    };

    struct ContextoRNG {
      std::mt19937_64 * mat;
      std::mt19937_64 * ray;
    };

    inline void vector_aleatorio_inplace(vector & v, double min_val, double max_val,
                                         std::mt19937_64 & rng) {
      std::uniform_real_distribution<double> dist(min_val, max_val);
      v = render::vector(dist(rng), dist(rng), dist(rng));
    }

    inline vector color_fondo(vector const & dir, configuracion const & conf) {
      static vector const & cd = conf.get_background_dark_color();
      static vector const & cl = conf.get_background_light_color();
      vector const unit_dir    = dir.normalized();
      double const t           = 0.5 * (unit_dir.get_y() + 1.0);
      return cl + (cd - cl) * t;
    }

    inline rayo calcular_refraccion(rayo const & r_in, vector const & normal, double eta_i,
                                    double eta_t) {
      vector const r              = r_in.get_direccion().normalized();
      vector n                    = normal;
      bool const frontal          = r.dot(n) < 0.0;
      double const etai_over_etat = frontal ? eta_i / eta_t : eta_t / eta_i;
      if (!frontal) {
        n = -n;
      }
      double const cos_theta = std::fmin(-r.dot(n), 1.0);
      double const sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);
      if (etai_over_etat * sin_theta > 1.0) {
        vector d = r;
        d -= n * (2.0 * r.dot(n));
        return {r_in.get_origen(), d};
      }
      double const cos_theta_p =
          std::sqrt(1.0 - (etai_over_etat * sin_theta) * (etai_over_etat * sin_theta));
      vector const d = r * etai_over_etat + n * (etai_over_etat * cos_theta - cos_theta_p);
      return {r_in.get_origen(), d};
    }

    vector calcular_color(rayo r0, escenario const & escena, configuracion const & conf,
                          std::mt19937_64 & rng) {
      rayo r = r0;
      vector color_acum(1.0, 1.0, 1.0);
      vector color_res(0.0, 0.0, 0.0);
      vector tmp;

      for (int d = 0; d < conf.get_max_depth(); ++d) {
        info_interseccion info;
        if (!escena.intersectar(r, 1e-3, info)) {
          color_res += color_acum * color_fondo(r.get_direccion(), conf);
          break;
        }
        vector & dr = tmp;
        if (info.mat->get_tipo() == TipoMaterial::Mate) {
          vector_aleatorio_inplace(dr, -1.0, 1.0, rng);
          dr += info.normal;
          if (dr.norm() < 1e-8) {
            dr = info.normal;
          }
          color_acum *= info.mat->get_color();
        } else if (info.mat->get_tipo() == TipoMaterial::Metal) {
          vector const rin = r.get_direccion().normalized();
          vector const ref = rin - info.normal * (2.0 * rin.dot(info.normal));
          vector_aleatorio_inplace(tmp, -info.mat->get_refl(), info.mat->get_refl(), rng);
          dr = ref.normalized() + tmp;
          color_acum *= info.mat->get_color();
        } else {
          dr = calcular_refraccion(r, info.normal, 1.0, info.mat->get_refrac()).get_direccion();
          color_acum *= vector(1.0, 1.0, 1.0);
        }
        r = rayo(info.punto, dr.normalized());
      }
      return color_res;
    }

    std::vector<uint64_t> generar_semillas(std::size_t n, uint64_t base) {
      std::vector<uint64_t> v(n);
      std::mt19937_64 const g(base);
      std::ranges::generate(v, g);
      return v;
    }

    Pixel procesar_pixel(int idx, int ancho, ContextoRender const & ctx, ContextoRNG & rngs) {
      int const f = idx / ancho;
      int const c = idx % ancho;

      vector acum;
      int const m = ctx.conf->get_samples_per_pixel();

      for (int i = 0; i < m; ++i) {
        rayo const ray = ctx.cam->generar_rayo(f, c, *rngs.ray);
        acum += calcular_color(ray, *ctx.escena, *ctx.conf, *rngs.mat);
      }

      double inv_m = 1.0 / m;
      double inv_g = 1.0 / ctx.conf->get_gamma();
      auto apply   = [&](double v) { return std::pow(std::clamp(v * inv_m, 0.0, 1.0), inv_g); };

      return {static_cast<uint8_t>(255.0 * apply(acum.get_x())),
              static_cast<uint8_t>(255.0 * apply(acum.get_y())),
              static_cast<uint8_t>(255.0 * apply(acum.get_z()))};
    }

    template <typename Func>
    void ejecutar_kernel(int total, std::size_t grano, std::string const & p, Func body) {
      auto rango = tbb::blocked_range<int>(0, total, grano);
      if (p == "static") {
        tbb::parallel_for(rango, body, tbb::static_partitioner());
      } else if (p == "simple") {
        tbb::parallel_for(rango, body, tbb::simple_partitioner());
      } else {
        tbb::parallel_for(rango, body, tbb::auto_partitioner());
      }
    }

  }  // namespace

  void renderizador::renderizar(configuracion const & conf, camara & cam, escenario const & esc,
                                imagen & img) {
    int const ancho = cam.getancho();
    img             = imagen(ancho, cam.getalto());

    auto nth = static_cast<std::size_t>(tbb::this_task_arena::max_concurrency());

    auto seeds_mat = generar_semillas(nth, static_cast<uint64_t>(conf.get_material_rng_seed()));
    auto seeds_ray = generar_semillas(nth, static_cast<uint64_t>(conf.get_ray_rng_seed()));

    tbb::enumerable_thread_specific<std::mt19937_64> tls_m{[&] {
      static std::atomic<size_t> c{0};
      return std::mt19937_64(seeds_mat[c++]);
    }};
    tbb::enumerable_thread_specific<std::mt19937_64> tls_r{[&] {
      static std::atomic<size_t> c{0};
      return std::mt19937_64(seeds_ray[c++]);
    }};

    ContextoRender const ctx{&conf, &cam, &esc};

    auto loop = [&](tbb::blocked_range<int> const & r) {
      ContextoRNG rngs{&tls_m.local(), &tls_r.local()};
      for (int i = r.begin(); i != r.end(); ++i) {
        img.set_pixel(i % ancho, i / ancho, procesar_pixel(i, ancho, ctx, rngs));
      }
    };

    ejecutar_kernel(ancho * cam.getalto(), grano, particionador, loop);
  }

}  // namespace render
