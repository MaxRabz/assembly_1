#include "../hw79.h"

#include <limits.h>
#include <stdio.h>
#include <string.h>

static int failures;

#define CHECK_EQ(label, actual_expression, expected_expression)                  \
    do {                                                                         \
        long actual_value = (actual_expression);                                 \
        long expected_value = (expected_expression);                             \
        if (actual_value != expected_value) {                                    \
            fprintf(stderr, "%s: expected %ld, got %ld\n",                    \
                    (label), expected_value, actual_value);                       \
            failures++;                                                          \
        }                                                                         \
    } while (0)

static void test_p1(void) {
    CHECK_EQ("p1 equal", relation_code(7, 7), 0);
    CHECK_EQ("p1 both", relation_code(3, 5), 3);
    CHECK_EQ("p1 signed only", relation_code(-1, 1), 1);
    CHECK_EQ("p1 unsigned only", relation_code(1, -1), 2);
    CHECK_EQ("p1 min max", relation_code(LONG_MIN, LONG_MAX), 1);
    CHECK_EQ("p1 max min", relation_code(LONG_MAX, LONG_MIN), 2);
    CHECK_EQ("p1 negatives", relation_code(-9, -3), 3);
}

static void test_p2(void) {
    CHECK_EQ("p2 below", clamp_long(-10, -4, 12), -4);
    CHECK_EQ("p2 inside", clamp_long(5, -4, 12), 5);
    CHECK_EQ("p2 above", clamp_long(99, -4, 12), 12);
    CHECK_EQ("p2 low edge", clamp_long(-4, -4, 12), -4);
    CHECK_EQ("p2 high edge", clamp_long(12, -4, 12), 12);
    CHECK_EQ("p2 negative interval", clamp_long(-20, -12, -2), -12);
    CHECK_EQ("p2 singleton", clamp_long(8, 3, 3), 3);
}

static long reference_weighted(long a, long b, long c, long d,
                               long e, long f, long g, long h) {
    return a + 2*b + 3*c + 4*d + 5*e + 6*f + 7*g + 8*h;
}

static void test_p3(void) {
    CHECK_EQ("p3 sequence", weighted_sum8(1,2,3,4,5,6,7,8),
             reference_weighted(1,2,3,4,5,6,7,8));
    CHECK_EQ("p3 signed", weighted_sum8(-4,9,-2,7,0,-3,11,-8),
             reference_weighted(-4,9,-2,7,0,-3,11,-8));
    CHECK_EQ("p3 stack arguments", weighted_sum8(0,0,0,0,0,0,13,17),
             7*13 + 8*17);
    CHECK_EQ("p3 register arguments", weighted_sum8(9,8,7,6,5,4,0,0),
             reference_weighted(9,8,7,6,5,4,0,0));
}

static long reference_alternating(long n) {
    long total = 0;
    long i = 1;
    long add_term = 1;
    while (i <= n) {
        if (add_term) total += i;
        else total -= i;
        add_term = !add_term;
        i++;
    }
    return total;
}

static void test_p4(void) {
    const long cases[] = {0,1,2,3,4,5,6,10,31,100,999};
    for (unsigned long i = 0; i < sizeof(cases)/sizeof(cases[0]); i++) {
        char label[64];
        snprintf(label, sizeof(label), "p4 n=%ld", cases[i]);
        CHECK_EQ(label, alternating_sum(cases[i]),
                 reference_alternating(cases[i]));
    }
}

static long reference_power(long base, long exponent) {
    long result = 1;
    while (exponent > 0) {
        result *= base;
        exponent--;
    }
    return result;
}

static void test_p5(void) {
    const long bases[] = {0,1,-1,2,-2,3,5,-7};
    const long exponents[] = {0,1,2,3,4,5,6,3};
    for (unsigned long i = 0; i < sizeof(bases)/sizeof(bases[0]); i++) {
        char label[64];
        snprintf(label, sizeof(label), "p5 %ld^%ld", bases[i], exponents[i]);
        CHECK_EQ(label, recursive_power(bases[i], exponents[i]),
                 reference_power(bases[i], exponents[i]));
    }
}

static void test_p6(void) {
    CHECK_EQ("p6 basic", combine_helpers(4, 5), 38);
    CHECK_EQ("p6 negative", combine_helpers(-9, -4), 76);
    CHECK_EQ("p6 zero", combine_helpers(0, 0), 7);
    CHECK_EQ("p6 distinct arguments", combine_helpers(12, 3), 160);
}

static int run_named(const char *name) {
    if (strcmp(name, "p1") == 0) test_p1();
    else if (strcmp(name, "p2") == 0) test_p2();
    else if (strcmp(name, "p3") == 0) test_p3();
    else if (strcmp(name, "p4") == 0) test_p4();
    else if (strcmp(name, "p5") == 0) test_p5();
    else if (strcmp(name, "p6") == 0) test_p6();
    else {
        fprintf(stderr, "unknown test group: %s\n", name);
        return 2;
    }
    return failures == 0 ? 0 : 1;
}

int main(int argc, char **argv) {
    if (argc == 2) {
        int result = run_named(argv[1]);
        if (result == 0) printf("%s passed\n", argv[1]);
        return result;
    }

    test_p1();
    test_p2();
    test_p3();
    test_p4();
    test_p5();
    test_p6();

    if (failures != 0) {
        fprintf(stderr, "%d test(s) failed\n", failures);
        return 1;
    }

    puts("all functional tests passed");
    return 0;
}
