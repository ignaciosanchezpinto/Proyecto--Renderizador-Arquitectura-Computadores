#include <cctype>
#include <cilindro.hpp>
#include <cstdlib>
#include <escenario.hpp>
#include <esfera.hpp>
#include <fstream>
#include <iostream>
#include <leer_escena.hpp>
#include <material.hpp>
#include <sstream>
#include <string>
#include <vector.hpp>

namespace render {
  namespace {

    /**
     * @brief Elimina espacios en blanco al inicio y final de una cadena
     * @param linea Cadena a recortar
     * @return Cadena recortada sin espacios en los extremos
     */
    std::string recortar(std::string const & linea) {
      size_t const inicio = linea.find_first_not_of(" \t\r\n");
      if (inicio == std::string::npos) {
        return "";
      }
      size_t const fin = linea.find_last_not_of(" \t\r\n");
      return linea.substr(inicio, fin - inicio + 1);
    }

    /**
     * @brief Valida que no haya datos extra después de los parámetros esperados
     * @param iss Stream de entrada para verificar
     * @param tipo Tipo de entidad que se está procesando
     * @param linea_bruta Línea original para mensajes de error
     * @return true si la línea es válida (sin datos extra)
     * @brief Termina el programa si encuentra datos extra
     */
    bool validar_linea(std::istringstream & iss, std::string const & tipo,
                       std::string const & linea_bruta) {
      std::string extra;
      if (iss >> extra) {
        std::cerr << "Error: Extra data after configuration value for key: [" << tipo << ":]\n"
                  << "Extra: \"" << extra << "\"\n"
                  << "Line: \"" << linea_bruta << "\"\n";
        std::exit(EXIT_FAILURE);
      }
      return true;
    }

    /**
     * @brief Verifica si un material con el mismo nombre ya existe
     * @param escena Escenario donde verificar
     * @param nombre Nombre del material a verificar
     * @param linea_bruta Línea original para mensajes de error
     * @brief Termina el programa si el material ya existe
     */
    void verificar_material_duplicado(escenario const & escena, std::string const & nombre,
                                      std::string const & linea_bruta) {
      if (escena.tiene_material(nombre)) {
        std::cerr << "Error: Material with name [" << nombre << "] already exists\n"
                  << "Line: \"" << linea_bruta << "\"\n";
        std::exit(EXIT_FAILURE);
      }
    }

    /**
     * @brief Verifica si un material existe en el escenario
     * @param escena Escenario donde verificar
     * @param nombre_mat Nombre del material a verificar
     * @param linea_bruta Línea original para mensajes de error
     * @brief Termina el programa si el material no existe
     */
    void verificar_material_existente(escenario const & escena, std::string const & nombre_mat,
                                      std::string const & linea_bruta) {
      if (!escena.tiene_material(nombre_mat)) {
        std::cerr << "Error: Material not found: [" << nombre_mat << "]\n"
                  << "Line: \"" << linea_bruta << "\"\n";
        std::exit(EXIT_FAILURE);
      }
    }

    /**
     * @brief Valida que los componentes de color estén en el rango [0, 1]
     * @param r Componente rojo
     * @param g Componente verde
     * @param b Componente azul
     * @return true si todos los componentes son válidos
     */
    bool color_valido(double r, double g, double b) {
      return r >= 0.0 and r <= 1.0 and g >= 0.0 and g <= 1.0 and b >= 0.0 and b <= 1.0;
    }

    /**
     * @brief Procesa un material mate
     * @param escena Escenario donde agregar el material
     * @param resto Parte de la línea después de la etiqueta
     * @param linea_bruta Línea original para mensajes de error
     */
    void procesar_mate(escenario & escena, std::string const & resto,
                       std::string const & linea_bruta) {
      std::istringstream iss(resto);
      std::string nombre;
      double r = 0.0, g = 0.0, b = 0.0;

      if (!(iss >> nombre >> r >> g >> b)) {
        std::cerr << "Error: Invalid matte material parameters\nLine: \"" << linea_bruta << "\"\n";
        std::exit(EXIT_FAILURE);
      }

      validar_linea(iss, "matte", linea_bruta);

      if (!color_valido(r, g, b)) {
        std::cerr << "Error: Invalid matte material parameters (out of range)\nLine: \""
                  << linea_bruta << "\"\n";
        std::exit(EXIT_FAILURE);
      }

      verificar_material_duplicado(escena, nombre, linea_bruta);
      escena.agregar_material(nombre, material(TipoMaterial::Mate, vector(r, g, b)));
    }

    /**
     * @brief Procesa un material metálico
     * @param escena Escenario donde agregar el material
     * @param resto Parte de la línea después de la etiqueta
     * @param linea_bruta Línea original para mensajes de error
     */
    void procesar_metal(escenario & escena, std::string const & resto,
                        std::string const & linea_bruta) {
      std::istringstream iss(resto);
      std::string nombre;
      double r = 0.0, g = 0.0, b = 0.0, dif = 0.0;

      if (!(iss >> nombre >> r >> g >> b >> dif)) {
        std::cerr << "Error: Invalid metal material parameters\nLine: \"" << linea_bruta << "\"\n";
        std::exit(EXIT_FAILURE);
      }

      validar_linea(iss, "metal", linea_bruta);

      if (!color_valido(r, g, b)) {
        std::cerr << "Error: Invalid metal material parameters (out of range)\nLine: \""
                  << linea_bruta << "\"\n";
        std::exit(EXIT_FAILURE);
      }

      verificar_material_duplicado(escena, nombre, linea_bruta);
      escena.agregar_material(nombre, material(TipoMaterial::Metal, vector(r, g, b), dif));
    }

