# CSA02 – C Programming Assignment 5 (Parts A & B)
**Case Study: District Blood Bank Network — Design, Pseudocode & Experimental Investigation**

* **Student Name:** Samiksha
* **Register Number:** 1972260036
* **Compiler:** GCC 16.2.0 (`WinLibs x86_64-w64-mingw32`)
* **Compile Command:** `gcc -Wall -Wextra <folder>\<source.c> -o <folder>\<output.exe>`
* **Run Command:** `.\<folder>\<output.exe>`

## Part A – Design & Pseudocode

| Task | Folder | Topic | Summary of Design & Complexity Findings |
| :--- | :--- | :--- | :--- |
| **Task A1** | `TaskA1/` | Function Decomposition | Structure chart, `const`-correct pointer prototypes, and `SearchResult` struct return vs. output pointers. |
| **Task A2** | `TaskA2/` | Recursion & Recurrences | Recursive digital root, $O(\log n)$ halving power, call stack traces, and $O(1)$ congruence formula comparison. |
| **Task A3** | `TaskA3/` | Pointers & CLI | Pointer arithmetic (`*p++`) memory diagram, safe `find_lowest_stock()` pointer return, and `argc`/`argv` CLI validation. |
| **Task A4** | `TaskA4/` | Dynamic Memory (`realloc`) | $O(1)$-amortized doubling donor array (`16` reallocs vs `5,000` for `+10` growth at $n=50,000$) and memory error safeguards. |
| **Task A5** | `TaskA5/` | File Storage (`fseek`) | Append-only `donors.txt` + `64-byte` binary `stock.bin` with $O(1)$ in-place `fseek`/`fwrite` updates. |

## Part B – Implementation & Experimental Investigation

| Task | Folder | Topic | Summary of Experimental Findings |
| :--- | :--- | :--- | :--- |
| **Task B1** | `TaskB1/` | Recursive Instrumentation | Verified `power_fast(2, 30)` takes `6` calls vs `31` linear (`61` if uncached double-branch); documented stack overflow limit vs $O(1)$-space iterative loop. |
| **Task B2** | `TaskB2/` | Adversarial Inputs | Tested oversized names (`fgets` truncation vs `scanf` overflow), missing files (`perror`), invalid CLI args, and clean heap deallocation. |
| **Task B3** | `TaskB3/` | Text vs. Binary Files | Benchmarked `1,000` to `100,000` records; proved binary `fseek` updates record `#50,000` in `< 0.05 ms` ($O(1)$) vs `~115 ms` ($O(n)$) text rewrite. |
| **Task B4** | `TaskB4/` | Production Scaling Decision | Processed `500,000` donors (`100×` district scale) in `6.00 MB` RAM and `< 25 ms` using `realloc` doubling; justified in-memory scan over disk index seeks. |
