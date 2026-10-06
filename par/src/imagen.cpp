#include "imagen.hpp"
#include <cstddef>
#include <fstream>
#include <stdexcept>
#include <string>

namespace render {

  void imagen::set_pixel(int x, int y, Pixel const & p) {
    // Validar que las coordenadas estén dentro de los límites
    if (x < 0 or x >= ancho or y < 0 or y >= alto) {
      throw std::out_of_range("Coordenadas de pixel fuera de rango");
    }

    // Calcular índice lineal en el buffer de datos
    auto const index =
        static_cast<std::size_t>(y) * static_cast<std::size_t>(ancho) + static_cast<std::size_t>(x);
    datos[index] = p;
  }

  void imagen::guardar_ppm(std::string const & ruta) const {
    // Abrir archivo para escritura
    std::ofstream file(ruta);
    if (!file) {
      throw std::runtime_error("No se pudo abrir el archivo PPM");
    }
    // Escribir cabecera PPM
    file << "P3\n" << ancho << " " << alto << "\n255\n";
    // Escribir datos de píxeles en formato RGB
    for (auto const & p : datos) {
      file << static_cast<int>(p.r) << ' ' << static_cast<int>(p.g) << ' ' << static_cast<int>(p.b)
           << '\n';
    }
  }

}  // namespace render
