# Motor de Trazado de Rayos Paralelizado en C++ (Ray Tracer)

Este proyecto implementa un motor de renderizado 3D fotorrealista desde cero utilizando la técnica de **Trazado de Rayos (Ray Tracing)** en C++. El sistema simula el comportamiento físico de la luz, calculando colisiones, reflejos y refracciones sobre distintas geometrías. 

Para lograr un rendimiento óptimo, el motor está fuertemente paralelizado empleando **Intel TBB (Threading Building Blocks)**, lo que permite distribuir la carga de cálculo de los píxeles entre todos los núcleos disponibles del procesador. Además, el proyecto cuenta con un entorno de pruebas automatizado para evaluar la escalabilidad, el rendimiento y el consumo energético del motor.

## 📁 Estructura del Proyecto

El código fuente y las herramientas de evaluación están organizados en los siguientes directorios:

* **`common/`**: Contiene las definiciones (`include`) compartidas de estructuras matemáticas (vectores, rayos), figuras geométricas (esferas, cilindros), materiales y utilidades de configuración.
* **`par/`**: Código fuente de la implementación paralela (`src/renderizador.cpp`, `main.cpp`), donde reside la lógica intensiva de TBB.
* **`configs/` & `scene/`**: Archivos de texto con los parámetros de configuración y las descripciones geométricas de las escenas 3D a renderizar.
* **`utcommon/`**: Utilidades y tests unitarios compartidos.
* **Entorno de Desarrollo:** Archivos de configuración para contenedores y editores como `.devcontainer`, `.vscode` e integraciones de linting/formateo (`.clang-format`, `.clang-tidy`).

## 🛠️ Uso y Ejecución del Motor

El binario principal se compila con **CMake** y se ejecuta desde la línea de comandos. 

**Sintaxis básica:**
`./render-par <config> <escena> <salida> [hilos] [part] [grano]`

* **Parámetros obligatorios:** `<config>` (archivo de configuración), `<escena>` (geometría y luces), `<salida>` (imagen PPM resultante).
* **Parámetros opcionales de paralelización (TBB):**
  * `[hilos]`: Número máximo de hilos concurrentes.
  * `[part]`: Estrategia de particionado (`auto`, `static` o `simple`).
  * `[grano]`: Tamaño del bloque de píxeles (grain size).

## 📊 Evaluación de Rendimiento y Energía

El proyecto incluye un conjunto de *scripts* en Bash (`.sh`) en la raíz del repositorio diseñados para perfilar exhaustivamente el rendimiento y la huella energética del motor para cada escena renderizada.

Se disponen de *scripts* independientes para cada caso de prueba, divididos en dos enfoques principales:
* **Rendimiento (`script-escenaX.sh`):** Utilizan herramientas como `perf stat` para medir los tiempos de ejecución y las métricas del hardware. Ejecutan barridos automáticos probando diferentes combinaciones de carga de trabajo:
  * Escalabilidad de hilos: `1, 2, 4, 8, 16, 32, 64, 128, 256`.
  * Particionadores de TBB: `simple`, `static`, `auto`.
  * Tamaños de grano: `1, 16, 64, 256`.
* **Consumo Energético (`script-escenaX-energ.sh`):** Diseñados específicamente para monitorizar y registrar el consumo de potencia durante el estrés computacional de la generación de la imagen.

Los resultados de estas evaluaciones se vuelcan automáticamente en archivos de texto (como `resultados_rend.txt`), permitiendo un análisis posterior para encontrar la configuración óptima (el *sweet spot* de hilos, particionador y grano) que ofrezca el mejor balance entre tiempo de renderizado y consumo de CPU.

## 🧪 Pruebas y Construcción

Para facilitar el despliegue y la validación, el repositorio cuenta con *scripts* de utilidad adicionales:
* `build.sh`: Automatiza la construcción del proyecto con CMake.
* `utest.sh` y `test_par.sh`: Lanzan las baterías de pruebas unitarias y de integración para garantizar que las optimizaciones en el código paralelo no rompan la física de las simulaciones de luz.