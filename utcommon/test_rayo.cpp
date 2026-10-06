#include <gtest/gtest.h>
#include <rayo.hpp>
#include <sstream>
#include <string>
#include <vector.hpp>

using namespace render;

TEST(RayoTest, ConstructorNormalizaDireccion) {
  vector const origen(0, 0, 0);
  vector const dir(3, 4, 0);  // longitud 5

  rayo const r(origen, dir);

  // La dirección debe ser normalizada
  EXPECT_NEAR(r.get_direccion().norm(), 1.0, 1e-8);
}

TEST(RayoTest, GettersFuncionanCorrectamente) {
  vector const origen(1, 2, 3);
  vector const dir(0, 0, 1);
  rayo const r(origen, dir);

  EXPECT_EQ(r.get_origen().get_x(), 1);
  EXPECT_EQ(r.get_origen().get_y(), 2);
  EXPECT_EQ(r.get_origen().get_z(), 3);

  EXPECT_NEAR(r.get_direccion().get_x(), 0, 1e-8);
  EXPECT_NEAR(r.get_direccion().get_y(), 0, 1e-8);
  EXPECT_NEAR(r.get_direccion().get_z(), 1, 1e-8);
}

TEST(RayoTest, PuntoCalculaCorrectamente) {
  vector const origen(0, 0, 0);
  vector const dir(1, 0, 0);  // dirección x
  rayo const r(origen, dir);

  vector const p = r.punto(5.0);
  EXPECT_NEAR(p.get_x(), 5.0, 1e-8);
  EXPECT_NEAR(p.get_y(), 0.0, 1e-8);
  EXPECT_NEAR(p.get_z(), 0.0, 1e-8);
}

TEST(RayoTest, AtEsAliasDePunto) {
  vector const origen(0, 1, 0);
  vector const dir(0, 0, 1);
  rayo const r(origen, dir);

  double const t  = 2.5;
  vector const p1 = r.punto(t);
  vector const p2 = r.at(t);

  EXPECT_NEAR(p1.get_x(), p2.get_x(), 1e-8);
  EXPECT_NEAR(p1.get_y(), p2.get_y(), 1e-8);
  EXPECT_NEAR(p1.get_z(), p2.get_z(), 1e-8);
}

TEST(RayoTest, DesplazadoCambiaSoloElOrigen) {
  vector const origen(0, 0, 0);
  vector const dir(0, 0, 1);
  vector const normal(0, 1, 0);
  double const eps = 1e-3;

  rayo const r(origen, dir);
  rayo const r2 = r.desplazado(normal, eps);

  // La dirección no debe cambiar
  EXPECT_NEAR(r2.get_direccion().get_x(), r.get_direccion().get_x(), 1e-8);
  EXPECT_NEAR(r2.get_direccion().get_y(), r.get_direccion().get_y(), 1e-8);
  EXPECT_NEAR(r2.get_direccion().get_z(), r.get_direccion().get_z(), 1e-8);

  // El origen debe moverse epsilon en la dirección de la normal
  EXPECT_NEAR(r2.get_origen().get_y(), eps, 1e-8);
}

TEST(RayoTest, OperadorSalidaImprimeCorrectamente) {
  vector const origen(1, 2, 3);
  vector const dir(0, 0, 1);
  rayo const r(origen, dir);

  std::ostringstream oss;
  oss << r;
  std::string const salida = oss.str();

  EXPECT_NE(salida.find("rayo(origen=("), std::string::npos);
  EXPECT_NE(salida.find("direccion=("), std::string::npos);
}
