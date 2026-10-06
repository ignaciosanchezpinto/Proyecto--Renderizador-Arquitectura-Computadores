#!/usr/bin/env python3
import sys
import math

def leer_ppm(ruta):
    """Lee un archivo PPM (P3 o P6) y devuelve ancho, alto, maxval y lista de píxeles (r, g, b)."""
    with open(ruta, "rb") as f:
        tipo = f.readline().strip()
        if tipo not in [b"P3", b"P6"]:
            raise ValueError("Formato PPM no soportado (solo P3 o P6)")

        # Función interna para leer línea saltando comentarios
        def leer_linea():
            linea = f.readline()
            while linea.startswith(b"#"):
                linea = f.readline()
            return linea

        # Leer dimensiones
        linea = leer_linea()
        w, h = map(int, linea.split())
        maxval = int(leer_linea())

        datos = []
        if tipo == b"P3":
            # Leer valores de manera textual
            contenido = f.read().split()
            if len(contenido) != w * h * 3:
                raise ValueError("Cantidad de datos P3 no coincide con ancho*alto*3")
            for i in range(0, len(contenido), 3):
                datos.append((int(contenido[i]), int(contenido[i+1]), int(contenido[i+2])))
        else:
            # P6: datos binarios
            raw = f.read()
            if len(raw) != w * h * 3:
                raise ValueError("Cantidad de datos P6 no coincide con ancho*alto*3")
            for i in range(0, len(raw), 3):
                datos.append((raw[i], raw[i+1], raw[i+2]))

        return w, h, maxval, datos


def diferencia_pixel(p1, p2):
    """Calcula la diferencia promedio de un pixel entre dos imágenes."""
    return (abs(p1[0]-p2[0]) + abs(p1[1]-p2[1]) + abs(p1[2]-p2[2])) / 3.0


def comparar(im1, im2):
    """Compara dos imágenes PPM y muestra diferencia máxima y RMS."""
    w1, h1, max1, data1 = leer_ppm(im1)
    w2, h2, max2, data2 = leer_ppm(im2)

    if (w1, h1) != (w2, h2):
        print("❌ Las imágenes NO tienen el mismo tamaño.")
        sys.exit(1)

    max_dif = 0.0
    sum_quad = 0.0
    total_pixels = w1 * h1

    for p1, p2 in zip(data1, data2):
        dif = diferencia_pixel(p1, p2)
        max_dif = max(max_dif, dif)
        sum_quad += dif * dif

    rms_error = math.sqrt(sum_quad / total_pixels)

    print(f"Diferencia máxima  : {max_dif:.4f} (aceptable < 150)")
    print(f"Error RMS          : {rms_error:.4f} (aceptable < 10)")

    if max_dif < 150 and rms_error < 10:
        print("\n✅ RESULTADO: Aceptable según los umbrales")
    else:
        print("\n❌ RESULTADO: No aceptable")


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Uso: python3 comparar_ppm.py imagen1.ppm imagen2.ppm")
        sys.exit(1)

    comparar(sys.argv[1], sys.argv[2])
