# 🔍 Reverse Engineering — Binary Analysis & Code Reconstruction

> **Module:** Reversing — Ingeniería Inversa  
> **Institution:** Universidad Hemisferios — Cybersecurity Master's Program  
> **Author:** Carlos Minda  

---

## 📋 Overview

A compiled x86 binary was provided that prints a numeric code derived from a hardcoded string. The binary was subsequently deleted, leaving only its disassembled ASM listing. Using **static reverse engineering techniques**, the assembly was analyzed, divided into basic blocks, mapped into a control flow diagram, and fully reconstructed into compilable C source code — then compiled, executed, and modified to operate on a new input string.

---

## 🎯 Challenge Statement

> A binary executable was obtained that, upon execution, displays a text with a numeric code. This code is generated from a string stored within the binary itself.
>
> After listing the ASM code, the binary was deleted. Only the copied ASM listing remains.
>
> The code-generation algorithm must be reused, so reverse engineering is required to analyze the binary and reconstruct the source code in a form that can be modified and recompiled correctly.

---

## ✅ Tasks Completed

| # | Task | Result |
|---|------|--------|
| 1 | Divide ASM into basic blocks | 6 basic blocks identified |
| 2 | Draw the control flow diagram | Loop + conditional structure mapped |
| 3 | Identify control structures | `for` loop via `CMP` + conditional jump |
| 4 | Reconstruct C source code | Fully reconstructed `main()` function |
| 5 | Compile with `gcc -m32` and execute | Output: `[+] Codigo generado: 15015` |
| 6 | Modify input string and recompile | Output: `[+] Codigo generado: 26080` |

---

## 🧠 Algorithm (Reverse Engineered)

The binary computes a **weighted sum** of ASCII character values:

```
result = Σ (ASCII(char[i]) × strlen(string))   for i = 0 to len-1
```

In pseudocode:
```
result = 0
for each character c in string:
    result += ascii(c) * len(string)
print "[+] Codigo generado: {result}"
```

### Verification

**String:** `3jd9cjfk98hnd` (length = 13)

| Char | ASCII | × 13 |
|------|-------|------|
| `3`  | 51    | 663  |
| `j`  | 106   | 1378 |
| `d`  | 100   | 1300 |
| `9`  | 57    | 741  |
| `c`  | 99    | 1287 |
| `j`  | 106   | 1378 |
| `f`  | 102   | 1326 |
| `k`  | 107   | 1391 |
| `9`  | 57    | 741  |
| `8`  | 56    | 728  |
| `h`  | 104   | 1352 |
| `n`  | 110   | 1430 |
| `d`  | 100   | 1300 |
| **Total** | | **15015** ✅ |

**String:** `Congratulations!` (length = 16)  
Sum of ASCII values = 1630 → 1630 × 16 = **26080** ✅

---

## 📁 Repository Structure

```
reverse-engineering-lab/
├── README.md                        # This file
├── docs/
│   ├── 01_problem_statement.md      # Challenge description and goals
│   ├── 02_basic_blocks.md           # ASM divided into 6 basic blocks
│   ├── 03_control_flow_diagram.md   # ASCII control flow graph
│   ├── 04_control_structures.md     # Identified for loop and CMP logic
│   └── 05_code_reconstruction.md    # Step-by-step C reconstruction
└── src/
    ├── original/
    │   └── source.c                 # Reconstructed C (original string)
    ├── modified/
    │   └── source_congratulations.c # Modified string version
    └── Makefile                     # Build both versions
```

---

## 🚀 Quick Start

```bash
# Clone and build
cd src/
make all

# Run original (expects: [+] Codigo generado: 15015)
./original/surca

# Run modified (expects: [+] Codigo generado: 26080)
./modified/surca_congratulations
```

> **Requirement:** GCC with 32-bit support (`gcc-multilib` on 64-bit systems)
> ```bash
> sudo apt install gcc-multilib
> ```

---

## 🔗 Tools Used

| Tool | Purpose |
|------|---------|
| `objdump` / `gdb` | Initial ASM listing |
| GCC (`-m32`) | 32-bit compilation |
| Manual analysis | Basic block division and C reconstruction |