    /**
     * @brief Procesa un material refractivo
     * @param escena Escenario donde agregar el material
     * @param resto Parte de la línea después de la etiqueta
     * @param linea_bruta Línea original para mensajes de error
     */
    void procesar_refractivo(escenario & escena, std::string const & resto,
                             std::string const & linea_bruta) {
      std::istringstream iss(resto);
      std::string nombre;
      double ior = 0.0;

      if (!(iss >> nombre >> ior) or ior <= 0) {
        std::cerr << "Error: Invalid refractive material parameters\nLine: \"" << linea_bruta
                  << "\"\n";
        std::exit(EXIT_FAILURE);
      }

      validar_linea(iss, "refractive", linea_bruta);
      verificar_material_duplicado(escena, nombre, linea_bruta);

      escena.agregar_material(nombre,
                              material(TipoMaterial::Refractivo, vector(1.0, 1.0, 1.0), 0.0, ior));
    }

    /**
     * @brief Procesa una esfera
     * @param escena Escenario donde agregar la esfera
     * @param resto Parte de la línea después de la etiqueta
     * @param linea_bruta Línea original para mensajes de error
     */
    void procesar_esfera(escenario & escena, std::string const & resto,
                         std::string const & linea_bruta) {
      std::istringstream iss(resto);
      double x = 0.0, y = 0.0, z = 0.0, r = 0.0;
      std::string nombre_mat;

      if (!(iss >> x >> y >> z >> r >> nombre_mat) or r <= 0) {
        std::cerr << "Error: Invalid sphere parameters\nLine: \"" << linea_bruta << "\"\n";
        std::exit(EXIT_FAILURE);
      }

      validar_linea(iss, "sphere", linea_bruta);
      verificar_material_existente(escena, nombre_mat, linea_bruta);

      escena.agregar_esfera(esfera(vector(x, y, z), r, escena.obtener_material(nombre_mat)));
    }

    /**
     * @brief Procesa un cilindro
     * @param escena Escenario donde agregar el cilindro
     * @param resto Parte de la línea después de la etiqueta
     * @param linea_bruta Línea original para mensajes de error
     */
    void procesar_cilindro(escenario & escena, std::string const & resto,
                           std::string const & linea_bruta) {
      std::istringstream iss(resto);
      double x = 0.0, y = 0.0, z = 0.0, r = 0.0, ax = 0.0, ay = 0.0, az = 0.0;
      std::string nombre_mat;

      if (!(iss >> x >> y >> z >> r >> ax >> ay >> az >> nombre_mat) or r <= 0) {
        std::cerr << "Error: Invalid cylinder parameters\nLine: \"" << linea_bruta << "\"\n";
        std::exit(EXIT_FAILURE);
      }

      validar_linea(iss, "cylinder", linea_bruta);
      verificar_material_existente(escena, nombre_mat, linea_bruta);

      escena.agregar_cilindro(
          cilindro(vector(x, y, z), r, escena.obtener_material(nombre_mat), vector(ax, ay, az)));
    }

  }  // namespace

  /**
   * @brief Carga una escena completa desde un archivo de definición
   * @param nombre_archivo Ruta del archivo a cargar
   * @return Escenario completo con materiales y objetos
   * @brief Procesa línea por línea el archivo, interpretando cada entidad
   * y construyendo el escenario correspondiente
   */
  escenario leer_escena::cargar_desde_archivo(std::string const & nombre_archivo) {
    std::ifstream archivo(nombre_archivo);
    if (!archivo.is_open()) {
      std::cerr << "Error al abrir el archivo: " << nombre_archivo << "\n";
      std::exit(EXIT_FAILURE);
    }
    escenario escena;
    std::string linea_bruta;
    std::string linea, etiqueta, resto;
    // Procesar cada línea del archivo
    while (std::getline(archivo, linea_bruta)) {
      linea = recortar(linea_bruta);
      if (linea.empty()) {
        continue;  // Saltar líneas vacías
      }
      // Separar etiqueta y parámetros
      size_t const espacio = linea.find(' ');
      if (espacio != std::string::npos) {
        etiqueta = linea.substr(0, espacio);
        resto    = linea.substr(espacio + 1);
      } else {
        etiqueta = linea;
        resto    = "";
      }
      if (etiqueta == "matte:") {
        procesar_mate(escena, resto, linea_bruta);
      } else if (etiqueta == "metal:") {
        procesar_metal(escena, resto, linea_bruta);
      } else if (etiqueta == "refractive:") {
        procesar_refractivo(escena, resto, linea_bruta);
      } else if (etiqueta == "sphere:") {
        procesar_esfera(escena, resto, linea_bruta);
      } else if (etiqueta == "cylinder:") {
        procesar_cilindro(escena, resto, linea_bruta);
      } else {
        std::cerr << "Error: Unknown scene entity: " << etiqueta << "\n";
        std::exit(EXIT_FAILURE);
      }
    }
    return escena;
  }

}  // namespace render
