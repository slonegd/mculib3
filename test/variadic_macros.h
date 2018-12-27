#pragma once

/* This counts the number of args */
#define NARGS_SEQ(_1,_2,_3,_4,_5,_6,_7,_8,N,...) N
#define NARGS(...) NARGS_SEQ(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)

/* This will let macros expand before concating them */
#define PRIMITIVE_CAT(x, y) x ## y
#define CAT(x, y) PRIMITIVE_CAT(x, y)

/* This will pop the last argument off */
#define POP_LAST(...) CAT(POP_LAST_, NARGS(__VA_ARGS__))(__VA_ARGS__)
#define POP_LAST_1(x1)
#define POP_LAST_2(x1, x2) x1
#define POP_LAST_3(x1, x2, x3) x1, x2
#define POP_LAST_4(x1, x2, x3, x4) x1, x2, x3
#define POP_LAST_5(x1, x2, x3, x4, x5) x1, x2, x3, x4
#define POP_LAST_6(x1, x2, x3, x4, x5, x6) x1, x2, x3, x4, x5
#define POP_LAST_7(x1, x2, x3, x4, x5, x6, x7) x1, x2, x3, x4, x5, x6
#define POP_LAST_8(x1, x2, x3, x4, x5, x6, x7, x8) x1, x2, x3, x4, x5, x6, x7

/* This will return the last argument */
#define LAST(...) CAT(LAST_, NARGS(__VA_ARGS__))(__VA_ARGS__)
#define LAST_1(x1) x1
#define LAST_2(x1, x2) x2
#define LAST_3(x1, x2, x3) x3
#define LAST_4(x1, x2, x3, x4) x4
#define LAST_5(x1, x2, x3, x4, x5) x5
#define LAST_6(x1, x2, x3, x4, x5, x6) x6
#define LAST_7(x1, x2, x3, x4, x5, x6, x7) x7
#define LAST_8(x1, x2, x3, x4, x5, x6, x7, x8) x8