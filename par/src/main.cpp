#include "camara.hpp"
#include "configuracion.hpp"
#include "escenario.hpp"
#include "imagen.hpp"
#include "leer_escena.hpp"
#include "renderizador.hpp"
#include <cstddef>
#include <exception>
#include <format>
#include <iostream>
#include <span>
#include <string>
#include <vector>

// TBB headers específicos
#include <oneapi/tbb/global_control.h>  // ← tbb::global_control

int main(int argc, char ** argv) try
{
  std::span<char * const> const spn(argv, static_cast<std::size_t>(argc));
  std::vector<std::string> args(spn.begin(), spn.end());
  if (args.size() < 4) {
    std::cerr << std::format("Uso: {} <config> <escena> <salida> [hilos] [part] [grano]\n",
                             args[0]);
    return 1;
  }
  int const n_threads             = args.size() >= 5 ? std::stoi(args[4]) : 64;
  std::string const particionador = args.size() >= 6 ? args[5] : "auto";
  int const grano                 = args.size() >= 7 ? std::stoi(args[6]) : 64;
  tbb::global_control const global_limit(tbb::global_control::max_allowed_parallelism,
                                         static_cast<std::size_t>(n_threads));
  render::renderizador::configurar(particionador, static_cast<std::size_t>(grano));
  std::cout << "Ejecutando con: " << n_threads << " hilos, " << particionador << ", grano " << grano
            << "\n";
  render::configuracion const conf = render::configuracion::validar_configuracion(args[1]);
  render::escenario const escena   = render::leer_escena::cargar_desde_archivo(args[2]);
  render::camara cam(conf);
  render::imagen img(cam.getancho(), cam.getalto());
  render::renderizador::renderizar(conf, cam, escena, img);
  img.guardar_ppm(args[3]);
  std::cout << "Renderizado completado correctamente.\n";
  return 0;
} catch (std::exception const & e) {
  std::cerr << "Error: " << e.what() << "\n";
  return 1;
}
