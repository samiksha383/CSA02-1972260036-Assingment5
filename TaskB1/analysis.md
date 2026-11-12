\# TASK B1 – Implement and Instrument the Recursive Functions



\## B1.1 – Validation Tests

\* \*\*`0` and `1`:\*\* Verifies immediate base-case termination (`digital\_root(0) = 0`, `power\_fast(5, 0) = 1`) without infinite recursion.

\* \*\*Single-digit boundary (`9`):\*\* Verifies `n < 10` stops before digit-splitting.

\* \*\*Multi-digit / Multi-step (`9875` and `3^5`):\*\* Verifies multi-step digit reduction (`9875 -> 29 -> 11 -> 2`) and odd-exponent multiplication (`3^5 = 243`).



\## B1.2 – Comparison with Part A Pseudocode

The C code matches Part A's pseudocode (`DIV` mapped to integer `/`, `MOD` mapped to `%`), with `long long` used for return types to prevent 32-bit overflow when computing `2^30 = 1,073,741,824`.



\## B1.3 – Call Count Comparison (`n = 10, 100, 1000, 10000`)



| Exponent `n` | Linear Calls (`n + 1`) | Fast Halving Calls (`floor(log2 n) + 2`) | Double-Branch Calls (`2\*n + 1`) |

| :--- | :--- | :--- | :--- |

| `10` | `11` | `5` | `21` |

| `30` | `31` | `6` | `61` |

| `100` | `101` | `8` | `201` |

| `1,000` | `1,001` | `11` | `2,001` |

| `10,000` | `10,001` | `15` | `20,001` |



\* \*\*Why Double-Branch (`power\_fast(b, exp/2) \* power\_fast(b, exp/2)`) Explodes to `61` Calls for `2^30`:\*\* Calling `power\_fast` twice per level turns the recurrence from $T(n) = T(n/2) + 1$ ($O(\\log n)$) into $T(n) = 2T(n/2) + 1$, which solves to \*\*$2n + 1 = 61$ calls\*\* ($O(n)$), completely destroying the divide-and-conquer speedup!



\## B1.4 – Stack Overflow Limit

\* On Windows GCC (default `1 MB` or `2 MB` thread stack, \~`32 bytes` per stack frame), `power\_linear(1, n)` crashes with a \*\*Stack Overflow (`0xC00000FD`)\*\* around \*\*`n ≈ 32,000` to `65,000`\*\*, whereas `power\_iterative(1, 100000000)` executes `100,000,000` iterations cleanly in \*\*$O(1)$ stack space\*\*.



\## Alternative Concept (Iterative Square-and-Multiply)

\* \*\*Time Complexity:\*\* \*\*$O(\\log n)$\*\* for both recursive `power\_fast` and iterative square-and-multiply (vs. \*\*$O(n)$\*\* for `power\_linear`).

\* \*\*Space Complexity:\*\* \*\*$O(1)$\*\* stack space for iterative square-and-multiply (3 local variables), vs. \*\*$O(\\log n)$\*\* stack frames for `power\_fast` and \*\*$O(n)$\*\* stack frames for `power\_linear`.



