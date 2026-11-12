\# TASK B2 – Engineer Adversarial Inputs



\## 1. Observed Behaviour on Adversarial Inputs

1\. \*\*Oversized Donor Name (`"Thiruvengadam\_Subramanian\_Rao"` into `char buf`):\*\*

&#x20;  \* \*\*`scanf("%s", buf)`:\*\* Writes all 29 characters + `'\\0'` past the 12-byte buffer, corrupting adjacent stack variables and triggering a stack-smashing crash (`STATUS\_STACK\_BUFFER\_OVERRUN`).

&#x20;  \* \*\*`fgets(buf, sizeof(buf), stream)`:\*\* Reads at most `11` characters and guarantees `buf == '\\0'`, safely truncating the name to `"Thiruvengad"` while flushing remaining characters on the line.

2\. \*\*Missing / Read-Only File:\*\* Passing `"non\_existent\_donors.txt"` to `fopen()` returns `NULL`. Checking `if (fp == NULL) { perror(...); }` prints `fopen(non\_existent\_donors.txt): No such file or directory` instead of dereferencing a `NULL` `FILE \*` pointer.

3\. \*\*Invalid CLI Arguments:\*\* Running `taskB2.exe` with `argc == 1` (no args), `argc == 4` (too many args), or `taskB2.exe X+` (invalid group) prints clear error messages and exits cleanly with status `1`.

4\. \*\*Memory Safety (`-fsanitize=address` / Valgrind):\*\* All `malloc`/`realloc` blocks are freed before exit (`0 bytes leaked`, `0 invalid accesses`).



\## 2. Alternative Concept (`getline()` vs. `fgets()`)

\* \*\*How `getline(\&lineptr, \&n, fp)` Works:\*\* Automatically allocates and grows a heap buffer via `realloc()` to fit lines of any length $L$.

\* \*\*Time \& Space:\*\* Reading a line of length $L$ takes \*\*$O(L)$\*\* time (amortized across geometric buffer doublings) and uses \*\*$O(L)$\*\* heap bytes.

\* \*\*Ownership Responsibility:\*\* Unlike `fgets()` (which writes into caller-owned stack memory), \*\*`getline()` allocates heap memory that the caller MUST explicitly `free(lineptr)`\*\* when finished to avoid a memory leak.



