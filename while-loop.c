#include <stdlib.h>
#include <stdio.h>

/* lower and upper iteration limits centered around 1.0 */
static double const eps1m01 = 1.0 - 0x1P-01;
static double const eps1p01 = 1.0 + 0x1P-01;
static double const eps1m24 = 1.0 - 0x1P-24;
static double const eps1p24 = 1.0 + 0x1P-24;

int main(int argc, char* argv[argc + 1]){
	for (int i = 1; i < argc; ++i) {
		double const a = strtod(argv[i], 0);
		double x = 1.0;
		for (;;) {
			double prod = a * x;
      printf("prod=%.12f,\teps1m01=%.12f,\teps1p01=%.12f\n", prod, eps1m01, eps1p01);
			if (prod < eps1m01) {
        x *= 2.0;
        printf("for1-1: x=%.5e\n", x);
      } else  if (eps1p01 < prod) {
          printf("eps1p01=%.12f\n", eps1p01);
          x *= 0.5;
          printf("for1-2: x=%.5e\n", x);
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
