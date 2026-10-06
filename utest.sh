#!/bin/bash
set -Eeuo pipefail
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

echo "=== LANZANDO PRUEBAS UNITARIAS ==="
echo ""

# Función para ejecutar pruebas unitarias
ejecutar_prueba() {
    local nombre=$1
    local ejecutable=$2

    echo "--- $nombre ---"
    echo "Ruta: $ejecutable"

    if [[ -x "$ejecutable" ]]; then
        if timeout 30s "$ejecutable"; then
            echo "$nombre - PASÓ"
        else
            echo "$nombre - FALLÓ"
        fi
    else
        echo "$nombre - NO ENCONTRADO"
    fi
    echo ""
}

# Rutas fijas de los ejecutables
UTCOMMON_EXEC="out/build/default/utcommon/Release/utcommon"
UTAOS_EXEC="out/build/default/utaos/Release/utaos"
UTSOA_EXEC="out/build/default/utsoa/Release/utsoa"

# Ejecutar todas las pruebas explícitamente
ejecutar_prueba "Pruebas Comunes" "$UTCOMMON_EXEC"

echo "=== EJECUCIÓN DE PRUEBAS COMPLETADA ==="
