#include <array>
#include <cctype>
#include <configuracion.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector.hpp>

namespace render {
  namespace {

    /// @brief Muestra un error cuando la etiqueta de configuración no se reconoce
    /// @param etiqueta_config Etiqueta que causó el error
    /// @throws Termina el programa con std::exit(EXIT_FAILURE)
    void error_etiqueta_no_reconocida(std::string const & etiqueta_config) {
      std::cerr << "Error: Unknown configuration key: [" << etiqueta_config << "]\n";
      std::exit(EXIT_FAILURE);
    }

    /// @brief Muestra un error cuando la estructura de la línea de configuración es inválida
    /// @param etiqueta_config Etiqueta asociada a la línea
    /// @param linea_config Línea completa con el error
    /// @throws Termina el programa con std::exit(EXIT_FAILURE)
    void error_estructura_lineas(std::string const & etiqueta_config,
                                 std::string const & linea_config) {
      std::cerr << "Error: Invalid value for key :[" << etiqueta_config << "]\nLine: '"
                << linea_config << "'\n";
      std::exit(EXIT_FAILURE);
    }

    /// @brief Muestra un error cuando hay información extra tras un valor válido
    /// @param etiqueta_config Etiqueta de la configuración
    /// @param extra Texto adicional encontrado
    /// @throws Termina el programa con std::exit(EXIT_FAILURE)
    void error_info_no_esperada(std::string const & etiqueta_config, std::string const & extra) {
      std::cerr << "Error: Extra data after configuration value for key :[" << etiqueta_config
                << "]\nExtra: '" << extra << "'\n";
      std::exit(EXIT_FAILURE);
    }

    /// @brief Muestra un error si no se puede abrir el archivo de configuración
    /// @param nombre_archivo Nombre del archivo que no pudo abrirse
    /// @throws Termina el programa con std::exit(EXIT_FAILURE)
    void error_archivo(std::string const & nombre_archivo) {
      std::cerr << "Error al abrir el archivo: " << nombre_archivo << "\n";
      std::exit(EXIT_FAILURE);
    }

    /// @brief Elimina espacios en blanco al inicio y final de una línea
    /// @param linea Línea a procesar
    /// @return Línea sin espacios en los extremos
    std::string tokenizar_linea(std::string const & linea) {
      size_t const inicio = linea.find_first_not_of(" \t\r\n");
      if (inicio == std::string::npos) {
        return "";
      }
      size_t const fin = linea.find_last_not_of(" \t\r\n");
      return linea.substr(inicio, fin - inicio + 1);
    }

    /// @brief Valida y convierte una línea en una relación de aspecto
    /// @param etiqueta_config Nombre de la etiqueta
    /// @param resto_linea Contenido tras la etiqueta
    /// @param linea_config Línea original completa
    /// @return Relación de aspecto como array [ancho, alto]
    /// @throws Termina el programa si los valores son inválidos o negativos
    std::array<int, 2> validar_aspect_ratio(std::string const & etiqueta_config,
                                            std::string const & resto_linea,
                                            std::string const & linea_config) {
      std::istringstream entrada(resto_linea);
      int anchura = 0;
      int altura  = 0;
      if (!(entrada >> anchura >> altura)) {
        error_estructura_lineas(etiqueta_config, linea_config);
      }
      std::string extra;
      entrada >> std::ws;  
      if (std::getline(entrada, extra) and !extra.empty()) {
        error_info_no_esperada(etiqueta_config, extra);
      }
      if (anchura <= 0 or altura <= 0) {
        error_estructura_lineas(etiqueta_config, linea_config);
      }
      return {anchura, altura};
    }

    /// @brief Valida el valor gamma
    /// @throws Termina el programa si el valor no es numérico o si hay información extra
    double validar_gamma(std::string const & etiqueta_config, std::string const & resto_linea,
                         std::string const & linea_config) {
      std::istringstream entrada(resto_linea);
      double valor = 0.0;
      if (!(entrada >> valor)) {
        error_estructura_lineas(etiqueta_config, linea_config);
      }
      std::string extra;
      entrada >> std::ws;  
      if (std::getline(entrada, extra) and !extra.empty()) {
        error_info_no_esperada(etiqueta_config, extra);
      }
      return valor;
    }

