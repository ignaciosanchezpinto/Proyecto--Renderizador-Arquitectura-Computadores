#include "configuracion.hpp"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <gtest/gtest.h>
#include <ios>

using namespace render;

TEST(ConfiguracionTest, CargaValida) {
  char const * tmp_file = "tmp_config.txt";
  {
    std::ofstream a(tmp_file);
    a << "image_width: 1200\n";
    a << "gamma: 2.2\n";
    a << "camera_position: 13 2 3\n";
    a << "camera_target: 0 0 0\n";
    a << "camera_north: 0 1 0\n";
    a << "field_of_view: 20\n";
    a << "samples_per_pixel: 10\n";
    a << "max_depth: 5\n";
    a << "material_rng_seed: 45\n";
    a << "ray_rng_seed: 133\n";
    a << "background_dark_color: 0.25 0.5 1\n";
    a << "background_light_color: 1 1 1\n";
  }

  configuracion const conf = configuracion::validar_configuracion(tmp_file);

  EXPECT_EQ(conf.get_image_width(), 1'200);
  EXPECT_DOUBLE_EQ(conf.get_gamma(), 2.2);
  EXPECT_DOUBLE_EQ(conf.get_camera_position().get_x(), 13.0);
  EXPECT_DOUBLE_EQ(conf.get_field_of_view(), 20.0);
  EXPECT_EQ(conf.get_samples_per_pixel(), 10);
  EXPECT_EQ(conf.get_max_depth(), 5);
  EXPECT_EQ(conf.get_material_rng_seed(), 45);
  EXPECT_EQ(conf.get_ray_rng_seed(), 133);
  EXPECT_DOUBLE_EQ(conf.get_background_dark_color().get_x(), 0.25);
  EXPECT_DOUBLE_EQ(conf.get_background_light_color().get_z(), 1.0);

  (void) std::remove(tmp_file);
}

TEST(ConfiguracionTest, ValoresPorDefecto) {
  char const * tmp_file = "tmp_empty.txt";
  {
    std::ofstream out(tmp_file, std::ios::trunc);
    out.close();
    (void) out;  // evita unused-variable warnings
  }

  configuracion const conf = configuracion::validar_configuracion(tmp_file);

  EXPECT_EQ(conf.get_aspect_ratio().at(0), 16);
  EXPECT_EQ(conf.get_aspect_ratio().at(1), 9);
  EXPECT_EQ(conf.get_image_width(), 1'920);
  EXPECT_DOUBLE_EQ(conf.get_gamma(), 2.2);
  EXPECT_DOUBLE_EQ(conf.get_field_of_view(), 90.0);
  EXPECT_EQ(conf.get_samples_per_pixel(), 20);
  EXPECT_EQ(conf.get_max_depth(), 5);
  EXPECT_EQ(conf.get_material_rng_seed(), 13);
  EXPECT_EQ(conf.get_ray_rng_seed(), 19);
  EXPECT_DOUBLE_EQ(conf.get_background_dark_color().get_y(), 0.5);
  EXPECT_DOUBLE_EQ(conf.get_background_light_color().get_x(), 1.0);

  (void) std::remove(tmp_file);
}

TEST(ConfiguracionTest, ParametrosRepetidos) {
  char const * tmp_file = "tmp_repeat.txt";
  {
    std::ofstream a(tmp_file);
    a << "gamma: 1.8\n";
    a << "gamma: 2.5\n";  // debe prevalecer este
  }

  configuracion const conf = configuracion::validar_configuracion(tmp_file);
  EXPECT_DOUBLE_EQ(conf.get_gamma(), 2.5);

  (void) std::remove(tmp_file);
}

TEST(ConfiguracionTest, EtiquetaInvalida) {
  char const * tmp_file = "tmp_badkey.txt";
  {
    std::ofstream a(tmp_file);
    a << "image_xwidth: 1200\n";  // incorrecta
  }

  EXPECT_EXIT((void) configuracion::validar_configuracion(tmp_file),
              ::testing::ExitedWithCode(EXIT_FAILURE), "Unknown configuration key");

  (void) std::remove(tmp_file);
}

TEST(ConfiguracionTest, DatosInsuficientes) {
  char const * tmp_file = "tmp_missing.txt";
  {
    std::ofstream a(tmp_file);
    a << "camera_position: 500 500\n";  // faltan datos
  }

  EXPECT_EXIT((void) configuracion::validar_configuracion(tmp_file),
              ::testing::ExitedWithCode(EXIT_FAILURE), "Invalid value");

  (void) std::remove(tmp_file);
}

TEST(ConfiguracionTest, DatosExtra) {
  char const * tmp_file = "tmp_extra.txt";
  {
    std::ofstream a(tmp_file);
    a << "gamma: 2.1 2.2 99\n";
  }

  EXPECT_EXIT((void) configuracion::validar_configuracion(tmp_file),
              ::testing::ExitedWithCode(EXIT_FAILURE), "Extra data");

  (void) std::remove(tmp_file);
}
