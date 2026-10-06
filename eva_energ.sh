#!/bin/bash
set -Eeuo pipefail
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

# Render-par
EXECUTABLE="out/build/default/par/Release/render-par"
EXEC_DIR=$(dirname "$EXECUTABLE")

echo "=========================================================="
echo "INICIO PRUEBA DE ENERGÍA RENDER-PAR"
echo "=========================================================="

echo "--- CONFIG1 / SCENE1 ---"
perf stat -r 1 -e power/energy-pkg/ \
    "$EXECUTABLE" configs/config1.txt scene/scene1.txt "$EXEC_DIR/output1-par.ppm"

echo "--- CONFIG2 / SCENE2 ---"
perf stat -r 1 -e power/energy-pkg/ \
    "$EXECUTABLE" configs/config2.txt scene/scene2.txt "$EXEC_DIR/output2-par.ppm"

echo "--- CONFIG3 / SCENE3 ---"
perf stat -r 1 -e power/energy-pkg/ \
    "$EXECUTABLE" configs/config3.txt scene/scene3.txt "$EXEC_DIR/output3-par.ppm"

echo "--- CONFIG4 / SCENE4 ---"
perf stat -r 1 -e power/energy-pkg/ \
    "$EXECUTABLE" configs/config4.txt scene/scene4.txt "$EXEC_DIR/output4-par.ppm"

echo "--- CONFIG5 / SCENE5 ---"
perf stat -r 1 -e power/energy-pkg/ \
    "$EXECUTABLE" configs/config5.txt scene/scene5.txt "$EXEC_DIR/output5-par.ppm"

echo "=========================================================="
echo "FIN PRUEBA DE ENERGÍA RENDER-PAR"
echo "=========================================================="
