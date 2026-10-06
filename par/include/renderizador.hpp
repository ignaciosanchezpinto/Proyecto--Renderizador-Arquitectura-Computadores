#ifndef RENDER_RENDERIZADOR_HPP
#define RENDER_RENDERIZADOR_HPP

#include "imagen.hpp"
#include <camara.hpp>
#include <cmath>
#include <configuracion.hpp>
#include <cstddef>  // Necesario para std::size_t
#include <escenario.hpp>
#include <string>

namespace render {

  class renderizador {
  public:
    // Cambiamos 'int' por 'std::size_t' para evitar warnings de signo
    static void configurar(std::string const & tipo, std::size_t grano);

    static void renderizar(configuracion const & conf, camara & cam, escenario const & escena,
                           imagen & img);

  private:
    static std::string particionador;
    static std::size_t grano;  // Aquí estaba el problema, ahora es size_t
  };

}  // namespace render

#endif
