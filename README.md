# COM2113 Homework: Assembly 7–9

## Objective

Implement six x86-64 functions in AT&T syntax. The assignment practices the material from Assembly Parts 1–3:

- integer and pointer arguments in registers;
- additional arguments on the stack;
- return values in `%rax`;
- signed and unsigned comparisons;
- labels and conditional jumps;
- loops;
- stack frames and function calls;
- recursion.

## File you may edit

Edit only:

```text
hw79.s
```

Do not edit the header, tests, helper functions, source checker, or Makefile.

## Build and test

Run everything:

```bash
make test
```

Run one problem:

```bash
make test-p1
make test-p2
make test-p3
make test-p4
make test-p5
make test-p6
```

Run only the source checker:

```bash
make check-source
```

## Assembly rules

The solution may use only these lecture instructions:

```text
movq  addq  subq  imulq  cmpq
jmp   je    jne   jl      jle
jg    jge   jb    jbe     ja    jae
pushq call  leave ret
```

Use ordinary memory operands such as `8(%rsp)` and `-8(%rbp)` where needed.

Not allowed:

- `leaq`;
- `cmov*` or `set*`;
- the `loop` instruction;
- division instructions;
- library calls other than the calls explicitly required by Problems 5 and 6;
- writable global data;
- C implementations of the required functions.

The source checker uses an instruction whitelist. An instruction not listed above will fail `make check-source`.

---

## Problem 1 — Signed and unsigned relation code

Implement:

```c
long relation_code(long x, long y);
```

Begin with a result of `0`.

- If signed `x < y`, add `1` to the result.
- Independently, if unsigned `x < y`, add `2` to the result.
- Return the result.

The same 64-bit patterns are interpreted twice: once as signed values and once as unsigned values.

| Signed comparison | Unsigned comparison | Result |
|---|---|---:|
| false | false | `0` |
| true | false | `1` |
| false | true | `2` |
| true | true | `3` |

### Example 1

```text
relation_code(3, 5) -> 3
```

- Signed: `3 < 5`, so the result gains `1`.
- Unsigned: `3 < 5`, so the result gains `2`.
- Final result: `1 + 2 = 3`.

### Example 2

```text
relation_code(-1, 1) -> 1
```

- Signed: `-1 < 1`, so the result gains `1`.
- Unsigned: the all-ones bit pattern for `-1` is the largest unsigned 64-bit value, so it is not less than unsigned `1`.
- Final result: `1`.

### Example 3

```text
relation_code(1, -1) -> 2
```

- Signed: `1 < -1` is false.
- Unsigned: unsigned `1` is less than the all-ones bit pattern used by `-1`, so the result gains `2`.
- Final result: `2`.

### Example 4

```text
relation_code(7, 7) -> 0
```

The values are equal under both interpretations. Neither less-than comparison is true, so the result remains `0`.

---

## Problem 2 — Clamp a signed value

Implement:

```c
long clamp_long(long x, long low, long high);
```

Precondition: `low <= high` using signed interpretation.

Return:

- `low` when `x < low`;
- `high` when `x > high`;
- otherwise, `x`.

All comparisons are signed.

### Example 1

```text
clamp_long(-10, -4, 12) -> -4
```

The valid interval is `[-4, 12]`. Since `-10` is below the interval, the nearest allowed value is `-4`.

### Example 2

```text
clamp_long(5, -4, 12) -> 5
```

`5` is between `-4` and `12`, so it is returned unchanged.

### Example 3

```text
clamp_long(99, -4, 12) -> 12
```

`99` is above the interval, so return the upper endpoint `12`.

### Example 4

```text
clamp_long(8, 3, 3) -> 3
```

The interval `[3, 3]` contains only one possible value. Any input clamps to `3`.

---

## Problem 3 — Eight-argument weighted sum

Implement:

```c
long weighted_sum8(long a, long b, long c, long d,
                   long e, long f, long g, long h);
```

Return:

```text
a + 2b + 3c + 4d + 5e + 6f + 7g + 8h
```

The first six integer arguments use the argument registers from lecture. The seventh and eighth arguments are supplied on the stack by the caller.

### Example 1

