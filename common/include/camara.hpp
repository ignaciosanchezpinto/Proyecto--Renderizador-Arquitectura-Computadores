#ifndef RENDER_CAMARA_HPP
#define RENDER_CAMARA_HPP

#include <cmath>
#include <configuracion.hpp>
#include <random>
#include <rayo.hpp>
#include <stdexcept>
#include <vector.hpp>

namespace render {

  /**
   * @brief Representa una cámara virtual
   *
   * La cámara se configura a partir de una estructura de configuración e incluye
   * los parámetros necesarios para generar rayos según su posición, dirección de
   * observación, campo de visión y dimensiones de la imagen
   */
  class camara {
  public:
    /**
     * @brief Constructor de la cámara.
     *
     * Inicializa la cámara con los valores de posición, destino, norte, vista, ancho y alto
     * obtenidos desde una configuración. También calcula la ventana de proyección.
     *
     * @param conf Objeto de configuración que contiene los parámetros de la cámara
     * @throws da un std::invalid_argument si las dimensiones o el campo de visión son inválidos
     */
    explicit camara(configuracion const & conf)
        : posicion{conf.get_camera_position()}, destino{conf.get_camera_target()},
          norte{conf.get_camera_north()}, vista{conf.get_field_of_view()},
          ancho{conf.get_image_width()}, alto{conf.get_image_height()} {
      if (ancho <= 0 or alto <= 0) {
        throw std::invalid_argument("camara: dimensiones inválidas");
      }
      if (vista <= 0.0 or vista >= 180.0) {
        throw std::invalid_argument("camara: campo de visión inválido");
      }
      calcular_ventana_proyeccion();
    }

    /**
     * @brief Genera un rayo que corresponde a una posición de píxel en la imagen
     *
     * Cada rayo incorpora un pequeño desplazamiento aleatorio generado con el
     * número aleatorio pasado como parámetro.
     *
     * @param fila Fila del píxel
     * @param columna Columna del píxel
     * @param ray_rng Generador de números aleatorios
     * @return rayo que sale desde la posición de la cámara hacia el píxel
     */
    rayo generar_rayo(int fila, int columna, std::mt19937_64 & ray_rng);

    /// @brief Devuelve el ancho de la imagen
    [[nodiscard]] int getancho() const noexcept { return ancho; }

    /// @brief Devuelve el alto de la imagen
    [[nodiscard]] int getalto() const noexcept { return alto; }

    /// @brief Devuelve el origen de los rayos
    [[nodiscard]] vector const & getorigen() const noexcept { return origen; }

  private:
    vector posicion;  /// Posición de la cámara
    vector destino;   /// Punto hacia el que mira la cámara
    vector norte;     /// Vector que define la orientación “hacia arriba”
    double vista;     /// Campo de visión en grados
    int ancho;        /// Ancho de la imagen en píxeles
    int alto;         /// Alto de la imagen en píxeles

    vector origen;   /// Punto inicial desde el que se proyectan los rayos
    vector ph;       /// Vector horizontal del plano de proyección
    vector pv;       /// Vector vertical del plano de proyección
    vector delta_x;  /// Incremento horizontal
    vector delta_y;  /// Incremento vertical

    /**
     * @brief Calcula la ventana de proyección de la cámara
     */
    void calcular_ventana_proyeccion();
  };

}  // namespace render
#endif
