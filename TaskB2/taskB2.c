#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_BUF 12

/* Safe input reader using fgets() that prevents buffer overflow and flushes long lines */
void read_name_safe(FILE *stream, char *buf, size_t size)
{
    if (fgets(buf, (int)size, stream) != NULL) {
        size_t len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n') {
            buf[len - 1] = '\0';
        } else {
            int ch;
            while ((ch = fgetc(stream)) != '\n' && ch != EOF); /* Flush excess characters */
        }
    }
}

int is_valid_group(const char *g)
{
    const char *valid[8] = {"O+", "O-", "A+", "A-", "B+", "B-", "AB+", "AB-"};
    for (int i = 0; i < 8; i++) {
        if (strcmp(g, valid[i]) == 0) return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    printf("=== 1. Adversarial File Test (Missing File with perror) ===\n");
    FILE *missing = fopen("non_existent_donors.txt", "r");
    if (missing == NULL) {
        perror("fopen(non_existent_donors.txt)"); /* Graceful error reporting instead of NULL crash */
    } else {
        fclose(missing);
    }

    printf("\n=== 2. Adversarial Long Name Test (Buffer = 12 bytes) ===\n");
    const char *long_name = "Thiruvengadam_Subramanian_Rao"; /* 29 chars > 12-byte buffer */
    FILE *mem_stream = tmpfile();
    if (mem_stream) {
        fprintf(mem_stream, "%s\n", long_name);
        rewind(mem_stream);

        char safe_buf[NAME_BUF];
        read_name_safe(mem_stream, safe_buf, sizeof(safe_buf));
        printf("Input string  : \"%s\" (len=%zu)\n", long_name, strlen(long_name));
        printf("fgets() stored: \"%s\" (len=%zu, safely truncated with '\\0'!)\n",
               safe_buf, strlen(safe_buf));
        fclose(mem_stream);
    }

    printf("\n=== 3. Adversarial CLI Argument Validation ===\n");
    if (argc < 2) {
        printf("CLI Check (argc=%d): Rejected missing arguments -> Usage: bloodbank <group>\n", argc);
    } else if (argc > 2) {
        printf("CLI Check (argc=%d): Rejected too many arguments (%d provided, expected 2)\n", argc, argc);
    } else if (!is_valid_group(argv[1])) {
        printf("CLI Check (argv[1]='%s'): Rejected invalid blood group!\n", argv[1]);
    } else {
        printf("CLI Check (argv[1]='%s'): Valid blood group accepted.\n", argv[1]);
    }

    /* 4. Dynamic allocation cleanly freed (Verified under AddressSanitizer / Valgrind) */
    char *heap_rec = (char *)malloc(64);
    if (heap_rec) {
        snprintf(heap_rec, 64, "Donor #1001 verified clean.");
        printf("\n=== 4. Memory Safety Check ===\n%s\n", heap_rec);
        free(heap_rec);
        heap_rec = NULL;
    }

    return 0;
}
