#include <cilindro.hpp>
#include <escenario.hpp>
#include <esfera.hpp>
#include <material.hpp>
#include <rayo.hpp>
#include <stdexcept>
#include <string>
#include <vector.hpp>

namespace render {

  /**
   * @brief Agrega un material al escenario
   * @param nombre Nombre único del material
   * @param m Material a agregar
   */
  void escenario::agregar_material(std::string const & nombre, material const & m) {
    materiales.emplace(nombre, m);
  }

  /**
   * @brief Agrega una esfera al escenario
   * @param e Esfera a agregar
   */
  void escenario::agregar_esfera(esfera const & e) {
    esferas.push_back(e);
  }

  /**
   * @brief Agrega un cilindro al escenario
   * @param c Cilindro a agregar
   */
  void escenario::agregar_cilindro(cilindro const & c) {
    cilindros.push_back(c);
  }

  /**
   * @brief Verifica si un material existe en el escenario
   * @param nombre Nombre del material a buscar
   * @return true si el material existe, false en caso contrario
   */
  bool escenario::tiene_material(std::string const & nombre) const {
    return materiales.contains(nombre);
  }

  /**
   * @brief Obtiene un material por nombre
   * @param nombre Nombre del material a obtener
   * @return Referencia constante al material
   * @throws std::runtime_error Si el material no existe
   */
  material const & escenario::obtener_material(std::string const & nombre) const {
    auto it = materiales.find(nombre);
    if (it == materiales.end()) {
      throw std::runtime_error("Material no encontrado: " + nombre);
    }
    return it->second;
  }

  /**
   * @brief Calcula la intersección más cercana entre un rayo y todos los objetos del escenario
   * @param r Rayo a intersectar
   * @param t_min Distancia mínima para considerar intersección válida
   * @param info Estructura con información de la intersección (salida)
   * @return true si hay al menos una intersección válida
   *
   * @brief Recorre todas las esferas y cilindros del escenario, encontrando la intersección
   * más cercana que esté dentro del rango válido [t_min, info.t]
   */
  bool escenario::intersectar(rayo const & r, double t_min, info_interseccion & info) const {
    bool hit         = false;
    double t_current = info.t;

    // Procesar intersecciones con esferas
    for (auto const & esfera : esferas) {
      double t_tmp = 0.0;
      vector p_tmp, n_tmp;
      if (esfera.intersectar(r, t_tmp, p_tmp, n_tmp) and t_tmp > t_min and t_tmp < t_current) {
        t_current   = t_tmp;
        info.t      = t_current;
        info.punto  = p_tmp;
        info.normal = n_tmp;
        info.mat    = &esfera.get_material();
        hit         = true;
      }
    }
    // Procesar intersecciones con cilindros
    for (auto const & cilindro : cilindros) {
      double t_tmp = 0.0;
      vector p_tmp, n_tmp;
      if (cilindro.intersectar(r, t_tmp, p_tmp, n_tmp) and t_tmp > t_min and t_tmp < t_current) {
        t_current   = t_tmp;
        info.t      = t_current;
        info.punto  = p_tmp;
        info.normal = n_tmp;
        info.mat    = &cilindro.get_material();
        hit         = true;
      }
    }

    return hit;
  }

}  // namespace render
