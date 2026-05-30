# 05 — C Code Reconstruction

## Mapping ASM → C

The reconstruction translates each basic block directly into C constructs.

### Variable Mapping

| ASM Location | C Variable | Type | Initialized To |
|-------------|-----------|------|---------------|
| `ebp-0x14` | `cadena` | `char *` | `"3jd9cjfk98hnd"` (from `<+36>`) |
| `ebp-0x18` | `longitud_cadena` | `int` | `strlen(cadena)` |
| `ebp-0x10` | `resultado` | `int` | `0` |
| `ebp-0x0c` | `x` | `int` | `0` (loop counter) |

---

### Block-by-Block Reconstruction

**BB1 → Function signature + local variable declarations**
```c
int main() {
    char *cadena;
    int longitud_cadena;
    int resultado;
    int x;
```

**BB2 → String assignment and strlen call**
```c
    cadena = "3jd9cjfk98hnd";       // <+36>: load string address into eax → ebp-0x14
    longitud_cadena = strlen(cadena); // <+50>: call strlen, result in eax
    resultado = 0;                    // <+55>: mov DWORD PTR [ebp-0x10], 0x0
```

**BB3 → Counter initialization + jump to condition (loop setup)**
```c
    // ebp-0x18 = longitud_cadena (already set)
    x = 0;                            // <+65>: mov DWORD PTR [ebp-0x0c], 0x0
    // jmp to condition → start of for loop
```

**BB4b + BB4 → for loop (condition + body)**
```c
    for (x = 0; x < longitud_cadena; x++) {  // BB4b: cmp + jl
        resultado += cadena[x] * longitud_cadena; // BB4: movsx + imul + add
    }
```

**BB5 → printf**
```c
    printf("[+] Codigo generado: %i\n", resultado); // <+110> + call printf
```

**BB6 → return**
```c
    return 0;
}
```

---

## Final Reconstructed C Code

```c
#include <stdio.h>
#include <string.h>

int main() {
    char *cadena           = "3jd9cjfk98hnd";   /* <+36>  */
    int   longitud_cadena  = strlen(cadena);
    int   resultado        = 0;
    int   x;

    for (x = 0; x < longitud_cadena; x++) {
        resultado += cadena[x] * longitud_cadena;
    }

    printf("[+] Codigo generado: %i\n", resultado);  /* <+110> */

    return 0;
}
```

---

## Compilation

```bash
gcc source.c -o surca -m32
```

> **Note:** The `-m32` flag compiles for 32-bit x86, matching the architecture of the original binary. On a 64-bit system, install multilib support first:
> ```bash
> sudo apt install gcc-multilib
> ```

---

## Execution — Original String

```bash
./surca
```

**Output:**
```
[+] Codigo generado: 15015
```

### Manual Verification

```
String: "3jd9cjfk98hnd"   Length = 13

Char  ASCII   × 13   Running Total
────  ─────  ──────  ─────────────
 3     51     663        663
 j    106    1378       2041
 d    100    1300       3341
 9     57     741       4082
 c     99    1287       5369
 j    106    1378       6747
 f    102    1326       8073
 k    107    1391       9464
 9     57     741      10205
 8     56     728      10933
 h    104    1352      12285
 n    110    1430      13715
 d    100    1300      15015  ✅
```

---

## Modification — Replace Input String

Change the string at `<+36>` from `"3jd9cjfk98hnd"` to `"Congratulations!"`:

```c
char *cadena = "Congratulations!";   /* modified */
```

Recompile and run:

```bash
gcc source_congratulations.c -o surca_congratulations -m32
./surca_congratulations
```

**Output:**
```
[+] Codigo generado: 26080
```

### Manual Verification

```
String: "Congratulations!"   Length = 16

Char  ASCII   × 16   Running Total
────  ─────  ──────  ─────────────
 C     67    1072       1072
 o    111    1776       2848
 n    110    1760       4608
 g    103    1648       6256
 r    114    1824       8080
 a     97    1552       9632
 t    116    1856      11488
 u    117    1872      13360
 l    108    1728      15088
 a     97    1552      16640
 t    116    1856      18496
 i    105    1680      20176
 o    111    1776      21952
 n    110    1760      23712
 s    115    1840      25552
 !     33     528      26080  ✅
```
