#include "leer_escena.hpp"
#include <cstdio>
#include <escenario.hpp>
#include <fstream>
#include <gtest/gtest.h>
#include <string>

namespace render {

  namespace {

    void crear_archivo(char const * nombre, std::string const & contenido) {
      std::ofstream archivo(nombre);
      archivo << contenido;
    }

    void borrar_archivo(char const * nombre) {
      (void) std::remove(nombre);
    }

  }  // namespace

  /**
   * Caso válido completo
   */
  TEST(LeerEscenaTest, ArchivoValido) {
    char const * tmp = "tmp_escena_ok.txt";

    crear_archivo(tmp, "matte: rojo 0.8 0.0 0.0\n"
                       "metal: plata 0.9 0.9 0.9 0.7\n"
                       "refractive: vidrio 1.5\n"
                       "sphere: 0 0 0 1 rojo\n"
                       "cylinder: 1 0 0 0.5 0 1 0 plata\n");

    EXPECT_NO_THROW({ escenario const e = leer_escena::cargar_desde_archivo(tmp); });

    borrar_archivo(tmp);
  }

  /**
   * Archivo inexistente
   */
  TEST(LeerEscenaTest, ArchivoNoExiste) {
    EXPECT_DEATH({ leer_escena::cargar_desde_archivo("no_existe_123.txt"); }, ".*");
  }

  /**
   * Etiqueta no reconocida
   */
  TEST(LeerEscenaTest, EtiquetaDesconocida) {
    char const * tmp = "tmp_err1.txt";
    crear_archivo(tmp, "pelotita: x x x\n");

    EXPECT_DEATH({ leer_escena::cargar_desde_archivo(tmp); }, "Unknown scene entity");

    borrar_archivo(tmp);
  }

  /**
   * Material duplicado
   */
  TEST(LeerEscenaTest, MaterialDuplicado) {
    char const * tmp = "tmp_dup_mat.txt";
    crear_archivo(tmp, "matte: rojo 0.8 0.0 0.0\n"
                       "matte: rojo 0.1 0.1 0.1\n");

    EXPECT_DEATH({ leer_escena::cargar_desde_archivo(tmp); }, "already exists");

    borrar_archivo(tmp);
  }

  /**
   * Material inexistente en esfera
   */
  TEST(LeerEscenaTest, ObjetoMaterialInexistente) {
    char const * tmp = "tmp_err_mat.txt";
    crear_archivo(tmp, "sphere: 0 0 0 1 ninguno\n");

    EXPECT_DEATH({ leer_escena::cargar_desde_archivo(tmp); }, "Material not found");

    borrar_archivo(tmp);
  }

  /**
   * Color fuera de rango (material mate)
   */
  TEST(LeerEscenaTest, ColorFueraRango) {
    char const * tmp = "tmp_bad_color.txt";
    crear_archivo(tmp, "matte: rojo 2.0 -1.0 0.0\n");

    EXPECT_DEATH({ leer_escena::cargar_desde_archivo(tmp); }, "out of range");

    borrar_archivo(tmp);
  }

  /**
   * Parámetros insuficientes
   */
  TEST(LeerEscenaTest, ParametrosInsuficientesMate) {
    char const * tmp = "tmp_bad_matte.txt";
    crear_archivo(tmp, "matte: rojo 0.8 0.2\n");

    EXPECT_DEATH({ leer_escena::cargar_desde_archivo(tmp); }, "Invalid matte material parameters");

    borrar_archivo(tmp);
  }

  /**
   * Parámetros extra
   */
  TEST(LeerEscenaTest, ParametrosExtraEsfera) {
    char const * tmp = "tmp_extra_sphere.txt";
    crear_archivo(tmp, "matte: uno 0.2 0.2 0.2\n"
                       "sphere: 0 0 0 1 uno 3\n");

    EXPECT_DEATH({ leer_escena::cargar_desde_archivo(tmp); }, "Extra data");

    borrar_archivo(tmp);
  }

  /**
   * Radio de esfera inválido (<= 0)
   */
  TEST(LeerEscenaTest, EsferaRadioInvalido) {
    char const * tmp = "tmp_bad_radius.txt";
    crear_archivo(tmp, "matte: uno 0.1 0.1 0.1\n"
                       "sphere: 0 0 0 0 uno\n");

    EXPECT_DEATH({ leer_escena::cargar_desde_archivo(tmp); }, "Invalid sphere parameters");

    borrar_archivo(tmp);
  }

}  // namespace render
