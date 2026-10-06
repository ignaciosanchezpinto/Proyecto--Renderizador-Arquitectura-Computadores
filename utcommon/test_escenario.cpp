#include <escenario.hpp>
#include <esfera.hpp>
#include <gtest/gtest.h>
#include <material.hpp>
#include <rayo.hpp>
#include <stdexcept>
#include <vector.hpp>

using namespace render;

TEST(EscenarioTest, AgregarYComprobarMaterial) {
  escenario e;
  material const rojo(TipoMaterial::Mate, vector(1, 0, 0), 0.5);
  e.agregar_material("rojo", rojo);

  EXPECT_TRUE(e.tiene_material("rojo"));
  EXPECT_FALSE(e.tiene_material("azul"));
}

TEST(EscenarioTest, ObtenerMaterialExistente) {
  escenario e;
  material const metal(TipoMaterial::Metal, vector(0.7, 0.7, 0.7), 0.9);
  e.agregar_material("metal", metal);

  auto const & mat = e.obtener_material("metal");
  EXPECT_NEAR(mat.get_refl(), 0.9, 1e-8);
}

TEST(EscenarioTest, ObtenerMaterialInexistenteLanzaExcepcion) {
  escenario const e;
  EXPECT_THROW(
      {
        auto const & ignored = e.obtener_material("fantasma");
        (void) ignored;  // evita warning [[nodiscard]]
      },
      std::runtime_error);
}

TEST(EscenarioTest, AgregarEsferaYComprobarInterseccion) {
  escenario e;
  material const rojo(TipoMaterial::Mate, vector(1, 0, 0), 0.5);
  e.agregar_material("rojo", rojo);
  esfera const s(vector(0, 0, 0), 1.0, rojo);
  e.agregar_esfera(s);

  rayo const r(vector(0, 0, -5), vector(0, 0, 1));
  info_interseccion info;

  bool const choca = e.intersectar(r, 0.001, info);

  EXPECT_TRUE(choca);
  EXPECT_NEAR(info.punto.get_z(), -1.0, 1e-2);
  EXPECT_NEAR(info.normal.norm(), 1.0, 1e-8);

  // Comprobamos color
  vector const & c = info.mat->get_color();
  EXPECT_NEAR(c.get_x(), 1.0, 1e-8);
  EXPECT_NEAR(c.get_y(), 0.0, 1e-8);
  EXPECT_NEAR(c.get_z(), 0.0, 1e-8);
}

TEST(EscenarioTest, InterseccionNulaSiNoHayObjetos) {
  escenario const e;
  rayo const r(vector(0, 0, 0), vector(1, 0, 0));
  info_interseccion info;

  bool const choca = e.intersectar(r, 0.001, info);
  EXPECT_FALSE(choca);
}
