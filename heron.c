#include <stdlib.h>
#include <stdio.h>

/* lower and upper iteration limits centered around 1.0 */
static double const eps1m01 = 1.0 - 0x1P-02;
static double const eps1p01 = 1.0 + 0x1P-02;
static double const eps1m24 = 1.0 - 0x1P-24;
static double const eps1p24 = 1.0 + 0x1P-24;
static double const eps2 = 0x1.6P-02; // (1*16^0 + 6 * 16^-1) * 2^-2 = 0.34375

int main(int argc, char* argv[argc + 1]) {
  printf("eps2=%.12f\n", eps2);
  /* 假定(1+a)/ 2 < 1 -a;
   * 那么 a < (1/3)
   * 所以a越小，下面的程序有可能进入死循环。 
   * */
	for (int i = 1; i < argc; ++i) {
		double const a = strtod(argv[i], 0);
		double x = 1.0;
		for (;;) {
      /*
       * 如果ax / 2 < eps1m01 && ax > eps1p01 程序会进入死循环，
       * 比如： eps1m01 = 1.0 - 0x1P-02; eps1p01 = 1 + 0x1P-02
       *        程序输入1.26时会进入死循环。
       *        也即程序输入大于eps1p01到1+(1/3)之间的数，
       *        或者大于eps1m01 小于 （2/3）的数，
       *        或者x经过变形，最终ax（比如a = 2.52）符合上述情况的
       *        都会进入死循环。
       */
			double prodi = 2 / a; 
      printf("prodi=%.12f,\teps1m01=%.12f,\teps1p01=%.12f\n", prodi, eps1m01, eps1p01);
      if (x >= prodi) {
          printf("eps1p01=%.12f\n", eps1p01);
          x *= 0.5;
          printf("for1-2: x=%.5f\n", x);
        } else break;
		}

		for (;;) { // Heron approximation
			double prod = a * x;
      printf("prod=%.12f,\teps1m24=%.12f,\teps1p24=%.12f\n", prod, eps1m24, eps1p24);
			if ((prod < eps1m24) || (eps1p24 < prod)) {
				x *= (2.0 - prod);
        printf("for2: x=%.5e\n", x);
      } else break;
		}

		printf("heron: a=%.5e,\tx=%.5e,\ta*x=%.12f\n",
			a, x, a * x);
	}
	return EXIT_SUCCESS;
}
