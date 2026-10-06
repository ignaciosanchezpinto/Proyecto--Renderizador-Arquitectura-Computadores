#ifndef RENDER_LEER_ESCENA_HPP
#define RENDER_LEER_ESCENA_HPP

#include <escenario.hpp>
#include <string>

namespace render {

  /**
   * @brief Clase responsable de cargar una escena desde un archivo de texto
   */
  class leer_escena {
  public:
    /**
     * @brief Método que crea un escenario a partir de un archivo
     * @param nombre_archivo Nombre o ruta del archivo de escena
     * @return Objeto escenario con todos los materiales y objetos cargados
     * @throws Finaliza el programa si el archivo no puede abrirse o el formato es incorrecto
     */
    static escenario cargar_desde_archivo(std::string const & nombre_archivo);
  };  // namespace render

}  // namespace render

#endif
