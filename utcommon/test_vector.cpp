#include <gtest/gtest.h>
#include <vector.hpp>

TEST(test_vector, norm_zero) {
  render::vector const vec{0.0, 0.0, 0.0};
  EXPECT_EQ(vec.norm(), 0.0);
}

TEST(test_vector, norm_positive) {
  render::vector const vec{3.0, 4.0, 0.0};
  EXPECT_EQ(vec.norm(), 5.0);
}

TEST(test_vector, suma_vectores) {
  render::vector const v1{1.0, 2.0, 3.0};
  render::vector const v2{4.0, -2.0, 1.0};
  render::vector const resultado = v1 + v2;
  EXPECT_EQ(resultado.get_x(), 5.0);
  EXPECT_EQ(resultado.get_y(), 0.0);
  EXPECT_EQ(resultado.get_z(), 4.0);
}

TEST(test_vector, resta_vectores) {
  render::vector const v1{5.0, 3.0, 1.0};
  render::vector const v2{2.0, 1.0, 4.0};
  render::vector const resultado = v1 - v2;
  EXPECT_EQ(resultado.get_x(), 3.0);
  EXPECT_EQ(resultado.get_y(), 2.0);
  EXPECT_EQ(resultado.get_z(), -3.0);
}

TEST(test_vector, multiplicacion_por_escalar) {
  render::vector const v{1.0, -2.0, 3.0};
  render::vector const resultado = v * 2.0;
  EXPECT_EQ(resultado.get_x(), 2.0);
  EXPECT_EQ(resultado.get_y(), -4.0);
  EXPECT_EQ(resultado.get_z(), 6.0);
}

TEST(test_vector, division_por_escalar) {
  render::vector const v{4.0, 8.0, 12.0};
  render::vector const resultado = v / 4.0;
  EXPECT_EQ(resultado.get_x(), 1.0);
  EXPECT_EQ(resultado.get_y(), 2.0);
  EXPECT_EQ(resultado.get_z(), 3.0);
}

TEST(test_vector, producto_punto) {
  render::vector const v1{1.0, 2.0, 3.0};
  render::vector const v2{4.0, -5.0, 6.0};
  EXPECT_EQ(v1.dot(v2), 12.0);
}

TEST(test_vector, producto_cruzado) {
  render::vector const v1{1.0, 0.0, 0.0};
  render::vector const v2{0.0, 1.0, 0.0};
  render::vector const resultado = v1.cross(v2);
  EXPECT_EQ(resultado.get_x(), 0.0);
  EXPECT_EQ(resultado.get_y(), 0.0);
  EXPECT_EQ(resultado.get_z(), 1.0);
}

TEST(test_vector, vector_normalizado_unitario) {
  render::vector const v{3.0, 0.0, 4.0};
  render::vector const norm = v.normalized();
  EXPECT_NEAR(norm.norm(), 1.0, 1e-9);
}

TEST(test_vector, vector_normalizado_cero) {
  render::vector const v{0.0, 0.0, 0.0};
  render::vector const norm = v.normalized();
  EXPECT_EQ(norm.get_x(), 0.0);
  EXPECT_EQ(norm.get_y(), 0.0);
  EXPECT_EQ(norm.get_z(), 0.0);
}

TEST(test_vector, operador_unario_negativo) {
  render::vector const v{1.0, -2.0, 3.0};
  render::vector const neg = -v;
  EXPECT_EQ(neg.get_x(), -1.0);
  EXPECT_EQ(neg.get_y(), 2.0);
  EXPECT_EQ(neg.get_z(), -3.0);
}

TEST(test_vector, operador_multiplicacion_vectorial) {
  render::vector const v1{1.0, 2.0, 3.0};
  render::vector const v2{2.0, 3.0, 4.0};
  render::vector const resultado = v1 * v2;
  EXPECT_EQ(resultado.get_x(), 2.0);
  EXPECT_EQ(resultado.get_y(), 6.0);
  EXPECT_EQ(resultado.get_z(), 12.0);
}
