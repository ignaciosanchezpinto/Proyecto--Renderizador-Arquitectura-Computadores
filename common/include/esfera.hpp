#ifndef RENDER_ESFERA_HPP
#define RENDER_ESFERA_HPP

#include <material.hpp>
#include <rayo.hpp>
#include <vector.hpp>

namespace render {

  /**
   * @brief Representa una esfera
   *
   * Contiene su centro, radio y material. Permite calcular intersecciones con rayos
   * y validar que los parámetros sean correctos
   */
  class esfera {
  public:
    /**
     * @brief Constructor de la esfera
     * @param c Centro de la esfera
     * @param r Radio de la esfera
     * @param m Material de la esfera
     * @throws std::invalid_argument si el radio es menor o igual que cero
     */
    esfera(vector const & c, double r, material const & m) : centro(c), radio(r), mat(m) { }

    /// @brief Devuelve el material de la esfera
    [[nodiscard]] material const & get_material() const { return mat; }

    /**
     * @brief Comprueba si un rayo intersecta con la esfera
     * @param ray Rayo
     * @param t Distancia al punto de intersección
     * @param p Punto de intersección
     * @param n Vector normal en el punto de intersección
     * @return true si el rayo intersecta con la esfera, false en caso contrario
     */
    [[nodiscard]] bool intersectar(rayo const & ray, double & t, vector & p, vector & n) const;

    /**
     * @brief Valida los parámetros de la esfera
     * @throws std::invalid_argument si el radio es inválido
     */
    void validar_esf() const;

  private:
    vector centro;  /// Centro de la esfera
    double radio;   /// Radio de la esfera
    material mat;   /// Material de la superficie de la esfera
  };

}  // namespace render

#endif
