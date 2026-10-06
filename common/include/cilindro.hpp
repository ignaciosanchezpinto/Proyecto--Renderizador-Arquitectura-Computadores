#ifndef RENDER_CILINDRO_HPP
#define RENDER_CILINDRO_HPP

#include <material.hpp>
#include <rayo.hpp>
#include <vector.hpp>

namespace render {

  /**
   * @brief Representa un cilindro 3D con centro, radio, eje y material
   *
   * El cilindro está definido por un centro, radio, vector de eje (que define
   * la dirección y altura), y un material. Soporta intersecciones con rayos
   * para renderizado por trazado de rayos.
   */
  class cilindro {
  public:
    /**
     * @brief Constructor del cilindro
     * @param c Centro del cilindro
     * @param r Radio del cilindro
     * @param m Material del cilindro
     * @param eje Vector que define la dirección y altura del cilindro
     */
    cilindro(vector const & c, double r, material const & m, vector const & eje)
        : centro(c), radio(r), mat(m), eje(eje), a_hat(eje.normalized()), altura(eje.norm()) { }

    /**
     * @brief Obtiene el material del cilindro
     * @return Referencia constante al material
     */
    [[nodiscard]] material const & get_material() const { return mat; }

    /**
     * @brief Calcula la intersección entre un rayo y el cilindro
     * @param ray Rayo a intersectar
     * @param t Parámetro de distancia de intersección (salida)
     * @param p Punto de intersección (salida)
     * @param n Normal en el punto de intersección (salida)
     * @return true si hay intersección, false en caso contrario
     */
    [[nodiscard]] bool intersectar(rayo const & ray, double & t, vector & p, vector & n) const;

    /**
     * @brief Valida la consistencia del cilindro
     * @throws std::runtime_error Si el cilindro tiene parámetros inválidos
     */
    void validar() const;

    /**
     * @brief Verifica si un punto está dentro de los límites de altura del cilindro
     * @param p Punto a verificar
     * @return true si el punto está dentro de los límites de altura, false en caso contrario
     */
    [[nodiscard]] bool dentro_de_altura(vector const & p) const;

    /**
     * @brief Calcula la normal en un punto de la superficie del cilindro
     * @param dr Dirección del rayo incidente
     * @param p Punto en la superficie
     * @param n Normal calculada (salida)
     */
    void calcular_normal(vector const & dr, vector const & p, vector & n) const;

  private:
    /**
     * @brief Intersección con el cilindro infinito (sin chequear altura).
     */
    [[nodiscard]] bool interseccionar_infinito(rayo const & ray, double & t) const;

    /**
     * @brief Comprueba la intersección del rayo con la superficie lateral.
     */
    [[nodiscard]] bool intersectar_superficie(rayo const & ray, double & t, vector & p,
                                              vector & n) const;

    /**
     * @brief Comprueba la intersección del rayo con la base superior.
     */
    [[nodiscard]] bool intersectar_base_superior(rayo const & ray, double & t, vector & p,
                                                 vector & n) const;

    /**
     * @brief Comprueba la intersección del rayo con la base inferior.
     */
    [[nodiscard]] bool intersectar_base_inferior(rayo const & ray, double & t, vector & p,
                                                 vector & n) const;

    vector centro;  // centro del cilindro
    double radio;   // radio del cilindro
    material mat;   // material del cilindro
    vector eje;     // vector que define la direccion y altura del cilindro
    vector a_hat;   // vector unitario en la direccion del eje
    double altura;  // altura del cilindro
  };

}  // namespace render

#endif
