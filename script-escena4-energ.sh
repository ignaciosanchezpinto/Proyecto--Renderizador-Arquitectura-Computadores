#!/bin/bash
set -Eeuo pipefail

# --- CONFIGURACIÓN ---
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

# Ruta al ejecutable
EXECUTABLE="out/build/default/par/Release/render-par"
EXEC_DIR=$(dirname "$EXECUTABLE")
# Archivo de imagen temporal (se sobrescribe siempre para no llenar el disco)
SALIDA_IMAGEN="$EXEC_DIR/temp_output.ppm"

# --- SELECCIÓN DE ESCENA ---
# Cambia el numero aquí si quieres otra escena (ej. config3.txt)
NUM_ESCENA=4
CONFIG="configs/config${NUM_ESCENA}.txt"
ESCENA="scene/scene${NUM_ESCENA}.txt"

# Comprobación simple
if [ ! -f "$EXECUTABLE" ]; then
    echo "ERROR: No encuentro el ejecutable en: $EXECUTABLE"
    exit 1
fi

if [ ! -f "$CONFIG" ]; then
    echo "ERROR: No encuentro la configuración: $CONFIG"
    exit 1
fi

echo "=========================================================="
echo " INICIO BARRIDO DE ENERGÍA (ESCENA $NUM_ESCENA)"
echo " Guardando datos en: resultados_energia.txt"
echo "=========================================================="

# Limpiamos el archivo de resultados previo (opcional, para empezar de cero)
echo "--- RESULTADOS DE ENERGÍA ESCENA $NUM_ESCENA ---" > resultados_energia.txt

# Listas de parámetros
HILOS_LISTA=(1 2 4 8 16 32 64 128 256)
PARTICIONADORES=("simple" "static" "auto")
GRANOS=(1 16 64 256)

for particionador in "${PARTICIONADORES[@]}"; do
    for grano in "${GRANOS[@]}"; do
        for hilos in "${HILOS_LISTA[@]}"; do
            
            # Imprimir por pantalla para que sepas por dónde va
            echo "Procesando: Hilos=$hilos | Part=$particionador | Grano=$grano"

            # Escribir cabecera en el archivo de texto
            echo "-------------------------------------------" >> resultados_energia.txt
            echo "Hilos=$hilos | Part=$particionador | Grano=$grano" >> resultados_energia.txt
            echo "-------------------------------------------" >> resultados_energia.txt

            # --- EJECUCIÓN ---
            # -r 1 : Una repetición
            # -e power/energy-pkg/ : SOLO ENERGÍA
            # 2>> : Redirige la salida de perf (que sale por error estandar) al archivo
            
            perf stat -r 1 -e power/energy-pkg/ \
                "$EXECUTABLE" "$CONFIG" "$ESCENA" "$SALIDA_IMAGEN" "$hilos" "$particionador" "$grano" 2>> resultados_energia.txt

            echo "" >> resultados_energia.txt # Salto de línea en el archivo

        done
    done
done

echo "=========================================================="
echo "FIN DE LA EJECUCIÓN"
echo "Revisa el archivo 'resultados_energia.txt' para ver los Julios."
echo "=========================================================="