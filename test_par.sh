#!/bin/bash
set -Eeuo pipefail
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

EXECUTABLE="out/build/default/par/Release/render-par"
EXEC_DIR=$(dirname "$EXECUTABLE")

echo "=== PRUEBAS FUNCIONALES RENDER-PAR ==="
echo "Usando ejecutable: $EXECUTABLE"
echo ""

echo -n "Escena 1 → "
perf stat -r 1 "$EXECUTABLE" configs/config1.txt scene/scene1.txt "$EXEC_DIR/output1-par.ppm" 2>&1 | grep "elapsed"

echo -n "Escena 2 → "
perf stat -r 1 "$EXECUTABLE" configs/config2.txt scene/scene2.txt "$EXEC_DIR/output2-par.ppm" 2>&1 | grep "elapsed"

echo -n "Escena 3 → "
perf stat -r 1 "$EXECUTABLE" configs/config3.txt scene/scene3.txt "$EXEC_DIR/output3-par.ppm" 2>&1 | grep "elapsed"

echo -n "Escena 4 → "
perf stat -r 1 "$EXECUTABLE" configs/config4.txt scene/scene4.txt "$EXEC_DIR/output4-par.ppm" 2>&1 | grep "elapsed"

echo -n "Escena 5 → "
perf stat -r 1 "$EXECUTABLE" configs/config5.txt scene/scene5.txt "$EXEC_DIR/output5-par.ppm" 2>&1 | grep "elapsed"

echo ""
echo "Pruebas de error (deben fallar)"

echo -n "Sin argumentos → "
if "$EXECUTABLE" > /dev/null 2>&1; then echo "Debería fallar"; else echo "Falló como se esperaba"; fi

echo -n "1 argumento → "
if "$EXECUTABLE" configs/config1.txt > /dev/null 2>&1; then echo "Debería fallar"; else echo "Falló como se esperaba"; fi

echo -n "2 argumentos → "
if "$EXECUTABLE" configs/config1.txt scene/scene1.txt > /dev/null 2>&1; then echo "Debería fallar"; else echo "Falló como se esperaba"; fi

echo -n "Config inexistente → "
if "$EXECUTABLE" no.txt scene/scene1.txt "$EXEC_DIR/e1.ppm" > /dev/null 2>&1; then echo "Debería fallar"; else echo "Falló como se esperaba"; fi

echo -n "Escena inexistente → "
if "$EXECUTABLE" configs/config1.txt no.txt "$EXEC_DIR/e2.ppm" > /dev/null 2>&1; then echo "Debería fallar"; else echo "Falló como se esperaba"; fi

echo ""
echo "Script RENDER-PAR finalizado"
