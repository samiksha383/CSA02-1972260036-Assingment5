\# TASK A3 – Design with Pointers



\## A3.1 – Pointer Expressions \& Memory Diagram

Given `int stock` (`sizeof(int) == 4` bytes) and `int \*p = stock;` starting at `0x1000`:

\* \*\*`p + 3`:\*\* Pointer arithmetic advancing 3 elements (`0x1000 + 3 × 4 = 0x100C`), yielding \*\*`\&stock`\*\*.

\* \*\*`\*(p + 3)`:\*\* Dereferences `0x100C`, yielding the integer value \*\*`stock`\*\* (`9`).

\* \*\*`p`:\*\* Equivalent to `\*(p + 3)`, yielding \*\*`stock`\*\* (`9`).

\* \*\*`\*p++`:\*\* Post-increment `++` has higher precedence than unary `\*`; it yields \*\*`\*p` (`stock`, `25`)\*\* and advances `p` to \*\*`0x1004` (`\&stock`)\*\*.



```text

Address:   0x1000   0x1004   0x1008   0x100C   0x1010   0x1014   0x1018   0x101C

&#x20;         +--------+--------+--------+--------+--------+--------+--------+--------+

stock\[]:  |   25   |    4   |   18   |    9   |   22   |    6   |   14   |    3   |

&#x20;         +--------+--------+--------+--------+--------+--------+--------+--------+

&#x20;            ^                          ^



&#x20;            |                          |

&#x20;     p (initially)                   p + 3

```



\## A3.2 – Returning a Pointer Safely vs. Local Variable Dangling Pointer

\* `find\_lowest\_stock(int \*stock, int n)` returns `\&stock\[min\_idx]`, which points into the \*\*caller's array\*\*, allowing `\*low\_ptr += 10;` to update inventory directly.

\* \*\*Why Returning a Local Variable Pointer is Wrong:\*\* A local variable resides inside the function's stack frame, which is reclaimed immediately when the function returns. Dereferencing that address causes Undefined Behaviour.



\## A3.3 – Command-Line Interface (`argc`, `argv`)

\* Supports `bloodbank find <group>` (`argc == 3`) and `bloodbank issue <group> <units>` (`argc == 4`).

\* Rejects wrong argument counts, unrecognized blood groups, non-numeric quantities (via `strtol`), negative/zero units, and over-issuing beyond available stock.



\## Alternative Concept \& Complexity Analysis (Index vs. Pointer Return)

\* \*\*Index Calling Code:\*\* `int idx = find\_lowest\_idx(stock, 8); stock\[idx] += 10;`

\* \*\*Time \& Space:\*\* Both run in \*\*O(n)\*\* time and \*\*O(1)\*\* space.

\* \*\*Safety on `realloc()`:\*\* \*\*Returning an integer index (`idx`) is safer\*\* if the underlying array is dynamically resized via `realloc()`, because `realloc()` may relocate the array to a new heap address—making a saved element pointer (`low\_ptr`) a \*\*dangling pointer\*\*, whereas index `idx` remains valid relative to the updated base pointer.



