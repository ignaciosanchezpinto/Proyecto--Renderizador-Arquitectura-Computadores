#ifndef RENDER_ESCENARIO_HPP
#define RENDER_ESCENARIO_HPP

#include <cilindro.hpp>
#include <esfera.hpp>
#include <map>
#include <material.hpp>
#include <string>
#include <vector>

namespace render {

  // Estructura que almacena información sobre el choque de un rayo con un objeto
  struct info_interseccion {
    vector punto;                    // Punto de intersección
    vector normal;                   // Normal en el punto de intersección
    material const * mat = nullptr;  // Puntero al material del objeto intersectado
    double t             = 1e9;      // Distancia desde el origen del rayo a la intersección
  };

  class escenario {
  public:
    /**
     * @brief Método que añade un material con el nombre que lo identifica
     * @param nombre nombre del material que se va a añadir
     * @param m tipo de material que se va a añadir
     */
    void agregar_material(std::string const & nombre, material const & m);

    /**
     * @brief Método que añade esfera al escenario
     * @param e esfera que se añade
     */
    void agregar_esfera(esfera const & e);

    /**
     * @brief Método que añade cilindro al escenario
     * @param c cilindro que se añade
     */
    void agregar_cilindro(cilindro const & c);

    /**
     * @brief Método que comprueba si un material existe
     * @param nombre Nombre del material
     * @return True si el nombre del material existe, False en caso contraio
     */
    [[nodiscard]] bool tiene_material(std::string const & nombre) const;

    /**
     * @brief Método que devuelve un material por su nombre
     * @param nombre Nombre del material
     * @return Material en caso de encontrar, en otro caso "Material no encontrado" + nombre
     */
    [[nodiscard]] material const & obtener_material(std::string const & nombre) const;

    /**
     * @brief Método que comprueba si un rayo choca con algún objeto
     * @param r rayo que interseccionará
     * @param t_min distancia mínima para la intersección
     * @param info guarda la información de la intersección
     * @return True si el rayo intersecta con algún objeto, False en caso contrario
     */
    [[nodiscard]] bool intersectar(rayo const & r, double t_min, info_interseccion & info) const;

  private:
    // Almacenamiento privado de escenario
    std::map<std::string, material>
        materiales;                   // Diccionario que encuentra por el nombre de un material
    std::vector<esfera> esferas;      // Lista de todas las esferas
    std::vector<cilindro> cilindros;  // Lista de todos los cilindros
  };

}  // namespace render

#endif
