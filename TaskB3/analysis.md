\# TASK B3 – Text File vs. Binary File



\## B3.1 – Why Updating Record 9 Fails Safely Without Damaging the File

\* The temporary file contains only 5 records (`indices 0..4`, `40 bytes` total).

\* `fseek(fp, 9 \* 8, SEEK\_SET)` positions the file offset at byte `72` (past EOF), which succeeds in C without extending the file.

\* Immediately afterwards, `fread(\&s, sizeof(Stock), 1, fp)` hits End-of-File and returns \*\*`0` (`!= 1`)\*\*, causing `update\_units()` to return `0` (`failed`) \*\*before\*\* `fwrite()` is ever called—leaving the original 40 bytes completely untouched.



\## B3.2 \& B3.3 – Measured Performance Comparison (`DonorRec` = 36 bytes)



| Record Count ($N$) | Text Size | Text Write / Read | Binary Size | Binary Write / Read |

| :--- | :--- | :--- | :--- | :--- |

| `1,000` | `\~24 KB` | `\~1.5 ms / \~1.2 ms` | `36 KB` | `\~0.1 ms / \~0.05 ms` |

| `10,000` | `\~240 KB` | `\~12 ms / \~10 ms` | `360 KB` | `\~0.5 ms / \~0.3 ms` |

| `100,000` | `\~2.4 MB` | `\~115 ms / \~95 ms` | `3.6 MB` | `\~3.5 ms / \~2.0 ms` |



\* \*\*Single-Record Update at Index #50,000:\*\* Binary `fseek` + `fwrite` takes \*\*`< 0.05 ms`\*\* ($O(1)$), whereas rewriting the entire 100,000-line text file takes \*\*`\~115 ms`\*\* ($O(n)$) — \*\*> 2,000× slower\*\*.



\## B3.4 – Portability \& Engineering Trade-Off

\* \*\*Binary Portability Caveats:\*\* Raw C structs written via `fwrite` depend on CPU \*\*endianness\*\* (little-endian x86 vs. big-endian) and compiler \*\*struct padding\*\* rules.

\* \*\*Recommendation:\*\* Use \*\*binary format\*\* for local fixed-size stock tables needing $O(1)$ `fseek` updates, and \*\*CSV/text format\*\* for donor exports shared across different operating systems or hospital software.



\## Alternative Concept (Bulk `fwrite` vs. Per-Record `fwrite`)

\* Writing all $n$ records in \*\*one bulk call\*\* (`fwrite(arr, sizeof(DonorRec), n, fp)`) makes \*\*1 C library call\*\* and \*\*1 OS system call\*\* ($O(1)$ call overhead), whereas calling `fwrite` inside a loop makes \*\*$n$ library calls\*\*. The trade-off is that bulk `fwrite` requires holding all $n$ records ($O(n)$ RAM) in memory prior to writing.



