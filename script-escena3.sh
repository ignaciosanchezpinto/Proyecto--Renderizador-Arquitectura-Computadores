#!/bin/bash
set -Eeuo pipefail

export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

# Ruta al ejecutable
EXECUTABLE="out/build/default/par/Release/render-par"
EXEC_DIR=$(dirname "$EXECUTABLE")
# Archivo de imagen temporal (se sobrescribe siempre)
SALIDA_IMAGEN="$EXEC_DIR/temp_output.ppm"

# Vamos a ejecutar la escena3
CONFIG="configs/config3.txt"
ESCENA="scene/scene3.txt"

# Comprobación simple
if [ ! -f "$EXECUTABLE" ]; then
    echo "ERROR: No encuentro el ejecutable en: $EXECUTABLE"
    exit 1
fi

echo "=============== ESCENA 3 =================="

# Listas de parámetros
HILOS_LISTA=(1 2 4 8 16 32 64 128 256)
PARTICIONADORES=("simple" "static" "auto")
GRANOS=(1 16 64 256)

for particionador in "${PARTICIONADORES[@]}"; do
    for grano in "${GRANOS[@]}"; do
        for hilos in "${HILOS_LISTA[@]}"; do
            
            echo "-------------------------------------------" >> resultados_rend.txt
            echo "Hilos=$hilos | Part=$particionador | Grano=$grano" >> resultados_rend.txt
            echo "-------------------------------------------" >> resultados_rend.txt

            # Ejecutamos perf directamente.
            # -r 1 : Una repetición

            perf stat -r 1 "$EXECUTABLE" "$CONFIG" "$ESCENA" "$SALIDA_IMAGEN" "$hilos" "$particionador" "$grano" 2>> resultados_rend.txt

            echo "" >> resultados_rend.txt # Salto de línea para separar
        done
    done
done

echo "FIN ESCENA 3"