```text
weighted_sum8(1, 2, 3, 4, 5, 6, 7, 8) -> 204
```

```text
1*1 = 1
2*2 = 4
3*3 = 9
4*4 = 16
5*5 = 25
6*6 = 36
7*7 = 49
8*8 = 64
```

Adding the terms gives `1 + 4 + 9 + 16 + 25 + 36 + 49 + 64 = 204`.

### Example 2

```text
weighted_sum8(0, 0, 0, 0, 0, 0, 13, 17) -> 227
```

The first six terms are zero. The remaining terms are:

```text
7*13 = 91
8*17 = 136
91 + 136 = 227
```

### Example 3

```text
weighted_sum8(-1, 1, -1, 1, -1, 1, -1, 1) -> 4
```

```text
-1 + 2 - 3 + 4 - 5 + 6 - 7 + 8 = 4
```

---

## Problem 4 — Alternating sum

Implement:

```c
long alternating_sum(long n);
```

Precondition: `n >= 0`.

Return the sum of the integers from `1` through `n`, adding odd terms and subtracting even terms:

```text
1 - 2 + 3 - 4 + 5 - 6 + ...
```

When `n == 0`, return `0`.

### Example 1

```text
alternating_sum(0) -> 0
```

There are no terms, so the total is `0`.

### Example 2

```text
alternating_sum(5) -> 3
```

```text
1 - 2 + 3 - 4 + 5
= -1 + 3 - 4 + 5
= 2 - 4 + 5
= -2 + 5
= 3
```

### Example 3

```text
alternating_sum(6) -> -3
```

The first five terms total `3`. The sixth term is even, so subtract `6`:

```text
3 - 6 = -3
```

### Example 4

```text
alternating_sum(10) -> -5
```

Pair neighboring terms:

```text
(1 - 2) + (3 - 4) + (5 - 6) + (7 - 8) + (9 - 10)
= -1 + -1 + -1 + -1 + -1
= -5
```

The submitted function must use a loop built from labels, comparisons, and jumps.

---

## Problem 5 — Recursive integer power

Implement:

```c
long recursive_power(long base, long exponent);
```

Precondition: `exponent >= 0`, and all tested results fit in a signed `long`.

Use these mathematical rules:

```text
base^0 = 1
base^exponent = base * base^(exponent - 1), when exponent > 0
```

The submitted function must actually call `recursive_power` recursively.

### Example 1

```text
recursive_power(2, 3) -> 8
```

```text
2^3 = 2 * 2^2
    = 2 * 2 * 2^1
    = 2 * 2 * 2 * 2^0
    = 2 * 2 * 2 * 1
    = 8
```

### Example 2

```text
recursive_power(-2, 4) -> 16
```

```text
(-2) * (-2) * (-2) * (-2) = 16
```

There are an even number of negative factors, so the result is positive.

### Example 3

```text
recursive_power(-2, 3) -> -8
```

```text
(-2) * (-2) * (-2) = -8
```

There are an odd number of negative factors, so the result is negative.

### Example 4

```text
recursive_power(37, 0) -> 1
```

Any tested base raised to exponent zero returns `1`.

---

## Problem 6 — Call two helper functions

Implement:

```c
long combine_helpers(long x, long y);
```

The provided helpers behave as follows:

```c
helper_square(x) = x * x
helper_affine(y) = 3 * y + 7
```

Your function must call each helper exactly once and return:

```text
helper_square(x) + helper_affine(y)
```

The helpers may overwrite argument and temporary registers. Values needed after a call must therefore be kept somewhere safe. Maintain the stack alignment convention from lecture before each `call`.

### Example 1

```text
combine_helpers(4, 5) -> 38
```

```text
helper_square(4) = 4 * 4 = 16
helper_affine(5) = 3 * 5 + 7 = 22
16 + 22 = 38
```

### Example 2

```text
combine_helpers(-9, -4) -> 76
```

```text
helper_square(-9) = (-9) * (-9) = 81
helper_affine(-4) = 3 * (-4) + 7 = -5
81 + (-5) = 76
```

### Example 3

```text
combine_helpers(0, 0) -> 7
```

```text
helper_square(0) = 0
helper_affine(0) = 7
0 + 7 = 7
```
