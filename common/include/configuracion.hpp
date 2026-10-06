#ifndef RENDER_CONFIGURACION_HPP
#define RENDER_CONFIGURACION_HPP

#include <array>
#include <string>
#include <vector.hpp>

namespace render {

  /**
   * @brief Clase que almacena la configuración general del renderizado
   *
   * Contiene todos los parámetros necesarios para configurar la cámara,
   * resolución, corrección gamma, semillas RNG y colores de fondo
   */
  class configuracion {
  public:
    /// @brief Constructor que inicializa todos los parámetros de configuración.
    configuracion(std::array<int, 2> aspect_ratio = {16, 9}, int image_width = 1'920,
                  double gamma = 2.2, vector const & camera_position = vector{0.0, 0.0, -10.0},
                  vector const & camera_target = vector{0.0, 0.0, 0.0},
                  vector const & camera_north = vector{0.0, 1.0, 0.0}, double field_of_view = 90.0,
                  int samples_per_pixel = 20, int max_depth = 5, int material_rng_seed = 13,
                  int ray_rng_seed                      = 19,
                  vector const & background_dark_color  = vector{0.25, 0.5, 1.0},
                  vector const & background_light_color = vector{1.0, 1.0, 1.0})
        : aspect_ratio{aspect_ratio}, image_width{image_width}, gamma{gamma},
          camera_position{camera_position}, camera_target{camera_target},
          camera_north{camera_north}, field_of_view{field_of_view},
          samples_per_pixel{samples_per_pixel}, max_depth{max_depth},
          material_rng_seed{material_rng_seed}, ray_rng_seed{ray_rng_seed},
          background_dark_color{background_dark_color},
          background_light_color{background_light_color} { }

    /**
     * @brief Carga y valida una configuración desde un archivo
     * @param nombre_archivo Ruta del archivo de configuración
     * @return Objeto configuracion válido con los valores leídos
     * @throws std::runtime_error Si el archivo no existe o contiene configuraciones inválidas
     */
    static configuracion validar_configuracion(std::string const & nombre_archivo);

    // --- Setters ---

    /**
     * @brief Establece la relación de aspecto de la imagen
     * @param v Nueva relación de aspecto [ancho, alto]
     */
    void set_aspect_ratio(std::array<int, 2> const & v) { aspect_ratio = v; }

    /**
     * @brief Establece el ancho de la imagen en píxeles
     * @param v Nuevo ancho de imagen
     */
    void set_image_width(int v) { image_width = v; }

    /**
     * @brief Establece el valor gamma para corrección de color
     * @param v Nuevo valor gamma
     */
    void set_gamma(double v) { gamma = v; }

    /**
     * @brief Establece la posición de la cámara
     * @param v Nueva posición de la cámara
     */
    void set_camera_position(vector const & v) { camera_position = v; }

    /**
     * @brief Establece el punto de mira de la cámara
     * @param v Nuevo objetivo de la cámara
     */
    void set_camera_target(vector const & v) { camera_target = v; }

    /**
     * @brief Establece el vector "arriba" de la cámara
     * @param v Nuevo vector norte/dirección arriba
     */
    void set_camera_north(vector const & v) { camera_north = v; }

    /**
     * @brief Establece el campo de visión
     * @param v Nuevo campo de visión en grados
     */
    void set_field_of_view(double v) { field_of_view = v; }

    /**
     * @brief Establece el número de muestras por píxel
     * @param v Nuevo número de muestras
     */
    void set_samples_per_pixel(int v) { samples_per_pixel = v; }

    /**
     * @brief Establece la profundidad máxima de recursión
     * @param v Nueva profundidad máxima
     */
    void set_max_depth(int v) { max_depth = v; }

    /**
     * @brief Establece la semilla RNG para materiales
     * @param v Nueva semilla
     */
    void set_material_rng_seed(int v) { material_rng_seed = v; }

    /**
     * @brief Establece la semilla RNG para rayos
     * @param v Nueva semilla
     */
    void set_ray_rng_seed(int v) { ray_rng_seed = v; }

    /**
     * @brief Establece el color oscuro del fondo
     * @param v Nuevo color oscuro
     */
    void set_background_dark_color(vector const & v) { background_dark_color = v; }

    /**
     * @brief Establece el color claro del fondo
     * @param v Nuevo color claro
     */
    void set_background_light_color(vector const & v) { background_light_color = v; }

    // --- Getters ---
    /// @return Relación de aspecto de la imagen (anchura, altura).
    [[nodiscard]] std::array<int, 2> const & get_aspect_ratio() const { return aspect_ratio; }

    /// @return Ancho de la imagen en píxeles.
    [[nodiscard]] int get_image_width() const { return image_width; }

    /// @return Altura de la imagen calculada según la relación de aspecto.
    [[nodiscard]] int get_image_height() const {
      return image_width * aspect_ratio[1] / aspect_ratio[0];
    }

    /// @return Valor gamma para la corrección de color.
    [[nodiscard]] double get_gamma() const { return gamma; }

    /// @return Posición de la cámara.
    [[nodiscard]] vector const & get_camera_position() const { return camera_position; }

    /// @return Punto hacia el que mira la cámara.
    [[nodiscard]] vector const & get_camera_target() const { return camera_target; }

    /// @return Vector que define el eje vertical de la cámara.
    [[nodiscard]] vector const & get_camera_north() const { return camera_north; }

    /// @return Campo de visión en grados.
    [[nodiscard]] double get_field_of_view() const { return field_of_view; }

    /// @return Número de muestras por píxel.
    [[nodiscard]] int get_samples_per_pixel() const { return samples_per_pixel; }

    /// @return Profundidad máxima de recursión de rayos.
    [[nodiscard]] int get_max_depth() const { return max_depth; }

    /// @return Semilla RNG usada para materiales.
    [[nodiscard]] int get_material_rng_seed() const { return material_rng_seed; }

    /// @return Semilla RNG usada para los rayos.
    [[nodiscard]] int get_ray_rng_seed() const { return ray_rng_seed; }

    /// @return Color de fondo oscuro (RGB).
    [[nodiscard]] vector const & get_background_dark_color() const { return background_dark_color; }

    /// @return Color de fondo claro (RGB).
    [[nodiscard]] vector const & get_background_light_color() const {
      return background_light_color;
    }

  private:
    std::array<int, 2> aspect_ratio;  /// Relación de aspecto (ancho, alto)
    int image_width;                  /// Ancho de la imagen en píxeles
    double gamma;                     /// Corrección gamma
    vector camera_position;           /// Posición de la cámara
    vector camera_target;             /// Punto hacia el que mira la cámara
    vector camera_north;              /// Vector "arriba" de la cámara
    double field_of_view;             /// Campo de visión en grados
    int samples_per_pixel;            /// Muestras por píxel
    int max_depth;                    /// Profundidad máxima de los rayos
    int material_rng_seed;            /// Semilla RNG para materiales
    int ray_rng_seed;                 /// Semilla RNG para rayos
    vector background_dark_color;     /// Color oscuro del fondo
    vector background_light_color;    ///  Color claro del fondo
  };

}  // namespace render

#endif
