#ifndef RENDER_MATERIAL_HPP
#define RENDER_MATERIAL_HPP

#include <vector.hpp>

namespace render {

  /// @brief Tipos posibles de material que puede tener un objeto
  enum class TipoMaterial { Mate, Metal, Refractivo };

  /// @brief Representa un material con sus propiedades físicas y ópticas.
  class material {
  public:
    
    /// @brief Constructor del material
    /// @param t Tipo de material (Mate, Metal o Refractivo)
    /// @param c Color del material (componentes entre 0 y 1)
    /// @param f Factor de reflexión en los materiales metálicos
    /// @param i Índice de refracción en los materiales refractivos
    material(TipoMaterial t, vector const & c, double f = 0.0, double i = 1.0)
        : tipo(t), color(c), refl(f), refrac(i) {
      validar();  // Validamos los parámetros del constructor
    }

    /// @brief Devuelve el tipo de material
    [[nodiscard]] TipoMaterial get_tipo() const;

    /// @brief Devuelve el color del material
    [[nodiscard]] vector const & get_color() const;

    /// @brief Devuelve el factor de reflexión
    [[nodiscard]] double get_refl() const;

    /// @brief Devuelve el índice de refracción
    [[nodiscard]] double get_refrac() const;

    /// @brief Verifica que los valores del material sean válidos
    void validar() const;

  private:
    TipoMaterial tipo;  /// Tipo de material
    vector color;       /// Color base del material
    double refl;        /// Factor de reflexión (solo para metales)
    double refrac;      /// Índice de refracción (solo para refractivos)
  };

}  // namespace render

#endif
