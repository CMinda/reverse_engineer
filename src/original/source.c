/**
 * source.c — Reconstructed from x86 ASM via reverse engineering
 *
 * Original binary: deleted (only ASM listing available)
 * Reconstructed by: Carlos Minda
 * Module: Reversing — Ingeniería Inversa
 *
 * Algorithm:
 *   For each character in the input string, multiply its ASCII value
 *   by the total string length, and accumulate the results.
 *   Print the final accumulated value.
 *
 * Compile:
 *   gcc source.c -o surca -m32
 *
 * Expected output:
 *   [+] Codigo generado: 15015
 */

#include <stdio.h>
#include <string.h>

int main() {
    /* ebp-0x14: string pointer loaded at <main+36> */
    char *cadena = "3jd9cjfk98hnd";

    /* ebp-0x18: string length via strlen() call at <main+50> */
    int longitud_cadena = strlen(cadena);

    /* ebp-0x10: accumulator, initialized to 0 at <main+55> */
    int resultado = 0;

    /* ebp-0x0c: loop counter, initialized to 0 at <main+65> */
    int x;

    /*
     * for loop reconstructed from:
     *   BB3  (init):      mov [ebp-0x0c], 0x0  +  jmp <+96>
     *   BB4b (condition): cmp eax, [ebp-0x18]  +  jl <+74>
     *   BB4  (body):      movsx + imul + add   +  inc [ebp-0x0c]
     */
    for (x = 0; x < longitud_cadena; x++) {
        resultado += cadena[x] * longitud_cadena;
    }

    /* printf call at <main+122> with format string loaded at <main+110> */
    printf("[+] Codigo generado: %i\n", resultado);

    return 0;  /* <main+127>: mov eax, 0x0 */
}
