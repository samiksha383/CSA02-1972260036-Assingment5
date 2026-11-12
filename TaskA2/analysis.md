\# TASK A2 – Design with Recursion



\## A2.1 – SIMATS Standard Recursive Pseudocode



```text

ALGORITHM DigitalRoot(n)

// Input: Non-negative integer n

// Output: Single-digit check digit (digital root)

BEGIN

&#x20;   IF n < 10 THEN

&#x20;       RETURN n                          // Base Case

&#x20;   ENDIF

&#x20;   sum <- 0

&#x20;   WHILE n > 0 DO

&#x20;       sum <- sum + (n MOD 10)

&#x20;       n <- n DIV 10

&#x20;   ENDWHILE

&#x20;   RETURN DigitalRoot(sum)               // Recursive Case

END



ALGORITHM FastPower(base, n)

// Input: Integer base, non-negative integer exponent n

// Output: base^n using binary exponentiation

BEGIN

&#x20;   IF n == 0 THEN

&#x20;       RETURN 1                          // Base Case

&#x20;   ENDIF

&#x20;   half <- FastPower(base, n DIV 2)      // Recursive Case

&#x20;   IF (n MOD 2) == 0 THEN

&#x20;       RETURN half \* half

&#x20;   ELSE

&#x20;       RETURN base \* half \* half

&#x20;   ENDIF

END

```



\## A2.2 – Call Traces \& Recurrences

\* \*\*Call Stack Trace for `DigitalRoot(9875)` (Deepest Point = 4 Frames):\*\*

&#x20; `DigitalRoot(9875)` $\\rightarrow$ `DigitalRoot(29)` $\\rightarrow$ `DigitalRoot(11)` $\\rightarrow$ `DigitalRoot(2)` (hits `2 < 10`, returns `2`).

\* \*\*Call Stack Trace for `FastPower(2, 5)` (Deepest Point = 4 Frames):\*\*

&#x20; `FastPower(2, 5)` $\\rightarrow$ `FastPower(2, 2)` $\\rightarrow$ `FastPower(2, 1)` $\\rightarrow$ `FastPower(2, 0)` (returns `1` $\\rightarrow$ `2` $\\rightarrow$ `4` $\\rightarrow$ `32`).

\* \*\*Recurrences for Number of Calls T(n):\*\*

&#x20; \* \*\*Linear Power:\*\* T(n) = T(n - 1) + 1, \\(T(0) = 1 \\implies \\mathbf{O(n)}\\) calls (`11` calls for `n = 10`).

&#x20; \* \*\*Halving Power (`FastPower`):\*\* \\(T(n) = T(\\lfloor n / 2 \\rfloor) + 1\\), \\(T(0) = 1 \\implies \\mathbf{O(\\log\_2 n)}\\) calls (`5` calls for `n = 10`).



\## A2.3 – Recursion Risks

\* \*\*Missing Base Case:\*\* Leads to infinite self-invocation until the thread call stack overflows (`Segmentation Fault` / stack overflow crash).

\* \*\*When to Replace Recursion with a Loop:\*\* When recursion depth grows linearly with large input size n (e.g., traversing 50,000 donors), where an iterative loop eliminates O(n) stack frame overhead.



\## Alternative Concept \& Complexity Analysis (Digital Root)



| Implementation | Time Complexity | Stack Space | Notes |

| :--- | :--- | :--- | :--- |

| \*\*Recursive `DigitalRoot`\*\* | \*\*O(d)\*\* (d = digits) | \*\*\\(O(\\log^\* n)\\)\*\* frames | Clear mathematical definition |

| \*\*Iterative `while` Loop\*\* | \*\*O(d)\*\* | \*\*O(1)\*\* | Zero function-call overhead |

| \*\*Formula `1 + (n - 1) % 9`\*\* | \*\*O(1)\*\* (for n > 0) | \*\*O(1)\*\* | Single modulo operation; optimal |



