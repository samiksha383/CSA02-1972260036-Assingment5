#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct { int group_code; int units; } Stock;

typedef struct {
    int id;
    char name[24];
    char group[4];
    int age;
} DonorRec;

/* Complete the Code: Overwrites the units of record number index directly in the binary file. */
int update_units(FILE *fp, int index, int units)
{
    if (!fp || index < 0) return 0;
    Stock s;
    long offset = (long)index * (long)sizeof(Stock);

    if (fseek(fp, offset, SEEK_SET) != 0) return 0;
    if (fread(&s, sizeof(Stock), 1, fp) != 1) return 0; /* Fails cleanly if index >= record count (EOF) */

    s.units = units;

    if (fseek(fp, offset, SEEK_SET) != 0) return 0;
    if (fwrite(&s, sizeof(Stock), 1, fp) != 1) return 0;
    fflush(fp);
    return 1;
}

void benchmark_formats(int count)
{
    DonorRec *arr = (DonorRec *)malloc((size_t)count * sizeof(DonorRec));
    if (!arr) return;
    for (int i = 0; i < count; i++) {
        arr[i].id = 10000 + i;
        snprintf(arr[i].name, 24, "Donor_%06d", i);
        snprintf(arr[i].group, 4, "O+");
        arr[i].age = 20 + (i % 40);
    }

    /* 1. Text Write & Read */
    clock_t t1 = clock();
    FILE *ft = fopen("bench_donors.txt", "w");
    for (int i = 0; i < count; i++) {
        fprintf(ft, "%d %s %s %d\n", arr[i].id, arr[i].name, arr[i].group, arr[i].age);
    }
    long txt_size = ftell(ft);
    fclose(ft);
    clock_t t2 = clock();

    ft = fopen("bench_donors.txt", "r");
    DonorRec tmp;
    for (int i = 0; i < count; i++) {
        if (fscanf(ft, "%d %23s %3s %d", &tmp.id, tmp.name, tmp.group, &tmp.age) != 4) break;
    }
    fclose(ft);
    clock_t t3 = clock();

    /* 2. Binary Write & Read */
    FILE *fb = fopen("bench_donors.bin", "wb");
    fwrite(arr, sizeof(DonorRec), (size_t)count, fb);
    long bin_size = ftell(fb);
    fclose(fb);
    clock_t t4 = clock();

    fb = fopen("bench_donors.bin", "rb");
    DonorRec *read_buf = (DonorRec *)malloc((size_t)count * sizeof(DonorRec));
    if (read_buf) {
        size_t nread = fread(read_buf, sizeof(DonorRec), (size_t)count, fb);
        (void)nread;
        free(read_buf);
    }
    fclose(fb);
    clock_t t5 = clock();

    double txt_w = 1000.0 * (double)(t2 - t1) / CLOCKS_PER_SEC;
    double txt_r = 1000.0 * (double)(t3 - t2) / CLOCKS_PER_SEC;
    double bin_w = 1000.0 * (double)(t4 - t3) / CLOCKS_PER_SEC;
    double bin_r = 1000.0 * (double)(t5 - t4) / CLOCKS_PER_SEC;

    printf("N = %6d | TXT: %7ld B (W: %6.2f ms, R: %6.2f ms) | BIN: %7ld B (W: %6.2f ms, R: %6.2f ms)\n",
           count, txt_size, txt_w, txt_r, bin_size, bin_w, bin_r);

    /* B3.3: Measure updating 50,000th record in binary (fseek) vs rewriting text file */
    if (count == 100000) {
        clock_t u1 = clock();
        fb = fopen("bench_donors.bin", "r+b");
        DonorRec mod = arr[50000];
        mod.age = 55;
        fseek(fb, 50000L * (long)sizeof(DonorRec), SEEK_SET);
        fwrite(&mod, sizeof(DonorRec), 1, fb);
        fclose(fb);
        clock_t u2 = clock();

        arr[50000].age = 55;
        ft = fopen("bench_donors.txt", "w");
        for (int i = 0; i < count; i++) {
            fprintf(ft, "%d %s %s %d\n", arr[i].id, arr[i].name, arr[i].group, arr[i].age);
        }
        fclose(ft);
        clock_t u3 = clock();

        printf("\n=== B3.3 Single-Record Update (Record #50,000) ===\n");
        printf("Binary fseek in-place update : %.3f ms\n", 1000.0 * (double)(u2 - u1) / CLOCKS_PER_SEC);
        printf("Text full-file rewrite       : %.3f ms\n\n", 1000.0 * (double)(u3 - u2) / CLOCKS_PER_SEC);
    }

    remove("bench_donors.txt");
    remove("bench_donors.bin");
    free(arr);
}

int main(void)
{
    /* 1. Prompt Expected Output Test */
    FILE *fp = tmpfile();
    if (fp == NULL) { perror("tmpfile"); return 1; }
    Stock initial[5] = {{1, 40}, {2, 25}, {3, 12}, {4, 8}, {5, 30}};
    fwrite(initial, sizeof(Stock), 5, fp);

    printf("update record 3: %s\n", update_units(fp, 3, 20) ? "done" : "failed");
    printf("update record 9: %s\n", update_units(fp, 9, 20) ? "done" : "failed");

    rewind(fp);
    Stock s;
    while (fread(&s, sizeof s, 1, fp) == 1) printf("%d:%d ", s.group_code, s.units);
    printf("\n\n");
    fclose(fp);

    /* 2. B3.2 & B3.3 Scaling Benchmarks (1,000, 10,000, 100,000) */
    printf("=== B3.2 Text vs Binary Scaling Benchmark ===\n");
    benchmark_formats(1000);
    benchmark_formats(10000);
    benchmark_formats(100000);

    return 0;
}
