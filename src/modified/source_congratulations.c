/**
 * source_congratulations.c — Modified version with new input string
 *
 * Change: string at <main+36> replaced from "3jd9cjfk98hnd"
 *         to "Congratulations!" as per exercise Task 6.
 *
 * Compile:
 *   gcc source_congratulations.c -o surca_congratulations -m32
 *
 * Expected output:
 *   [+] Codigo generado: 26080
 *
 * Verification:
 *   "Congratulations!" has 16 characters.
 *   Sum of ASCII values = 1630
 *   1630 × 16 = 26080
 */

#include <stdio.h>
#include <string.h>

int main() {
    /* Modified: "3jd9cjfk98hnd" → "Congratulations!" */
    char *cadena = "Congratulations!";

    int longitud_cadena = strlen(cadena);
    int resultado = 0;
    int x;

    for (x = 0; x < longitud_cadena; x++) {
        resultado += cadena[x] * longitud_cadena;
    }

    printf("[+] Codigo generado: %i\n", resultado);

    return 0;
}
