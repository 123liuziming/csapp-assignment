/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */

#endif
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y)
{
  return ~((~x) | (~y));
}
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n)
{
  return ((x >> (n << 3))) & 0x000000ff;
}
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n)
{
  int mask = ~(((1 << 31) >> n) << 1);
  return mask & (x >> n);
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x)
{
  // 分治思想
  // 构造掩码0101010101....01
  int tmp_mask = 0x55 | (0x55 << 8);
  int mask_1 = tmp_mask | (tmp_mask << 16);
  // 构造掩码00110011....0011
  tmp_mask = 0x33 | (0x33 << 8);
  int mask_2 = tmp_mask | (tmp_mask << 16);
  // 构造掩码00001111....00001111
  tmp_mask = 0x0f | (0x0f << 8);
  int mask_3 = tmp_mask | (tmp_mask << 16);
  // 构造掩码00000000111111110000000011111111
  int mask_4 = 0xff | (0xff << 16);
  // 构造掩码00000000000000001111111111111111
  int mask_5 = 0xff | (0xff << 8);
  // 之后求总和
  int count = (x & mask_1) + ((x >> 1) & mask_1);
  count = (count & mask_2) + ((x >> 2) & mask_2);
  count = (count & mask_3) + ((x >> 4) & mask_3);
  count = (count & mask_4) + ((x >> 8) & mask_4);
  count = (count & mask_5) + ((x >> 16) & mask_5);
  return count;
}
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x)
{
  return ~(((1 << 31) >> 31) & x);
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void)
{
  return 1 << 31;
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n)
{
  int neg_n = ~n + 1;
  n = 32 + n;
  int mask = ((1 << 31) >> n) << 1;
  return mask & x;
}
/* 
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n)
{
  int sign = (x >> 31) & 1;
  int bias = (sign << n) + (~sign + 1);
  return (x + bias) >> n;
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x)
{
  return ~x + 1;
}
/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x)
{
  return !(x >> 31) & (!!x);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y)
{
  /*
    <= 相当于求不大于
    1. x < 0 y > 0
    2. x > 0 y < 0
    3. x > 0 y > 0
    4. x < 0 y < 0
  */
  int x_sign = (x >> 31) & 0x1;
  int y_sign = (x >> 31) & 0x1;
  return !((!x_sign & y_sign) | (x_sign & y_sign & ((y + ~x + 1) >> 31)) | (!x_sign & !y_sign & ((y + ~x + 1) >> 31)));
}
/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int ilog2(int x)
{
  // 采用二分法
  // 看高16位有没有1
  int shift_16 = (!!(x >> 16)) >> 4;
  // 有1的话直接往右移16位
  x = x >> shift_16;
  int shift_8 = (!!(x >> 8)) >> 3;
  x = x >> shift_8;
  int shift_4 = (!!(x >> 4) >> 2);
  x = x >> shift_4;
  int shift_2 = (!!(x >> 2) >> 1);
  x = x >> shift_2;
  return shift_16 + shift_8 + shift_4 + shift_2 + !!(x >> 1);
}
/* 
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf)
{
  int is_nan = ((uf >> 23) & 0xff == 0xff) & (uf << 9);
  return is_nan ? uf : ((1 << 31) ^ uf);
}
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x)
{
  /*
    整数的浮点数一定是规格化数
    浮点数可以写作类似:1.xxxxxxx * 2^E这种形式
    这样我们就可以有M=f以及E=e-bias,其中bias=127
  */
  // 先判断正负
  unsigned mask = 1 << 31;
  unsigned sign = mask & x;
  unsigned absx = x;
  unsigned e;
  unsigned f;
  unsigned left_bit = 0;
  unsigned frac_tail;
  if (sign) 
    absx = -x;
  // 找int中最左边的1
  while (absx & 0x80000000 != 0x80000000) {
    absx <<= 1;
    ++left_bit;
  }
  // 离x最近的2的次方是2^(31-left_bit)
  e = 127 + 31 - left_bit;
  f = ~(mask >> 8) & (absx >> 8);
  frac_tail = absx & 0xff;
  unsigned res = e + f + sign;
  if (frac_tail > 0x80 || (frac_tail == 0x80 && (f & 0x1)))
    ++res;
  return res;
}
/* 
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf)
{
  /*
    浮点数: f = (-1)^sign * 2 ^E * M
    规格化数: E = e - bias, M = 1 + f
    非规格化数: E = 1 - bias, M = f
    所以规格化数的两倍可以让阶码加一
    非规格化数的两倍可以让尾数变为两倍
  */
  unsigned E = uf & 0x7f800000;
  unsigned S = uf & 0x80000000;
  unsigned F = uf & 0x007fffff;
  if (E == 0)
    uf = S + E + (F << 1);
  else if (E & 0x7f800000 == 0x7f800000)
    uf = S + (E + 0x00800000) + F;
  return uf;
}
