#include <iostream>
#include <rayo.hpp>
#include <vector.hpp>

namespace render {

  /// @brief Calcula P(t) = origen + t * direccion
  vector rayo::punto(double t) const noexcept {
    // En lugar de: return origen + direccion * t;
    double const x = origen.get_x() + direccion.get_x() * t;
    double const y = origen.get_y() + direccion.get_y() * t;
    double const z = origen.get_z() + direccion.get_z() * t;
    return {x, y, z};
  }

  /// @brief Alias de punto(t)
  vector rayo::at(double t) const noexcept {
    return punto(t);
  }

  /// @brief Devuelve un rayo desplazado a lo largo de una normal
  rayo rayo::desplazado(vector const & normal, double epsilon) const noexcept {
    return {origen + normal * epsilon, direccion};
  }

  /// @brief Imprime el rayo para depuración
  std::ostream & operator<<(std::ostream & os, rayo const & r) {
    os << "rayo(origen=(" << r.get_origen().get_x() << ", " << r.get_origen().get_y() << ", "
       << r.get_origen().get_z() << "), direccion=(" << r.get_direccion().get_x() << ", "
       << r.get_direccion().get_y() << ", " << r.get_direccion().get_z() << "))";
    return os;
  }

}  // namespace render
