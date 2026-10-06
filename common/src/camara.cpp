#include <camara.hpp>
#include <cmath>
#include <numbers>
#include <random>
#include <rayo.hpp>
#include <vector.hpp>

namespace render {

  /**
   * @brief Calcula la ventana de proyección de la cámara
   *
   * Determina las dimensiones del plano de proyección
   * en función del campo de visión y de la distancia focal, también establece los vectores
   * base que servirán para posicionar cada píxel en el espacio 3D
   */
  void camara::calcular_ventana_proyeccion() {
    vector const vf = posicion - destino;
    double const df = vf.norm();

    vector const vf_norm   = vf.normalized();
    double const alpha_rad = vista * std::numbers::pi / 180.0;
    double const hp        = 2.0 * std::tan(alpha_rad / 2.0) * df;
    double const wp        = hp * (static_cast<double>(ancho) / alto);

    vector const u = (norte.cross(vf_norm)).normalized();
    vector const v = (vf_norm.cross(u)).normalized();

    ph      = u * wp;
    pv      = v * (-hp);
    delta_x = ph / static_cast<double>(ancho);
    delta_y = pv / static_cast<double>(alto);
    origen  = posicion - vf - (ph + pv) * 0.5 + (delta_x + delta_y) * 0.5;
  }

  /**
   * @brief Genera un rayo que pasa por el píxel indicado
   *
   * Utiliza un generador aleatorio para aplicar un pequeño desplazamiento dentro del píxel,
   * lo que mejora la calidad visual
   *
   * @param fila Fila del píxel
   * @param columna Columna del píxel
   * @param ray_rng Generador de números aleatorios
   * @return rayo con origen en la cámara y dirección hacia el píxel correspondiente
   */
  rayo camara::generar_rayo(int fila, int columna, std::mt19937_64 & ray_rng) {
    std::uniform_real_distribution<double> dist(-0.5, 0.5);
    double const dx = dist(ray_rng);
    double const dy = dist(ray_rng);

    // Cálculos directos sin temporales
    double const col_d = columna + dx;
    double const fil_d = fila + dy;

    // Cálculo directo sin operadores sobrecargados
    vector q = origen;
    q += delta_x * col_d;
    q += delta_y * fil_d;

    // Dirección calculada directamente
    vector const direccion(q.get_x() - posicion.get_x(), q.get_y() - posicion.get_y(),
                           q.get_z() - posicion.get_z());

    return {posicion, direccion};
  }

}  // namespace render
