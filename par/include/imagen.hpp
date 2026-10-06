#ifndef RENDER_IMAGEN_HPP
#define RENDER_IMAGEN_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace render {

  /**
   * @brief Estructura que representa un píxel en formato RGB
   */
  struct Pixel {
    uint8_t r, g, b;  // componentes rojo, verdo y azul del pixel
  };

  /**
   * @brief Clase para representar y manipular imágenes
   *
   * Proporciona funcionalidad para crear imágenes, establecer píxeles
   * y guardar en formato PPM
   */
  class imagen {
  public:
    /**
     * @brief Constructor que crea una imagen con dimensiones específicas
     * @param n_columnas Ancho de la imagen en píxeles
     * @param n_filas Alto de la imagen en píxeles
     */
    imagen(int n_columnas, int n_filas)
        : ancho{n_columnas}, alto{n_filas}, datos(static_cast<std::size_t>(n_columnas * n_filas)) {
    }

    // Establece el color de un pixel, que se encuentra en la coordenada x,y
    void set_pixel(int x, int y, Pixel const & p);

    // Guarda la imagen en formato PPM
    void guardar_ppm(std::string const & ruta) const;

    // Obtiene el ancho de la imagen
    [[nodiscard]] int get_ancho() const { return ancho; }

    // Obtiene el alto de la imagen
    [[nodiscard]] int get_alto() const { return alto; }

  private:
    int ancho;
    int alto;
    std::vector<Pixel> datos;
  };

}  // namespace render

#endif
