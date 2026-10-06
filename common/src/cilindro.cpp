#include <cilindro.hpp>
#include <cmath>
#include <rayo.hpp>
#include <stdexcept>
#include <vector.hpp>

namespace render {

  /**
   * @brief Valida los parámetros del cilindro
   * @throws std::invalid_argument Si el radio es inválido o el eje es nulo
   */
  void cilindro::validar() const {
    if (radio <= 0) {
      throw std::invalid_argument("Radio del cilindro inválido");
    }
    if (eje.norm() == 0) {
      throw std::invalid_argument("Eje del cilindro no puede ser nulo");
    }
  }

  /**
   * @brief Calcula la intersección con un cilindro infinito (sin límites de altura)
   * @param ray Rayo a intersectar
   * @param t Parámetro de distancia de intersección (salida)
   * @return true si hay intersección con el cilindro infinito
   */
  bool cilindro::interseccionar_infinito(rayo const & ray, double & t) const {
    // Extraer componentes para optimizar cálculos
    double const dx = ray.get_direccion().get_x(), dy = ray.get_direccion().get_y(),
                 dz = ray.get_direccion().get_z();
    double const ox = ray.get_origen().get_x(), oy = ray.get_origen().get_y(),
                 oz = ray.get_origen().get_z();
    double const cx = centro.get_x(), cy = centro.get_y(), cz = centro.get_z();
    double const ahx = a_hat.get_x(), ahy = a_hat.get_y(), ahz = a_hat.get_z();
    // Vector desde el centro al origen del rayo
    double const ocx = ox - cx, ocy = oy - cy, ocz = oz - cz;
    // Proyecciones en la dirección del eje
    double const dr_dot_a = dx * ahx + dy * ahy + dz * ahz;
    double const oc_dot_a = ocx * ahx + ocy * ahy + ocz * ahz;
    // Componentes perpendiculares al eje
    double const dr_perp_x = dx - ahx * dr_dot_a;
    double const dr_perp_y = dy - ahy * dr_dot_a;
    double const dr_perp_z = dz - ahz * dr_dot_a;
    double const oc_perp_x = ocx - ahx * oc_dot_a;
    double const oc_perp_y = ocy - ahy * oc_dot_a;
    double const oc_perp_z = ocz - ahz * oc_dot_a;
    // Coeficientes de la ecuación cuadrática
    double const a = dr_perp_x * dr_perp_x + dr_perp_y * dr_perp_y + dr_perp_z * dr_perp_z;
    double const b = 2.0 * (dr_perp_x * oc_perp_x + dr_perp_y * oc_perp_y + dr_perp_z * oc_perp_z);
    double const c =
        oc_perp_x * oc_perp_x + oc_perp_y * oc_perp_y + oc_perp_z * oc_perp_z - radio * radio;
    // Calcular discriminante
    double const disc = b * b - 4.0 * a * c;
    if (disc < 0.0) {
      return false;
    }
    // Calcular raíces
    double const sqrt_disc = std::sqrt(disc);
    double const inv_2a    = 1.0 / (2.0 * a);
    double const t0        = (-b - sqrt_disc) * inv_2a;
    double const t1        = (-b + sqrt_disc) * inv_2a;
    // Seleccionar la raíz positiva más cercana
    t = (t0 > 1e-3) ? t0 : t1;
    return t > 1e-3;
  }

