# 03 — Control Flow Diagram

## ASCII Control Flow Graph

```
                    ┌─────────────────────────┐
                    │      ENTRY / BB1         │
                    │  <main+0> → <main+14>    │
                    │  Stack frame setup       │
                    │  Allocate local vars     │
                    └────────────┬─────────────┘
                                 │
                                 ▼
                    ┌─────────────────────────┐
                    │          BB2             │
                    │  <main+36> → <main+55>  │
                    │  Load "3jd9cjfk98hnd"   │
                    │  call strlen()           │
                    │  Init accumulator = 0    │
                    └────────────┬─────────────┘
                                 │
                                 ▼
                    ┌─────────────────────────┐
                    │          BB3             │
                    │  <main+62> → <main+72>  │
                    │  Store strlen result     │
                    │  Init counter x = 0      │
                    │  jmp → BB4b (condition)  │
                    └────────────┬─────────────┘
                                 │  unconditional jmp
                                 │
                    ┌────────────▼─────────────┐
              ┌────►│         BB4b              │◄─────────┐
              │     │  <main+96> → <main+102>  │          │
              │     │  cmp x, strlen            │          │
              │     │  jl BB4 (x < strlen?)     │          │
              │     └──────────┬────────────────┘          │
              │                │                           │
              │         NO     │        YES                │
              │    (x ≥ len)   │    (x < len)              │
              │                │                           │
              │                ▼                           │
              │     ┌──────────────────────────┐           │
              │     │          BB4             │           │
              │     │  <main+74> → <main+90>  │           │
              │     │  eax = string[x]         │           │
              │     │  eax *= strlen           │           │
              │     │  accumulator += eax      │           │
              │     │  x++                     ├───────────┘
              │     └──────────────────────────┘  (loop back)
              │
              └── falls through when x >= strlen
                                │
                                ▼
                    ┌─────────────────────────┐
                    │          BB5             │
                    │  <main+108> → <main+122>│
                    │  Load result             │
                    │  call printf()           │
                    │  "[+] Codigo generado:   │
                    │       %i\n"              │
                    └────────────┬─────────────┘
                                 │
                                 ▼
                    ┌─────────────────────────┐
                    │          BB6             │
                    │  <main+127> → <main+133>│
                    │  return 0                │
                    │  leave / ret             │
                    └─────────────────────────┘
                                 │
                               EXIT
```

---

## Edge Table

| From | To | Condition |
|------|----|-----------|
| BB1 | BB2 | Always (fall-through) |
| BB2 | BB3 | Always (fall-through after `call strlen`) |
| BB3 | BB4b | Always (unconditional `jmp`) |
| BB4b | BB4 | `x < strlen` (jump taken: `jl`) |
| BB4b | BB5 | `x >= strlen` (jump not taken) |
| BB4 | BB4b | Always (fall-through, loop back) |
| BB5 | BB6 | Always (fall-through after `call printf`) |

---

## Loop Structure

The back-edge from **BB4 → BB4b** creates the loop. This is a classical **for-loop** pattern in x86:

```
BB3:  init (x = 0)
  └──► BB4b: condition (x < strlen?)
              ├── TRUE  ──► BB4: body (accumulate + x++)
              │              └──► BB4b  (back edge)
              └── FALSE ──► BB5: exit loop
```

This is equivalent to:
```c
for (x = 0; x < strlen; x++) {
    accumulator += string[x] * strlen;
}
```