    /// @brief Valida una línea que define una posición o vector de cámara (x, y, z)
    /// @throws Termina el programa si el formato o cantidad de datos es incorrecta
    vector validar_camara(std::string const & etiqueta_config, std::string const & resto_linea,
                          std::string const & linea_config) {
      std::istringstream entrada(resto_linea);
      double x = 0.0;
      double y = 0.0;
      double z = 0.0;
      if (!(entrada >> x >> y >> z)) {
        error_estructura_lineas(etiqueta_config, linea_config);
      }
      std::string extra;
      entrada >> std::ws;  
      if (std::getline(entrada, extra) and !extra.empty()) {
        error_info_no_esperada(etiqueta_config, extra);
      }
      return vector{x, y, z};
    }

    /// @brief Valida un valor numérico tipo double
    /// @throws Termina el programa si el número no es válido o está fuera de rango
    double validar_double(std::string const & etiqueta_config, std::string const & resto_linea,
                          std::string const & linea_config) {
      std::istringstream entrada(resto_linea);
      double valor = 0.0;
      if (!(entrada >> valor)) {
        error_estructura_lineas(etiqueta_config, linea_config);
      }
      std::string extra;
      entrada >> std::ws;  
      if (std::getline(entrada, extra) and !extra.empty()) {
        error_info_no_esperada(etiqueta_config, extra);
      }
      if (valor <= 0.0 or valor >= 180.0) {
        error_estructura_lineas(etiqueta_config, linea_config);
      }
      return valor;
    }

    /// @brief Valida un valor entero positivo
    /// @throws Termina el programa si no es entero o es menor o igual a 0
    int validar_enteros(std::string const & etiqueta_config, std::string const & resto_linea,
                        std::string const & linea_config) {
      std::istringstream entrada(resto_linea);
      int valor = 0;
      if (!(entrada >> valor)) {
        error_estructura_lineas(etiqueta_config, linea_config);
      }
      std::string extra;
      entrada >> std::ws;  
      if (std::getline(entrada, extra) and !extra.empty()) {
        error_info_no_esperada(etiqueta_config, extra);
      }
      if (valor <= 0) {
        error_estructura_lineas(etiqueta_config, linea_config);
      }
      return valor;
    }

    /// @brief Valida un color de fondo (RGB) con componentes entre 0 y 1
    /// @throws Termina el programa si las componentes están fuera de rango o el formato es inválido
    vector validar_background_color(std::string const & etiqueta_config,
                                    std::string const & resto_linea,
                                    std::string const & linea_config) {
      std::istringstream entrada(resto_linea);
      double x = 0.0;
      double y = 0.0;
      double z = 0.0;
      if (!(entrada >> x >> y >> z)) {
        error_estructura_lineas(etiqueta_config, linea_config);
      }
      std::string extra;
      entrada >> std::ws;  
      if (std::getline(entrada, extra) and !extra.empty()) {
        error_info_no_esperada(etiqueta_config, extra);
      }
      if (x < 0.0 or x > 1.0 or y < 0.0 or y > 1.0 or z < 0.0 or z > 1.0) {
        error_estructura_lineas(etiqueta_config, linea_config);
      }
      return vector{x, y, z};
    }

    /// @brief Determina el tipo de dato asociado a una etiqueta de configuración y devuelve su valor
    /// @return Tupla con los posibles valores leídos (entero, vector, double, aspect_ratio, etiqueta)
    std::tuple<int, vector, double, std::array<int, 2>, std::string> clasificador(
        std::string const & etiqueta_config, std::string const & resto_linea,
        std::string const & linea_config) {
      int valor_entero = 0;
      vector valor_vector;
      double valor_double                   = 0.0;
      std::array<int, 2> valor_aspect_ratio = {0, 0};
      if (etiqueta_config == "aspect_ratio:") {
        valor_aspect_ratio = validar_aspect_ratio(etiqueta_config, resto_linea, linea_config);
      } else if (etiqueta_config == "image_width:" or
                 etiqueta_config == "samples_per_pixel:" or
                 etiqueta_config == "max_depth:" or
                 etiqueta_config == "material_rng_seed:" or
                 etiqueta_config == "ray_rng_seed:")
      {
        valor_entero = validar_enteros(etiqueta_config, resto_linea, linea_config);
      } else if (etiqueta_config == "gamma:") {
        valor_double = validar_gamma(etiqueta_config, resto_linea, linea_config);
      } else if (etiqueta_config == "camera_position:" or
                 etiqueta_config == "camera_target:" or
                 etiqueta_config == "camera_north:")
      {
        valor_vector = validar_camara(etiqueta_config, resto_linea, linea_config);
      } else if (etiqueta_config == "field_of_view:") {
        valor_double = validar_double(etiqueta_config, resto_linea, linea_config);
      } else if (etiqueta_config == "background_dark_color:" or
                 etiqueta_config == "background_light_color:")
      {
        valor_vector = validar_background_color(etiqueta_config, resto_linea, linea_config);
      } else {
        error_etiqueta_no_reconocida(etiqueta_config);
      }
      return {valor_entero, valor_vector, valor_double, valor_aspect_ratio, etiqueta_config};
    }

