#include <cmath>
#include <esfera.hpp>
#include <rayo.hpp>
#include <stdexcept>
#include <vector.hpp>

namespace render {

  /**
   * @brief Valida los parámetros de la esfera
   * @throws std::invalid_argument Si el radio no es positivo
   */
  void esfera::validar_esf() const {
    if (radio <= 0) {
      throw std::invalid_argument("Radio de la esfera inválido (debe ser positivo)");
    }
  }

  /**
   * @brief Calcula la intersección entre un rayo y la esfera
   * @param ray Rayo a intersectar
   * @param t Parámetro de distancia de intersección (salida)
   * @param p Punto de intersección (salida)
   * @param n Normal en el punto de intersección (salida)
   * @return true si hay intersección válida
   *
   * @brief Resuelve la ecuación cuadrática para la esfera de forma optimizada,
   * utilizando simplificaciones matemáticas para mejorar el rendimiento
   */
  bool esfera::intersectar(rayo const & ray, double & t, vector & p, vector & n) const {
    // Extraer componentes para cálculos optimizados
    double const ox = ray.get_origen().get_x(), oy = ray.get_origen().get_y(),
                 oz = ray.get_origen().get_z();
    double const dx = ray.get_direccion().get_x(), dy = ray.get_direccion().get_y(),
                 dz = ray.get_direccion().get_z();
    double const cx = centro.get_x(), cy = centro.get_y(), cz = centro.get_z();

    // Vector desde el centro de la esfera al origen del rayo
    double const ocx = ox - cx, ocy = oy - cy, ocz = oz - cz;
    // Coeficientes optimizados de la ecuación cuadrática
    // b = 2 * (oc · d) se simplifica a b = (oc · d) y luego se multiplica por 2 implícitamente
    double const b = ocx * dx + ocy * dy + ocz * dz;  // Representa b/2 en la fórmula estándar
    double const c = ocx * ocx + ocy * ocy + ocz * ocz - radio * radio;
    // Si c > 0 y b > 0, el rayo no puede intersectar
    if (c > 0.0 and b > 0.0) {
      return false;
    }
    // Calcular discriminante (dividido por 4)
    double const disc = b * b - c;  // Representa (b² - 4ac)/4
    if (disc < 0.0) {
      return false;  // No hay intersección real
    }
    // Calcular raíces
    double const sqrt_disc = std::sqrt(disc);
    double const t0        = -b - sqrt_disc;
    double const t1        = -b + sqrt_disc;
    // Seleccionar la raíz positiva más cercana
    t = (t0 > 1e-3) ? t0 : t1;
    if (t < 1e-3) {
      return false;  // Intersección detrás del origen del rayo
    }
    // Calcular punto de intersección
    double const px = ox + dx * t, py = oy + dy * t, pz = oz + dz * t;
    p = vector(px, py, pz);
    // Calcular normal (vector desde centro a punto de intersección, normalizado)
    double const inv_radio = 1.0 / radio;
    n = vector((px - cx) * inv_radio, (py - cy) * inv_radio, (pz - cz) * inv_radio);

    return true;
  }

}  // namespace render
