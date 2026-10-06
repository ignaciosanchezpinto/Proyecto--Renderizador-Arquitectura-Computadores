#include <camara.hpp>
#include <configuracion.hpp>
#include <gtest/gtest.h>
#include <random>
#include <rayo.hpp>
#include <vector.hpp>

using namespace render;

TEST(CamaraTest, ConstructorValido) {
  configuracion conf;
  conf.set_camera_position({0, 0, 5});
  conf.set_camera_target({0, 0, 0});
  conf.set_camera_north({0, 1, 0});
  conf.set_field_of_view(90.0);
  conf.set_image_width(800);  // altura se calcula automáticamente

  EXPECT_NO_THROW(camara const cam(conf));
}

TEST(CamaraTest, RayoNormalizado) {
  configuracion conf;
  conf.set_camera_position({0, 0, 5});
  conf.set_camera_target({0, 0, 0});
  conf.set_camera_north({0, 1, 0});
  conf.set_field_of_view(90.0);
  conf.set_image_width(800);

  camara cam(conf);
  std::mt19937_64 rng(42);

  rayo const r = cam.generar_rayo(300, 400, rng);
  EXPECT_NEAR(r.get_direccion().norm(), 1.0, 1e-8);
}

TEST(CamaraTest, RayosEsquinasNormalizados) {
  configuracion conf;
  conf.set_camera_position({0, 0, 5});
  conf.set_camera_target({0, 0, 0});
  conf.set_camera_north({0, 1, 0});
  conf.set_field_of_view(90.0);
  conf.set_image_width(800);

  camara cam(conf);
  std::mt19937_64 rng(123);

  int const alto  = conf.get_image_height();
  int const ancho = conf.get_image_width();

  rayo const r00 = cam.generar_rayo(0, 0, rng);
  rayo const r0W = cam.generar_rayo(0, ancho - 1, rng);
  rayo const rH0 = cam.generar_rayo(alto - 1, 0, rng);
  rayo const rHW = cam.generar_rayo(alto - 1, ancho - 1, rng);

  EXPECT_NEAR(r00.get_direccion().norm(), 1.0, 1e-8);
  EXPECT_NEAR(r0W.get_direccion().norm(), 1.0, 1e-8);
  EXPECT_NEAR(rH0.get_direccion().norm(), 1.0, 1e-8);
  EXPECT_NEAR(rHW.get_direccion().norm(), 1.0, 1e-8);
}
