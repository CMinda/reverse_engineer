# 01 — Problem Statement

## Context

A compiled x86 binary executable was provided as part of a reverse engineering exercise. When executed, the binary displayed a numeric code on screen. This code was generated internally from a hardcoded string embedded within the binary itself.

After listing the disassembled ASM code, the binary was deleted. Only the ASM listing remains. The goal is to perform **static reverse engineering** on the assembly to:

1. Understand what the binary does
2. Reconstruct the original C source code
3. Recompile and verify the output matches the expected numeric code
4. Modify the algorithm to work on a different input string

---

## Known Information

Two key addresses were provided as hints within the disassembly:

| Offset | Register | Value |
|--------|----------|-------|
| `<+36>` | `eax` | Address of string: `3jd9cjfk98hnd` |
| `<+110>` | `eax` | Address of format string: `[+] Codigo generado: %i\n` |

---

## Tasks

### Task 1 — Divide into Basic Blocks
Divide the ASM listing into **basic blocks**, splitting at:
- API/function calls (e.g., `strlen`, `printf`)
- Conditional jumps (`je`, `jl`, `jg`, etc.)
- Unconditional jumps (`jmp`)

### Task 2 — Control Flow Diagram
Draw a control flow diagram connecting all basic blocks with directional arrows showing execution flow.

### Task 3 — Identify Control Structures
Determine whether an `if` statement, `for` loop, `while` loop, or other control structure exists. Identify which basic blocks are involved.

### Task 4 — Reconstruct C Code
Convert the full `main()` function from assembly to equivalent C source code.

### Task 5 — Compile and Execute (Original String)
Compile with:
```bash
gcc source.c -o surca -m32
```
Run and record the output.

### Task 6 — Modify and Recompile (New String)
Replace the string at `<+36>` with `Congratulations!`. Recompile and record the new output.

---

## Expected Outputs

| Input String | Expected Output |
|-------------|----------------|
| `3jd9cjfk98hnd` | `[+] Codigo generado: 15015` |
| `Congratulations!` | `[+] Codigo generado: 26080` |
