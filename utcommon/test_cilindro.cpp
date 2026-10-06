#include <cilindro.hpp>
#include <gtest/gtest.h>
#include <material.hpp>
#include <rayo.hpp>
#include <stdexcept>

using namespace render;

TEST(CilindroTest, Valido) {
  material const m(TipoMaterial::Metal, vector(0.3, 0.3, 0.3), 0.5);
  EXPECT_NO_THROW(cilindro const c(vector(0, 0, 0), 1.0, m, vector(0, 1, 0)));
}

TEST(CilindroTest, RadioNegativo) {
  material const m(TipoMaterial::Metal, vector(0.3, 0.3, 0.3), 0.5);
  cilindro const c(vector(0, 0, 0), -1.0, m, vector(0, 1, 0));
  EXPECT_THROW(c.validar(), std::invalid_argument);
}

TEST(CilindroTest, EjeNulo) {
  material const m(TipoMaterial::Metal, vector(0.3, 0.3, 0.3), 0.5);
  cilindro const c(vector(0, 0, 0), 1.0, m, vector(0, 0, 0));
  EXPECT_THROW(c.validar(), std::invalid_argument);
}

TEST(CilindroTest, Intersectar_superficie) {
  vector v = {0, 0, 0};
  vector p = {1, 1, 1};
  material const m(TipoMaterial::Metal, vector(0.3, 0.3, 0.3), 0.5);
  rayo const r(v, p);
  double num = 8.0;
  cilindro const c(v, 1.0, m, p);

  bool const resultado = c.intersectar(r, num, v, p);
  EXPECT_TRUE(resultado);
}

TEST(CilindroTest, GettersCorrectos) {
  vector const v = {0, 0, 0};
  vector const p = {1, 1, 1};
  material const m(TipoMaterial::Metal, vector(0.3, 0.3, 0.3), 0.5);
  cilindro const c(v, 8.0, m, p);

  EXPECT_EQ(c.get_material().get_tipo(), m.get_tipo());
  EXPECT_EQ(c.get_material().get_color(), m.get_color());
  EXPECT_DOUBLE_EQ(c.get_material().get_refl(), m.get_refl());
  EXPECT_DOUBLE_EQ(c.get_material().get_refrac(), m.get_refrac());
}
