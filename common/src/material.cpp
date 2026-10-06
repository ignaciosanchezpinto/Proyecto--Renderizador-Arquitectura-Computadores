#include <material.hpp>
#include <stdexcept>
#include <vector.hpp>

namespace render {

  /// @brief Devuelve el tipo de material
  TipoMaterial material::get_tipo() const {
    return tipo;
  }

  /// @brief Devuelve el color de material
  vector const & material::get_color() const {
    return color;
  }

  /// @brief Devuelve el factor de reflexión del material
  double material::get_refl() const {
    return refl;
  }

  /// @brief Devuelve el índice de refracción del material
  double material::get_refrac() const {
    return refrac;
  }

  /// @brief Valida que el color del material esté dentro del rango [0, 1].
  /// @throws std::invalid_argument Si alguna componente del color está fuera del rango.
  void material::validar() const {
    // Validar color (cada componente entre 0 y 1)
    if (color.get_x() < 0.0 or
        color.get_x() > 1.0 or
        color.get_y() < 0.0 or
        color.get_y() > 1.0 or
        color.get_z() < 0.0 or
        color.get_z() > 1.0)
    {
      throw std::invalid_argument("Color fuera de rango [0,1]");
    }
  }

}  // namespace render
