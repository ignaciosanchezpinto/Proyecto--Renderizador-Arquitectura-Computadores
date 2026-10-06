#include <esfera.hpp>
#include <gtest/gtest.h>
#include <material.hpp>
#include <rayo.hpp>
#include <stdexcept>
#include <vector.hpp>

using namespace render;

TEST(EsferaTest, Valida) {
  material const m(TipoMaterial::Mate, vector(0.5, 0.5, 0.5));
  EXPECT_NO_THROW(esfera const s(vector(0, 0, 0), 1.0, m));
}

TEST(EsferaTest, RadioNegativoLanzaExcepcion) {
  material const m(TipoMaterial::Mate, vector(0.5, 0.5, 0.5));
  esfera const s(vector(0, 0, 0), -1.0, m);
  EXPECT_THROW(s.validar_esf(), std::invalid_argument);
}

TEST(EsferaTest, RadioCeroLanzaExcepcion) {
  material const m(TipoMaterial::Mate, vector(0.5, 0.5, 0.5));
  esfera const s(vector(0, 0, 0), 0.0, m);
  EXPECT_THROW(s.validar_esf(), std::invalid_argument);
}

TEST(EsferaTest, GettersCorrectos) {
  material const m(TipoMaterial::Mate, vector(0.2, 0.3, 0.4));
  vector const centro(1.0, 2.0, 3.0);
  double const radio = 5.0;

  esfera const s(centro, radio, m);

  EXPECT_EQ(s.get_material().get_tipo(), m.get_tipo());
  EXPECT_EQ(s.get_material().get_color(), m.get_color());
  EXPECT_DOUBLE_EQ(s.get_material().get_refl(), m.get_refl());
  EXPECT_DOUBLE_EQ(s.get_material().get_refrac(), m.get_refrac());
}

TEST(EsferaTest, RayoNoIntersecta) {
  material const m(TipoMaterial::Mate, vector(0.5, 0.5, 0.5));
  esfera const s(vector(0, 0, 0), 1.0, m);

  rayo const ray(vector(0, 0, 5), vector(0, 1, 0));
  double t = 0.0;
  vector p, n;

  EXPECT_FALSE(s.intersectar(ray, t, p, n));
}

TEST(EsferaTest, RayoAtraviesaCentro) {
  material const m(TipoMaterial::Mate, vector(0.5, 0.5, 0.5));
  esfera const s(vector(0, 0, 0), 1.0, m);

  rayo const ray(vector(0, 0, -3), vector(0, 0, 1));
  double t = 0.0;
  vector p, n;

  EXPECT_TRUE(s.intersectar(ray, t, p, n));
  EXPECT_GT(t, 0.0);
}
