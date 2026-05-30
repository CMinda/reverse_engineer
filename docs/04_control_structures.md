# 04 — Control Structure Identification

## Is there an `if` or `for` in the control flow?

**Yes — there is a `for` loop.** There is no `if` statement.

---

## The `for` Loop

### Basic Blocks Involved

| Block | Role in the Loop |
|-------|-----------------|
| **BB3** | **Initialization** — sets counter `x = 0` before the loop starts |
| **BB4b** | **Condition check** — `CMP x, strlen` + `jl` — the loop guard |
| **BB4** | **Loop body** — character multiply-accumulate + `x++` |

### How the Loop is Encoded in ASM

```asm
; ── INITIALIZATION (BB3) ─────────────────────────────────────
<main+65>:  mov    DWORD PTR [ebp-0x0c], 0x0   ; x = 0
<main+72>:  jmp    <main+96>                   ; jump straight to condition

; ── CONDITION CHECK (BB4b) ───────────────────────────────────
<main+96>:  mov    eax, DWORD PTR [ebp-0x0c]   ; eax = x
<main+99>:  cmp    eax, DWORD PTR [ebp-0x18]   ; compare x vs strlen
<main+102>: jl     <main+74>                   ; if x < strlen → loop body

; ── LOOP BODY (BB4) ──────────────────────────────────────────
<main+74>:  mov    eax, DWORD PTR [ebp-0x14]   ; eax = string ptr
<main+77>:  add    eax, DWORD PTR [ebp-0x0c]   ; eax = &string[x]
<main+80>:  movsx  eax, BYTE PTR [eax]          ; eax = (int) string[x]
<main+83>:  imul   eax, DWORD PTR [ebp-0x18]   ; eax *= strlen
<main+87>:  add    DWORD PTR [ebp-0x10], eax   ; accumulator += eax
<main+90>:  add    DWORD PTR [ebp-0x0c], 0x1   ; x++
             ; falls through to BB4b → back to condition
```

### Equivalent C

```c
for (x = 0; x < string_length; x++) {
    result += string[x] * string_length;
}
```

---

## Why This is a `for` Loop and Not `while` or `do-while`

The key indicator is the **initial unconditional jump** from BB3 to BB4b:

```asm
<main+72>:  jmp    <main+96>    ; skip body, go straight to condition
```

This pattern — where code jumps over the loop body to check the condition first — is the classic x86 compiler output for a **`for` loop** (or equivalently a `while` loop). The condition is always checked **before** the body executes, which means if `strlen` were 0, the body would never run. A `do-while` would not have this initial jump.

---

## Variable Roles

| ASM Variable | C Variable | Role |
|-------------|-----------|------|
| `ebp-0x14` | `cadena` | Pointer to the input string |
| `ebp-0x18` | `longitud_cadena` | String length (from `strlen`) |
| `ebp-0x10` | `resultado` | Accumulator — sum of `char × length` |
| `ebp-0x0c` | `x` | Loop counter |

---

## Is There an `if` Statement?

**No.** The only conditional instruction is the `jl` at `<main+102>`, which implements the loop termination condition (`x < strlen`). There is no separate branching logic for an `if`/`else` structure — the CFG has a single back-edge (loop) with no diverging branches inside the loop body.
