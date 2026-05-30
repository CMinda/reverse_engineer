# 02 — Basic Block Analysis

The ASM listing of `main()` was divided into **6 basic blocks** following two splitting criteria:

1. **API/function calls** — every `call` instruction ends a block
2. **Conditional or unconditional jumps** — every `jmp`, `jl`, `jge`, etc. starts or ends a block

---

## Basic Block #1 — Stack Frame Setup

```asm
<main+0>:   push   ebp
<main+1>:   mov    ebp, esp
<main+3>:   sub    esp, 0x28
<main+6>:   and    esp, 0xfffffff0
<main+9>:   mov    eax, 0x0
<main+14>:  sub    esp, eax
```

**Purpose:** Standard function prologue. Establishes the stack frame by saving `ebp`, setting the new frame base, and allocating local variable space (`0x28` = 40 bytes). Aligns the stack to a 16-byte boundary.

**Variables allocated on stack:**

| Variable | Offset | Role |
|----------|--------|------|
| `ebp-0x14` | -20 | Pointer to the input string |
| `ebp-0x18` | -24 | String length (from `strlen`) |
| `ebp-0x10` | -16 | Accumulator (running total) |
| `ebp-0x0c` | -12 | Loop counter `x` |

---

## Basic Block #2 — Load String & Call strlen

```asm
<main+36>:  mov    eax, <addr_of_string>   ; load "3jd9cjfk98hnd"
<main+41>:  mov    DWORD PTR [ebp-0x14], eax  ; store string ptr
<main+44>:  mov    eax, DWORD PTR [ebp-0x14]  ; pass to strlen
<main+47>:  mov    DWORD PTR [esp], eax
<main+50>:  call   strlen
<main+55>:  mov    DWORD PTR [ebp-0x10], 0x0  ; init accumulator = 0
```

**Purpose:**
- Loads the hardcoded string address (`3jd9cjfk98hnd`) into `eax` and stores it in `ebp-0x14`
- Passes the string to `strlen()` — result (length) returned in `eax`
- Initializes the accumulator variable (`ebp-0x10`) to `0`

**Split reason:** `call strlen` — API function call.

---

## Basic Block #3 — Store Length & Jump to Loop Condition

```asm
<main+62>:  mov    DWORD PTR [ebp-0x18], eax  ; store strlen result
<main+65>:  mov    DWORD PTR [ebp-0x0c], 0x0  ; init counter x = 0
<main+72>:  jmp    <main+96>                  ; jump to loop condition check
```

**Purpose:**
- Stores the return value of `strlen` (in `eax`) into `ebp-0x18`
- Initializes the loop counter (`ebp-0x0c`) to `0`
- Unconditionally jumps to the loop condition check (Block #4b)

**Split reason:** Unconditional `jmp` instruction.

---

## Basic Block #4 — Loop Body (Core Algorithm)

This is the heart of the program. It implements the `for` loop body.

```asm
<main+74>:  mov    eax, DWORD PTR [ebp-0x14]  ; eax = string ptr
<main+77>:  add    eax, DWORD PTR [ebp-0x0c]  ; eax = &string[x]  (pointer arithmetic)
<main+80>:  movsx  eax, BYTE PTR [eax]         ; eax = string[x]  (load 1 byte, sign-extend)
<main+83>:  imul   eax, DWORD PTR [ebp-0x18]  ; eax = string[x] * strlen
<main+87>:  add    DWORD PTR [ebp-0x10], eax  ; accumulator += eax
<main+90>:  add    DWORD PTR [ebp-0x0c], 0x1  ; x++  (counter increment)
```

**Purpose:**
- Computes the address of `string[x]` by adding the counter to the base pointer
- Loads the character at that position (`movsx` sign-extends the byte to 32-bit)
- Multiplies the character's ASCII value by the total string length (`imul`)
- Adds the product to the accumulator (`ebp-0x10`)
- Increments the loop counter

**Example (first iteration with `3jd9cjfk98hnd`):**
- `string[0]` = `'3'` → ASCII `51` (0x33)
- `strlen` = `13` (0x0D)
- `51 × 13 = 663`
- Accumulator = `0 + 663 = 663`

---

## Basic Block #4b — Loop Condition Check

```asm
<main+96>:  mov    eax, DWORD PTR [ebp-0x0c]   ; eax = x (counter)
<main+99>:  cmp    eax, DWORD PTR [ebp-0x18]   ; compare x vs strlen
<main+102>: jl     <main+74>                   ; if x < strlen → loop body
```

**Purpose:**
- Loads the counter into `eax`
- Compares counter against string length
- If counter is less than length (`jl` = jump if less), goes back to Block #4 (loop body)
- Otherwise falls through to Block #5 (print result)

**Split reason:** Conditional jump `jl` — controls the loop.

---

## Basic Block #5 — Print Result

```asm
<main+108>: mov    eax, DWORD PTR [ebp-0x10]   ; eax = accumulated result
<main+110>: mov    edx, <addr_format_string>   ; "[+] Codigo generado: %i\n"
<main+115>: mov    DWORD PTR [esp+0x4], eax    ; push result as 2nd arg
<main+119>: mov    DWORD PTR [esp], edx        ; push format string as 1st arg
<main+122>: call   printf                      ; printf("[+] ...", result)
```

**Purpose:**
- Loads the final accumulated result from `ebp-0x10`
- Loads the format string address (`[+] Codigo generado: %i\n`)
- Sets up the `printf` call with both arguments
- Calls `printf`

**Split reason:** `call printf` — API function call.

---

## Basic Block #6 — Stack Cleanup & Return

```asm
<main+127>: mov    eax, 0x0    ; return value = 0
<main+132>: leave              ; restore ebp, esp
<main+133>: ret                ; return to caller
```

**Purpose:** Standard function epilogue. Sets return value to `0`, restores the stack frame, and returns to the OS.

---

## Summary Table

| Block | Offsets | Key Operation | Split Reason |
|-------|---------|--------------|--------------|
| BB1 | +0 → +14 | Stack frame setup | — (entry) |
| BB2 | +36 → +55 | Load string + `strlen()` | `call strlen` |
| BB3 | +62 → +72 | Store length, init counter | `jmp` |
| BB4 | +74 → +90 | Loop body: multiply & accumulate | falls to BB4b |
| BB4b | +96 → +102 | Loop condition: `x < strlen` | `jl` conditional jump |
| BB5 | +108 → +122 | `printf()` result | `call printf` |
| BB6 | +127 → +133 | Cleanup and `return 0` | — (exit) |
