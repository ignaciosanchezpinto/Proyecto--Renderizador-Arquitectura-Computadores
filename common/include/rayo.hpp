#ifndef RENDER_RAYO_HPP
#define RENDER_RAYO_HPP

#include <cmath>
#include <iostream>
#include <vector.hpp>

namespace render {

  class rayo {
  public:
    /// @brief Constructor sin normalización automática
    rayo(vector const & o, vector const & d) noexcept : origen{o}, direccion{d.normalized()} { }

    /// @brief Devuelve el punto de origen del rayo.
    [[nodiscard]] vector const & get_origen() const noexcept { return origen; }

    /// @brief Devuelve la dirección del rayo (no necesariamente unitaria).
    [[nodiscard]] vector const & get_direccion() const noexcept { return direccion; }

    /// @brief Calcula el punto P(t) = origen + t * dirección.
    [[nodiscard]] vector punto(double t) const noexcept;

    /// @brief Alias de punto(t)
    [[nodiscard]] vector at(double t) const noexcept;

    /// @brief Normaliza la dirección del rayo.
    void normalizar_direccion() noexcept;

    /// @brief Crea una copia del rayo desplazada a lo largo de la normal.
    /// @param normal Vector normal que define la dirección del desplazamiento
    /// @param epsilon Distancia a desplazar (1e-4)
    [[nodiscard]] rayo desplazado(vector const & normal, double epsilon = 1e-4) const noexcept;

    /// @brief Operador de salida para depuración
    friend std::ostream & operator<<(std::ostream & os, rayo const & r);

  private:
    vector origen;     /// Origen del rayo
    vector direccion;  /// Dirección del rayo
  };

}  // namespace render

#endif  // RENDER_RAYO_HPP
