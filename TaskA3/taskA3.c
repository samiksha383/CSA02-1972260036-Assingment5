#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *GROUPS = {"O+", "O-", "A+", "A-", "B+", "B-", "AB+", "AB-"};

/* A3.2: Returns a pointer to the lowest-stock element so caller can update directly */
int *find_lowest_stock(int *stock, int n)
{
    if (!stock || n <= 0) return NULL;
    int *lowest = &stock;
    for (int i = 1; i < n; i++) {
        if (stock[i] < *lowest) {
            lowest = &stock[i];
        }
    }
    return lowest;
}

int find_group_index(const char *grp)
{
    for (int i = 0; i < 8; i++) {
        if (strcmp(GROUPS[i], grp) == 0) return i;
    }
    return -1;
}

/* A3.3: Command-Line Interface handler */
int main(int argc, char *argv[])
{
    int stock = {25, 4, 18, 9, 22, 6, 14, 3}; /* AB- has lowest stock (3) */

    if (argc >= 2) {
        if (strcmp(argv, "find") == 0 && argc == 3) {
            int idx = find_group_index(argv);
            if (idx < 0) {
                fprintf(stderr, "Error: Invalid blood group '%s'.\n", argv);
                return 1;
            }
            printf("Group %s has %d units available.\n", argv, stock[idx]);
            return 0;
        } else if (strcmp(argv, "issue") == 0 && argc == 4) {
            int idx = find_group_index(argv);
            char *endptr = NULL;
            long units = strtol(argv, &endptr, 10);
            if (idx < 0 || *endptr != '\0' || units <= 0) {
                fprintf(stderr, "Error: Invalid group or unit count.\n");
                return 1;
            }
            if (stock[idx] < units) {
                fprintf(stderr, "Error: Insufficient stock for %s (%d available).\n",
                        argv, stock[idx]);
                return 1;
            }
            stock[idx] -= (int)units;
            printf("Issued %ld units of %s. Remaining: %d\n", units, argv, stock[idx]);
            return 0;
        } else {
            fprintf(stderr, "Usage:\n  %s find <group>\n  %s issue <group> <units>\n",
                    argv, argv);
            return 1;
        }
    }

    /* Default demonstration when run without CLI args */
    int *p = stock;
    printf("Pointer demo: *(p + 3) = %d, p = %d\n", *(p + 3), p);

    int *low_ptr = find_lowest_stock(stock, 8);
    int low_idx = (int)(low_ptr - stock);
    printf("Lowest stock group: %s (%d units at address %p)\n",
           GROUPS[low_idx], *low_ptr, (void *)low_ptr);

    *low_ptr += 10; /* Emergency restock directly via returned pointer */
    printf("After emergency restock via pointer: %s = %d units\n",
           GROUPS[low_idx], stock[low_idx]);
    return 0;
}
