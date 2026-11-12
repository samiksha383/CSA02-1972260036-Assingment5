#include <stdio.h>

long long calls = 0; /* number of recursive calls made */

/* Digital Root (Recursive) */
int digital_root(long long n)
{
    calls++;
    if (n < 10) return (int)n;
    long long sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return digital_root(sum);
}

/* Linear recursion: base^exp = base * base^(exp-1) */
long long power_linear(long long base, int exp)
{
    calls++;
    if (exp == 0) return 1;
    return base * power_linear(base, exp - 1);
}

/* Divide-and-conquer recursion: compute base^(exp/2) once and square it. */
long long power_fast(long long base, int exp)
{
    calls++;
    if (exp == 0) return 1;
    long long half = power_fast(base, exp / 2);
    if (exp % 2 == 0) return half * half;
    return base * half * half;
}

/* Unoptimized double-branch power for comparison */
long long power_double_branch(long long base, int exp)
{
    calls++;
    if (exp == 0) return 1;
    if (exp % 2 == 0) {
        return power_double_branch(base, exp / 2) * power_double_branch(base, exp / 2);
    }
    return base * power_double_branch(base, exp / 2) * power_double_branch(base, exp / 2);
}

/* Iterative square-and-multiply (O(log n) time, O(1) stack space) */
long long power_iterative(long long base, int exp)
{
    long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) result *= base;
        base *= base;
        exp /= 2;
    }
    return result;
}

int main(void)
{
    /* 1. Prompt Expected Output Test */
    calls = 0;
    long long a = power_linear(2, 30);
    printf("linear: %lld in %lld calls\n", a, calls);

    calls = 0;
    long long b = power_fast(2, 30);
    printf("fast  : %lld in %lld calls\n", b, calls);

    calls = 0;
    long long c = power_double_branch(2, 30);
    printf("double-branch (2^30): %lld in %lld calls!\n\n", c, calls);

    /* 2. B1.1 Validation Tests (ordinary, 0, 1, custom) */
    printf("=== B1.1 Validation Table ===\n");
    long long dr_tests[4] = {0, 1, 9, 9875};
    for (int i = 0; i < 4; i++) {
        calls = 0;
        int r = digital_root(dr_tests[i]);
        printf("digital_root(%4lld) = %d (calls = %lld)\n", dr_tests[i], r, calls);
    }
    printf("power_fast(5, 0) = %lld | power_fast(5, 1) = %lld | power_fast(3, 5) = %lld\n\n",
           power_fast(5, 0), power_fast(5, 1), power_fast(3, 5));

    /* 3. B1.3 Call Count Scaling (n = 10, 100, 1000, 10000) */
    printf("=== B1.3 Call Counts vs Recurrence Predictions ===\n");
    int ns[4] = {10, 100, 1000, 10000};
    for (int i = 0; i < 4; i++) {
        int n = ns[i];
        calls = 0;
        power_linear(1, n);
        long long lin_calls = calls;

        calls = 0;
        power_fast(1, n);
        long long fast_calls = calls;

        printf("n = %5d -> Linear Calls = %6lld (T(n)=n+1) | Fast Calls = %2lld (T(n)=floor(log2 n)+2)\n",
               n, lin_calls, fast_calls);
    }

    return 0;
}
