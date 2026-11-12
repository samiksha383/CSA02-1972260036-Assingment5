#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int id;
    char group[4];
    int days_since_last_donation;
} Donor;

typedef struct {
    Donor *data;
    int size;
    int capacity;
} DonorList;

/* Complete the Code: Growable donor list using realloc doubling */
int add_donor(DonorList *list, int id, const char *group)
{
    if (list->size == list->capacity) {
        int new_cap = (list->capacity == 0) ? 2 : (list->capacity * 2);
        Donor *p = (Donor *)realloc(list->data, (size_t)new_cap * sizeof(Donor));
        if (p == NULL) {
            return 0; /* Old list->data remains intact and leak-free */
        }
        list->data = p;
        list->capacity = new_cap;
    }
    list->data[list->size].id = id;
    strncpy(list->data[list->size].group, group, 3);
    list->data[list->size].group[3] = '\0';
    list->data[list->size].days_since_last_donation = 85 + (id % 15); /* Eligible within next 7 days if >= 83 */
    list->size++;
    return 1;
}

int main(void)
{
    /* 1. Prompt Expected Output Test */
    DonorList list = {NULL, 0, 0};
    const char *groups[5] = {"O+", "A+", "B-", "AB+", "O-"};
    for (int i = 0; i < 5; i++) {
        add_donor(&list, 7001 + i, groups[i]);
        printf("added %d (%s): size = %d, capacity = %d\n",
               list.data[i].id, list.data[i].group, list.size, list.capacity);
    }
    free(list.data);

    /* 2. B4.1 Nightly Report Simulation at 100x Scale (25 hospitals x 20,000 = 500,000 donors) */
    printf("\n=== B4.1 District Scale Nightly Eligibility Report (500,000 Donors) ===\n");
    clock_t t1 = clock();
    DonorList district = {NULL, 0, 0};
    int total_donors = 500000;

    for (int i = 0; i < total_donors; i++) {
        if (!add_donor(&district, 100000 + i, groups[i % 5])) {
            fprintf(stderr, "Allocation failed at %d\n", i);
            free(district.data);
            return 1;
        }
    }

    int eligible_next_week = 0;
    for (int i = 0; i < district.size; i++) {
        if (district.data[i].days_since_last_donation >= 83) {
            eligible_next_week++;
        }
    }
    clock_t t2 = clock();

    double mem_mb = (double)((size_t)district.capacity * sizeof(Donor)) / (1024.0 * 1024.0);
    double ms = 1000.0 * (double)(t2 - t1) / CLOCKS_PER_SEC;

    printf("Loaded & Filtered: %d donors | Eligible Next Week: %d\n",
           district.size, eligible_next_week);
    printf("Allocated Capacity: %d slots | Peak Heap Memory: %.2f MB | Time: %.2f ms\n",
           district.capacity, mem_mb, ms);

    /* B4.2 Clean Memory Release */
    free(district.data);
    district.data = NULL;
    printf("Memory Safety: All heap blocks freed cleanly.\n");
    return 0;
}
