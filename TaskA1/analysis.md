\# TASK A1 – Decompose the Program into Functions



\## A1.1 – Function Decomposition \& Structure Chart

The District Blood Bank Network is modularized into cohesive, single-purpose functions:

\* `main()`: Parses CLI commands, initializes files, dispatches operations, and saves state before exit.

\* `load\_donors()` / `save\_donors()`: Reads and writes donor records to `donors.txt`.

\* `load\_stock()` / `save\_stock()`: Reads and writes the 8 blood-group stock records to `stock.bin`.

\* `register\_donor()`: Validates donor fields, computes the ID check digit, and appends to the growable donor list.

\* `is\_eligible()`: Checks whether a donor is aged `18–65` and has waited at least `90` days since their last donation.

\* `find\_eligible\_donors()`: Filters donors matching a requested blood group and eligibility, returning both the count and the youngest donor's ID.

\* `update\_stock()`: Increments or decrements available units for a blood group while preventing negative inventory.

\* `find\_lowest\_stock()`: Scans the 8 blood groups and returns a pointer to the lowest-stock unit.



```text

&#x20;                              +------------------+



&#x20;                              |      main()      |

&#x20;                              +------------------+

&#x20;              \_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_/   |   \\\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_

&#x20;             /                         |                         \\

&#x20;            v                          v                          v

+------------------------+  +------------------------+  +------------------------+



|   File Persistence     |  |    Donor Management    |  |    Stock Management    |

+------------------------+  +------------------------+  +------------------------+



| - load\_donors()        |  | - register\_donor()     |  | - update\_stock()       |

| - save\_donors()        |  | - find\_eligible()      |  | - find\_lowest\_stock()  |

| - load\_stock()         |  +------------------------+  +------------------------+

| - save\_stock()         |              |

+------------------------+              v

&#x20;                           +------------------------+



&#x20;                           |     is\_eligible()      |

&#x20;                           +------------------------+

```



\## A1.2 – Function Prototypes \& Parameter Passing Justification



```c

int load\_donors(const char \*path, Donor \*\*list, size\_t \*count, size\_t \*cap);

int save\_donors(const char \*path, const Donor \*list, size\_t count);

int register\_donor(Donor \*\*list, size\_t \*count, size\_t \*cap, const Donor \*new\_d);

int is\_eligible(const Donor \*d, Date today);

SearchResult find\_eligible\_donors(const Donor \*list, size\_t count, const char \*group, Date today);

int update\_stock(int stock, int group\_idx, int delta);

int \*find\_lowest\_stock(int \*stock, int n);

```



\* \*\*Pass by Value (`Date today`, `size\_t count`, `int group\_idx`, `int delta`):\*\* Small scalar values (`<= 12 bytes`) fit in CPU registers and cannot be accidentally modified by the callee.

\* \*\*Pass by Pointer (`Donor \*\*list`, `size\_t \*count`, `size\_t \*cap`, `int stock`):\*\* Required when the function must mutate caller state (such as updating the base address on `realloc()`, incrementing `count`, or modifying `stock`).

\* \*\*Pass by `const` Pointer (`const char \*path`, `const Donor \*list`, `const Donor \*new\_d`):\*\* Avoids copying multi-byte structures (`\~80 bytes`) while enforcing compile-time read-only safety.



\## A1.3 – Returning More Than One Value (Count \& Youngest Donor ID)

\* \*\*Alternative 1 (Output Pointer Parameters):\*\*

&#x20; ```c

&#x20; void find\_eligible\_ptr(const Donor \*list, size\_t count, const char \*group,

&#x20;                        Date today, int \*out\_count, int \*out\_youngest\_id);

&#x20; ```

\* \*\*Alternative 2 (Returning a Structure):\*\*

&#x20; ```c

&#x20; typedef struct {

&#x20;     int eligible\_count;

&#x20;     int youngest\_donor\_id;

&#x20; } SearchResult;



&#x20; SearchResult find\_eligible\_struct(const Donor \*list, size\_t count, const char \*group, Date today);

&#x20; ```

\* \*\*Comparison:\*\* \*\*Returning a `SearchResult` struct\*\* is superior because callers cannot pass `NULL` output pointers, both related values stay bundled atomically, and on 64-bit architectures an 8-byte struct returns directly inside a single CPU register (`RAX`) with zero extra memory writes.



\## Alternative Concept \& Complexity Analysis (Global Variables vs. Parameters)

\* \*\*Time \& Space:\*\* Algorithmic running time (\*\*O(n)\*\*) and total memory consumption remain unchanged.

\* \*\*Coupling, Testing \& Reuse:\*\* Global variables create \*\*tight coupling\*\*—the program cannot track separate stock arrays for multiple hospital branches simultaneously, unit tests must manually reset hidden global state before each run, and any function can accidentally corrupt shared records. Parameterized interfaces isolate state and enable clean multi-hospital reuse.