    /// @brief Asigna el valor leído al campo correspondiente de la configuración
    /// @param cfg Objeto de configuración que se modificará
    /// @param etiqueta_config Etiqueta de configuración actual
    /// @param resto_linea Contenido de la línea tras la etiqueta
    /// @param linea_config Línea completa leída del archivo
    void rellenar_objeto(configuracion & cfg, std::string const & etiqueta_config,
                         std::string const & resto_linea, std::string const & linea_config) {
      int valor_entero = 0;
      vector valor_vector;
      double valor_double                   = 0.0;
      std::array<int, 2> valor_aspect_ratio = {0, 0};
      std::string etiqueta;
      std::tie(valor_entero, valor_vector, valor_double, valor_aspect_ratio, etiqueta) =
          clasificador(etiqueta_config, resto_linea, linea_config);
      if (etiqueta == "aspect_ratio:") {
        cfg.set_aspect_ratio(valor_aspect_ratio);
      } else if (etiqueta == "image_width:") {
        cfg.set_image_width(valor_entero);
      } else if (etiqueta == "samples_per_pixel:") {
        cfg.set_samples_per_pixel(valor_entero);
      } else if (etiqueta == "max_depth:") {
        cfg.set_max_depth(valor_entero);
      } else if (etiqueta == "material_rng_seed:") {
        cfg.set_material_rng_seed(valor_entero);
      } else if (etiqueta == "ray_rng_seed:") {
        cfg.set_ray_rng_seed(valor_entero);
      } else if (etiqueta == "gamma:") {
        cfg.set_gamma(valor_double);
      } else if (etiqueta == "field_of_view:") {
        cfg.set_field_of_view(valor_double);
      } else if (etiqueta == "camera_position:") {
        cfg.set_camera_position(valor_vector);
      } else if (etiqueta == "camera_target:") {
        cfg.set_camera_target(valor_vector);
      } else if (etiqueta == "camera_north:") {
        cfg.set_camera_north(valor_vector);
      } else if (etiqueta == "background_dark_color:") {
        cfg.set_background_dark_color(valor_vector);
      } else if (etiqueta == "background_light_color:") {
        cfg.set_background_light_color(valor_vector);
      }
    }

  }  // namespace

  /// @brief Carga un archivo de configuración, valida cada línea y devuelve un objeto configurado
  /// @param nombre_archivo Ruta del archivo de configuración
  /// @return Objeto configuracion con los parámetros cargados
  /// @throws Finaliza el programa si hay errores de formato o el archivo no puede abrirse
  configuracion configuracion::validar_configuracion(std::string const & nombre_archivo) {
    std::ifstream archivo_conf(nombre_archivo);
    if (!archivo_conf.is_open()) {
      error_archivo(nombre_archivo);  
    }
    std::string linea_config;
    configuracion cfg{};  
    while (std::getline(archivo_conf, linea_config)) {
      std::string linea = tokenizar_linea(linea_config);
      if (!linea.empty()) {  
        std::string etiqueta_config;
        std::string resto_linea;
        size_t const espacio = linea.find(' ');  
        if (espacio != std::string::npos) {
          etiqueta_config = linea.substr(0, espacio);
          resto_linea = linea.substr(espacio + 1);  
          char const dos_puntos = linea[espacio - 1];
          if (dos_puntos != ':') {
            error_etiqueta_no_reconocida(etiqueta_config);
          }
        } else {
          etiqueta_config = linea;
          resto_linea     = "";
        }
        rellenar_objeto(cfg, etiqueta_config, resto_linea, linea_config);
      }
    }
    return cfg;
  }

}  // namespace render