  /**
   * @brief Calcula intersección con la superficie lateral del cilindro
   * @param ray Rayo a intersectar
   * @param t Parámetro de distancia de intersección (salida)
   * @param p Punto de intersección (salida)
   * @param n Normal en el punto de intersección (salida)
   * @return true si hay intersección válida con la superficie lateral
   */
  bool cilindro::intersectar_superficie(rayo const & ray, double & t, vector & p,
                                        vector & n) const {
    if (!interseccionar_infinito(ray, t)) {
      return false;
    }
    double const dx = ray.get_direccion().get_x(), dy = ray.get_direccion().get_y(),
                 dz = ray.get_direccion().get_z();
    double const ox = ray.get_origen().get_x(), oy = ray.get_origen().get_y(),
                 oz = ray.get_origen().get_z();
    double const cx = centro.get_x(), cy = centro.get_y(), cz = centro.get_z();
    double const ahx = a_hat.get_x(), ahy = a_hat.get_y(), ahz = a_hat.get_z();
    double const px = ox + dx * t, py = oy + dy * t, pz = oz + dz * t;
    p                 = vector(px, py, pz);
    double const proj = (px - cx) * ahx + (py - cy) * ahy + (pz - cz) * ahz;
    if (proj < -altura * 0.5 or proj > altura * 0.5) {
      return false;
    }
    // La fórmula es (I-C) - ((I-C).a)a.
    // NO SE DEBE NORMALIZAR (no dividir por su magnitud).
    double nx = (px - cx) - ahx * proj;
    double ny = (py - cy) - ahy * proj;
    double nz = (pz - cz) - ahz * proj;

    /* BLOQUE ELIMINADO PARA CORREGIR RMSE (NORMALIZACION)
    double const n_mag = std::sqrt(nx * nx + ny * ny + nz * nz);
    if (n_mag > 1e-9) {
      double const inv_mag = 1.0 / n_mag;
      nx *= inv_mag;
      ny *= inv_mag;
      nz *= inv_mag;
    }
    */

    // Asegurar que la normal apunte hacia el rayo incidente
    if (dx * nx + dy * ny + dz * nz > 0.0) {
      nx = -nx;
      ny = -ny;
      nz = -nz;
    }
    n = vector(nx, ny, nz);
    return true;
  }

  /**
   * @brief Calcula intersección con la base inferior del cilindro
   * @param ray Rayo a intersectar
   * @param t Parámetro de distancia de intersección (salida)
   * @param p Punto de intersección (salida)
   * @param n Normal en el punto de intersección (salida)
   * @return true si hay intersección con la base inferior
   */
  bool cilindro::intersectar_base_inferior(rayo const & ray, double & t, vector & p,
                                           vector & n) const {
    double const ahx = a_hat.get_x(), ahy = a_hat.get_y(), ahz = a_hat.get_z();
    double const cx = centro.get_x(), cy = centro.get_y(), cz = centro.get_z();
    // Normal de la base inferior (opuesta al eje)
    double const nx = -ahx, ny = -ahy, nz = -ahz;
    // Centro de la base inferior
    double const pcx = cx - ahx * altura * 0.5;
    double const pcy = cy - ahy * altura * 0.5;
    double const pcz = cz - ahz * altura * 0.5;
    // Verificar intersección con el plano de la base
    double const dx = ray.get_direccion().get_x(), dy = ray.get_direccion().get_y(),
                 dz    = ray.get_direccion().get_z();
    double const denom = dx * nx + dy * ny + dz * nz;
    if (std::abs(denom) < 1e-8) {
      return false;
    }
    // Calcular parámetro t de intersección
    double const ox = ray.get_origen().get_x(), oy = ray.get_origen().get_y(),
                 oz     = ray.get_origen().get_z();
    double const t_temp = ((pcx - ox) * nx + (pcy - oy) * ny + (pcz - oz) * nz) / denom;
    if (t_temp < 1e-3) {
      return false;  // Intersección detrás del origen del rayo
    }
    // Calcular punto de intersección
    double const px = ox + dx * t_temp, py = oy + dy * t_temp, pz = oz + dz * t_temp;
    p = vector(px, py, pz);
    // Verificar que el punto esté dentro del radio de la base
    double const pdx = px - pcx, pdy = py - pcy, pdz = pz - pcz;
    if (pdx * pdx + pdy * pdy + pdz * pdz > radio * radio) {
      return false;
    }
    t = t_temp;
    // Ajustar normal según la dirección del rayo
    if (dx * nx + dy * ny + dz * nz > 0.0) {
      n = vector(-nx, -ny, -nz);  // Normal apuntando hacia el rayo
    } else {
      n = vector(nx, ny, nz);
    }
    return true;
  }

