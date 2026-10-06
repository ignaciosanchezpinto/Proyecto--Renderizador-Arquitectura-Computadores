#include <gtest/gtest.h>
#include <material.hpp>
#include <stdexcept>

using namespace render;

TEST(MaterialTest, MateValido) {
  EXPECT_NO_THROW(material const m(TipoMaterial::Mate, vector(0.5, 0.5, 0.5)));
}

TEST(MaterialTest, MateColorFueraDeRango) {
  EXPECT_THROW(material const m(TipoMaterial::Mate, vector(1.5, 0.0, 0.0)), std::invalid_argument);
}

TEST(MaterialTest, MetalValido) {
  EXPECT_NO_THROW(material const m(TipoMaterial::Metal, vector(0.3, 0.3, 0.3), 0.8));
}

TEST(MaterialTest, RefractivoValido) {
  EXPECT_NO_THROW(material const m(TipoMaterial::Refractivo, vector(1.0, 1.0, 1.0), 0.0, 1.5));
}

TEST(MaterialTest, MateColorLimiteInferiorSuperior) {
  EXPECT_NO_THROW(material const m(TipoMaterial::Mate, vector(0.0, 1.0, 0.0)));
}

TEST(MaterialTest, GettersCorrectos) {
  material const m(TipoMaterial::Metal, vector(0.2, 0.4, 0.6), 0.5);
  EXPECT_EQ(m.get_tipo(), TipoMaterial::Metal);
  EXPECT_EQ(m.get_color(), vector(0.2, 0.4, 0.6));
  EXPECT_DOUBLE_EQ(m.get_refl(), 0.5);
  EXPECT_DOUBLE_EQ(m.get_refrac(), 1.0);  // default para metal
}
