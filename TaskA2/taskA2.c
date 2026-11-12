#include <stdio.h>

static int calls = 0;

/* Recursive Digital Root for donor ID check digit */
int digital_root_rec(long n)
{
    calls++;
    if (n < 10) return (int)n; /* Base case */
    long sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return digital_root_rec(sum); /* Recursive case */
}

/* O(1) Mathematical Congruence Formula */
int digital_root_formula(long n)
{
    if (n == 0) return 0;
    return (int)(1 + (n - 1) % 9);
}

/* O(log n) Fast Halving Power: base^n = (base^(n/2))^2 */
long long fast_power(long long base, int n)
{
    calls++;
    if (n == 0) return 1; /* Base case */
    long long half = fast_power(base, n / 2); /* Recursive case */
    if (n % 2 == 0) return half * half;
    return base * half * half;
}

/* O(n) Linear Power for comparison */
long long linear_power(long long base, int n)
{
    calls++;
    if (n == 0) return 1;
    return base * linear_power(base, n - 1);
}

int main(void)
{
    long donor_id = 9875;
    calls = 0;
    int dr = digital_root_rec(donor_id);
    printf("DigitalRoot(%ld) = %d (Recursive calls = %d | O(1) Formula = %d)\n",
           donor_id, dr, calls, digital_root_formula(donor_id));

    calls = 0;
    long long p_fast = fast_power(2, 10);
    int fast_calls = calls;

    calls = 0;
    long long p_lin = linear_power(2, 10);
    int lin_calls = calls;

    printf("Power(2, 10) = %lld | Fast Halving Calls = %d vs Linear Calls = %d\n",
           p_fast, fast_calls, lin_calls);
    (void)p_lin;
    return 0;
}
