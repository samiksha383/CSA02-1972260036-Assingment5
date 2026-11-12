\# TASK A5 – Design the File Storage



\## A5.1 – Justification of File Formats

\* \*\*Donors as Text File (`donors.txt` via `fprintf` / `fgets`):\*\* Donor records have variable-length names, are human-readable for hospital staff audits or CSV/Excel exports, and are appended sequentially (`"a"` mode) without requiring fixed-offset random overwrites.

\* \*\*Stock as Binary File (`stock.bin` via `fwrite` / `fread`):\*\* There are always exactly \*\*8 fixed-size records\*\* (`struct StockRecord { char group; int units; }` = `8 bytes` each). Binary format eliminates text parsing and guarantees constant record width, enabling direct `fseek()` random-access updates.



\## A5.2 – In-Place Binary Update Pseudocode (`fseek`, `fread`, `fwrite`)



```text

ALGORITHM UpdateStockInPlace(filename, group\_idx, delta)

// Input: Binary stock file path, blood group index (0..7), unit change (+ or -)

// Output: TRUE on success, FALSE on file error or insufficient stock

BEGIN

&#x20;   fp <- FOPEN(filename, "r+b")

&#x20;   IF fp == NULL THEN RETURN FALSE ENDIF



&#x20;   offset <- group\_idx \* SIZEOF(StockRecord)

&#x20;   FSEEK(fp, offset, SEEK\_SET)

&#x20;   FREAD(\&rec, SIZEOF(StockRecord), 1, fp)



&#x20;   IF rec.units + delta < 0 THEN

&#x20;       FCLOSE(fp)

&#x20;       RETURN FALSE                      // Reject negative stock

&#x20;   ENDIF



&#x20;   rec.units <- rec.units + delta

&#x20;   FSEEK(fp, offset, SEEK\_SET)           // Reposition file pointer back to record start

&#x20;   FWRITE(\&rec, SIZEOF(StockRecord), 1, fp)

&#x20;   FCLOSE(fp)

&#x20;   RETURN TRUE

END

```



\* \*\*Why In-Place Update is Difficult in a Text File:\*\* Text records have variable byte lengths (e.g., changing `"O+ 9\\n"` to `"O+ 10\\n"` adds 1 byte). Overwriting in place shifts or corrupts the first character of the next blood group's line.



\## Alternative Concept \& Complexity Analysis (Full Text Rewrite vs. Binary `fseek`)



| Metric | Binary In-Place (`fseek` + `fwrite`) | Full Text File Rewrite |

| :--- | :--- | :--- |

| \*\*Time per Update (n records)\*\* | \*\*O(1)\*\* (1 seek + 8-byte write) | \*\*O(n)\*\* (Must format and rewrite all n lines) |

| \*\*File Size (8 Blood Groups)\*\* | \*\*64 bytes\*\* (`8 × 8 bytes`, exact) | \*\*\~72–96 bytes\*\* (Depends on digit lengths \& newlines) |

| \*\*Data Integrity Risk\*\* | Minimal (single atomic sector write) | High (power failure mid-rewrite truncates the entire file) |



