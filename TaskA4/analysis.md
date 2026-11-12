\# TASK A4 – Design Dynamic Memory Use



\## A4.1 – Growable Donor List (`realloc` Doubling Strategy)



```text

ALGORITHM RegisterDonor(list, count, capacity, new\_donor)

// Input: Pointer to donor array pointer (list), current count, capacity, new\_donor record

// Output: TRUE if added, FALSE if memory allocation fails

BEGIN

&#x20;   IF count == capacity THEN

&#x20;       new\_cap <- (capacity == 0) ? 16 : (capacity \* 2)

&#x20;       temp <- REALLOC(list, new\_cap \* SIZEOF(Donor))

&#x20;       IF temp == NULL THEN

&#x20;           PRINT "Error: Out of memory; existing donor list preserved"

&#x20;           RETURN FALSE                  // Critical: original 'list' is NOT lost!

&#x20;       ENDIF

&#x20;       list <- temp

&#x20;       capacity <- new\_cap

&#x20;   ENDIF

&#x20;   list\[count] <- new\_donor

&#x20;   count <- count + 1

&#x20;   RETURN TRUE

END

```



\## A4.2 – Three Memory Errors \& Prevention in Blood Bank Program

1\. \*\*Memory Leak:\*\*

&#x20;  \* \*How it happens:\* Writing `list = realloc(list, new\_size);` directly when `realloc` fails (`NULL`) overwrites the only pointer to the existing donor block, leaking all previously allocated records; or exiting `main()` without calling `free(list)`.

&#x20;  \* \*Prevention:\* Always assign `realloc` to a temporary pointer `temp` first, and call `free(list)` on every exit path.

2\. \*\*Dangling Pointer:\*\*

&#x20;  \* \*How it happens:\* Storing `Donor \*youngest = \&list;` before calling `RegisterDonor()`, which triggers `realloc()` and moves `list` to a new heap address. Accessing `youngest->name` afterwards reads freed memory.

&#x20;  \* \*Prevention:\* Store integer indices (`int youngest\_idx`) instead of raw element pointers across `realloc()` calls.

3\. \*\*Double Free:\*\*

&#x20;  \* \*How it happens:\* Calling `free(list)` inside `save\_donors()` and calling `free(list)` again during program shutdown in `main()`.

&#x20;  \* \*Prevention:\* Transfer ownership clearly to `main()` and immediately set `list = NULL;` right after `free(list);`.



\## Alternative Concept \& Complexity Analysis (Fixed `+10` vs. `×2` Doubling)



| Strategy | Reallocations for n = 50,000 | Total Elements Copied | Max Unused Capacity |

| :--- | :--- | :--- | :--- |

| \*\*Doubling (`capacity \* 2`)\*\* | \*\*16\*\* (\\(\\lceil\\log\_2 n\\rceil\\)) | \\(1 + 2 + 4 + \\dots + n < 2n \\implies \\mathbf{O(n)}\\) (\*\*O(1)\*\* amortized/insert) | Up to \*\*n - 1 records\*\* (`< 50%`) |

| \*\*Fixed Increment (`capacity + 10`)\*\* | \*\*5,000\*\* (\\(\\lfloor n / 10 \\rfloor\\)) | \\(10 + 20 + \\dots + n = \\frac{n(n+10)}{20} \\implies \\mathbf{O(n^2)}\\) (`\~125,000,000` copies!) | At most \*\*9 records\*\* (O(1)) |



\*\*Conclusion:\*\* Doubling reduces total element copies from \*\*125 million\*\* ($O(n^2)$) down to \*\*< 100,000\*\* ($O(n)$) at district scale.