  /**
   * @brief Calcula intersección con la base superior del cilindro
   * @param ray Rayo a intersectar
   * @param t Parámetro de distancia de intersección (salida)
   * @param p Punto de intersección (salida)
   * @param n Normal en el punto de intersección (salida)
   * @return true si hay intersección con la base superior
   */
  bool cilindro::intersectar_base_superior(rayo const & ray, double & t, vector & p,
                                           vector & n) const {
    double const ahx = a_hat.get_x(), ahy = a_hat.get_y(), ahz = a_hat.get_z();
    double const cx = centro.get_x(), cy = centro.get_y(), cz = centro.get_z();
    // Normal de la base superior (misma dirección del eje)
    double const nx = ahx, ny = ahy, nz = ahz;
    // Centro de la base superior
    double const pcx = cx + ahx * altura * 0.5;
    double const pcy = cy + ahy * altura * 0.5;
    double const pcz = cz + ahz * altura * 0.5;
    // Verificar intersección con el plano de la base
    double const dx = ray.get_direccion().get_x(), dy = ray.get_direccion().get_y(),
                 dz    = ray.get_direccion().get_z();
    double const denom = dx * nx + dy * ny + dz * nz;
    if (std::abs(denom) < 1e-8) {
      return false;  // Rayo paralelo al plano
    }
    // Calcular parámetro t de intersección
    double const ox = ray.get_origen().get_x(), oy = ray.get_origen().get_y(),
                 oz     = ray.get_origen().get_z();
    double const t_temp = ((pcx - ox) * nx + (pcy - oy) * ny + (pcz - oz) * nz) / denom;
    if (t_temp < 1e-3) {
      return false;  // Intersección detrás del origen del rayo
    }
    // Calcular punto de intersección
    double const px = ox + dx * t_temp, py = oy + dy * t_temp, pz = oz + dz * t_temp;
    p = vector(px, py, pz);
    // Verificar que el punto esté dentro del radio de la base
    double const pdx = px - pcx, pdy = py - pcy, pdz = pz - pcz;
    if (pdx * pdx + pdy * pdy + pdz * pdz > radio * radio) {
      return false;
    }
    t = t_temp;
    // Ajustar normal según la dirección del rayo
    if (dx * nx + dy * ny + dz * nz > 0.0) {
      n = vector(-nx, -ny, -nz);  // Normal apuntando hacia el rayo
    } else {
      n = vector(nx, ny, nz);
    }
    return true;
  }

  /**
   * @brief Calcula la intersección más cercana entre un rayo y el cilindro completo
   * @param ray Rayo a intersectar
   * @param t Parámetro de distancia de la intersección más cercana (salida)
   * @param p Punto de la intersección más cercana (salida)
   * @param n Normal en el punto de intersección más cercana (salida)
   * @return true si hay al menos una intersección válida
   */
  bool cilindro::intersectar(rayo const & ray, double & t, vector & p, vector & n) const {
    double t_temp = 0.0;
    vector p_temp, n_temp;
    bool hit     = false;
    double t_min = 1e9;  // Valor grande para encontrar el mínimo
    // Probar intersección con base superior
    if (intersectar_base_superior(ray, t_temp, p_temp, n_temp) and t_temp < t_min) {
      t_min = t_temp;
      p     = p_temp;
      n     = n_temp;
      hit   = true;
    }
    // Probar intersección con base inferior
    if (intersectar_base_inferior(ray, t_temp, p_temp, n_temp) and t_temp < t_min) {
      t_min = t_temp;
      p     = p_temp;
      n     = n_temp;
      hit   = true;
    }
    // Probar intersección con superficie lateral
    if (intersectar_superficie(ray, t_temp, p_temp, n_temp) and t_temp < t_min) {
      t_min = t_temp;
      p     = p_temp;
      n     = n_temp;
      hit   = true;
    }

    t = t_min;
    return hit;
  }

}  // namespace render
