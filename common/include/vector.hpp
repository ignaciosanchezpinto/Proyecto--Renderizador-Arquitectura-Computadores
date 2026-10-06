#ifndef RENDER_VECTOR_HPP
#define RENDER_VECTOR_HPP

#include <cmath>

namespace render {

  /**
   * @brief Clase que representa un vector tridimensional con operaciones básicas.
   */
  class vector {
  public:
    /**
     * @brief Constructor con componentes iniciales.
     * @param cx Componente X del vector.
     * @param cy Componente Y del vector.
     * @param cz Componente Z del vector.
     */
    vector(double cx, double cy, double cz) : x{cx}, y{cy}, z{cz} { }

    /**
     * @brief Constructor por defecto. Inicializa el vector en (0, 0, 0).
     */
    vector() : x{0.0}, y{0.0}, z{0.0} { }

    /**
     * @brief Calcula la norma (longitud) del vector.
     * @return Norma del vector.
     */
    [[nodiscard]] double norm() const { return std::sqrt(x * x + y * y + z * z); }

    /**
     * @brief Devuelve el vector normalizado (unitario).
     * @return Vector unitario. Si es (0,0,0), devuelve (0,0,0).
     */
    [[nodiscard]] vector normalized() const {
      double const n = norm();
      if (n < 1e-9) {
        return {0, 0, 0};
      }
      double const inv_n = 1.0 / n;
      return {x * inv_n, y * inv_n, z * inv_n};
    }

    /**
     * @brief Calcula el cuadrado de la norma (más rápido para comparaciones).
     * @return Cuadrado de la norma del vector.
     */
    [[nodiscard]] double squared_norm() const { return x * x + y * y + z * z; }

    /**
     * @brief Suma otro vector a este (in-place, más eficiente).
     * @param v Vector a sumar.
     * @return Referencia al propio vector modificado.
     */
    vector & add(vector const & v) {
      x += v.x;
      y += v.y;
      z += v.z;
      return *this;
    }

    /**
     * @brief Resta otro vector a este (in-place, más eficiente).
     * @param v Vector a restar.
     * @return Referencia al propio vector modificado.
     */
    vector & subtract(vector const & v) {
      x -= v.x;
      y -= v.y;
      z -= v.z;
      return *this;
    }

    /**
     * @brief Multiplica este vector por un escalar (in-place, más eficiente).
     * @param s Escalar a multiplicar.
     * @return Referencia al propio vector modificado.
     */
    vector & multiply(double s) {
      x *= s;
      y *= s;
      z *= s;
      return *this;
    }

    /**
     * @brief Multiplica este vector por otro componente a componente (in-place).
     * @param v Vector a multiplicar.
     * @return Referencia al propio vector modificado.
     */
    vector & multiply(vector const & v) {
      x *= v.x;
      y *= v.y;
      z *= v.z;
      return *this;
    }

    /**
     * @brief Obtiene la componente X del vector.
     * @return Valor de la componente X.
     */
    [[nodiscard]] double get_x() const { return x; }

    /**
     * @brief Obtiene la componente Y del vector.
     * @return Valor de la componente Y.
     */
    [[nodiscard]] double get_y() const { return y; }

    /**
     * @brief Obtiene la componente Z del vector.
     * @return Valor de la componente Z.
     */
    [[nodiscard]] double get_z() const { return z; }

    /**
     * @brief Suma este vector con otro.
     * @param v Vector a sumar.
     * @return Resultado de la suma.
     */
    vector operator+(vector const & v) const { return {x + v.x, y + v.y, z + v.z}; }

    /**
     * @brief Resta otro vector de este.
     * @param v Vector a restar.
     * @return Resultado de la resta.
     */
    vector operator-(vector const & v) const { return {x - v.x, y - v.y, z - v.z}; }

    /**
     * @brief Multiplica el vector por un escalar.
     * @param escalar Valor a multiplicar.
     * @return Vector resultante.
     */
    vector operator*(double escalar) const { return {x * escalar, y * escalar, z * escalar}; }

    /**
     * @brief Divide el vector por un escalar.
     * @param escalar Valor escalar (distinto de 0).
     * @return Vector resultante.
     */
    vector operator/(double escalar) const {
      double const inv_escalar = 1.0 / escalar;
      return {x * inv_escalar, y * inv_escalar, z * inv_escalar};
    }

    /**
     * @brief Aplica la negación del vector.
     * @return Vector con componentes negativas.
     */
    vector operator-() const { return {-x, -y, -z}; }

    /**
     * @brief Multiplica dos vectores componente a componente.
     * @param v Segundo vector.
     * @return Vector resultante.
     */
    vector operator*(vector const & v) const { return {x * v.x, y * v.y, z * v.z}; }

    /**
     * @brief Suma otro vector a este.
     * @param v Vector a sumar.
     * @return Referencia al propio vector modificado.
     */
    vector & operator+=(vector const & v) {
      x += v.x;
      y += v.y;
      z += v.z;
      return *this;
    }

    /**
     * @brief Resta otro vector a este (operador de asignación)
     * @param v Vector a restar
     * @return Referencia al propio vector modificado
     */
    vector & operator-=(vector const & v) {
      x -= v.x;
      y -= v.y;
      z -= v.z;
      return *this;
    }

    /**
     * @brief Multiplica este vector por un escalar.
     * @param escalar Valor escalar.
     * @return Referencia al propio vector modificado.
     */
    vector & operator*=(double escalar) {
      x *= escalar;
      y *= escalar;
      z *= escalar;
      return *this;
    }

    /**
     * @brief Multiplica este vector por otro (componente a componente).
     * @param v Segundo vector.
     * @return Referencia al propio vector modificado.
     */
    vector & operator*=(vector const & v) {
      x *= v.x;
      y *= v.y;
      z *= v.z;
      return *this;
    }

    /**
     * @brief Calcula el producto punto con otro vector.
     * @param v Segundo vector.
     * @return Resultado del producto escalar.
     */
    [[nodiscard]] double dot(vector const & v) const { return x * v.x + y * v.y + z * v.z; }

    /**
     * @brief Calcula el producto cruzado con otro vector.
     * @param v Segundo vector.
     * @return Vector perpendicular a ambos vectores.
     */
    [[nodiscard]] vector cross(vector const & v) const {
      return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
    }

  private:
    double x;  // Componente X
    double y;  // Componente Y
    double z;  // Componente Z
  };

  /**
   * @brief Multiplica un escalar por un vector.
   * @param escalar Valor escalar.
   * @param v Vector a multiplicar.
   * @return Vector resultante.
   */
  inline vector operator*(double escalar, vector const & v) {
    return {v.get_x() * escalar, v.get_y() * escalar, v.get_z() * escalar};
  }

  /**
   * @brief Comprueba si dos vectores son iguales (con tolerancia EPS).
   * @param a Primer vector.
   * @param b Segundo vector.
   * @return true si son iguales, false si no.
   */
  inline bool operator==(vector const & a, vector const & b) {
    constexpr double EPS = 1e-9;
    return std::abs(a.get_x() - b.get_x()) < EPS and
           std::abs(a.get_y() - b.get_y()) < EPS and
           std::abs(a.get_z() - b.get_z()) < EPS;
  }

  /**
   * @brief Comprueba si dos vectores son distintos.
   * @param a Primer vector.
   * @param b Segundo vector.
   * @return true si son distintos, false si no.
   */
  inline bool operator!=(vector const & a, vector const & b) {
    return !(a == b);
  }

}  // namespace render

#endif
