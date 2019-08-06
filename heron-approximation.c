#include <stdlib.h>
#include <stdio.h>

/* lower and upper iteration limits centered around 1.0 */
static double const eps1m01 = 1.0 - 0x1P-02; //此处最好是2的-1次幂，小于等于-2的整数次幂都可能出现死循环
static double const eps1p01 = 1.0 + 0x1P-02;
static double const eps1m24 = 1.0 - 0x1P-24; //此处1为目标近似值，0x1P-24为精度。如要目标为2，需要修改下
static double const eps1p24 = 1.0 + 0x1P-24; // 面第二个for循环中2为3（b-1 = 目标值）
static double const eps2 = 0x1P-02;

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
       *        或者x经过变形，最终ax（比如a = 2.52, eps1m01 = 0.75, eps1p01 = 1.25）符合上述情况的
       *        都会进入死循环。
       */
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

    /*
     * for 循环的原理为：f(x) = x*(b-ax)二次函数的值在x < (b -1) / a 时，二次函数的值大于x的值，
     * 由此 f(x) > x, f(f(x)) > f(x), 
     * 当 x > (b -1) / a时，f(x) < x, f(f(x)) < f(x) < x,
     * 总之使x 无限向 (b - 1) / a 靠近，而在此处y = ax 的值为 b - 1, 此时令y=1或者y=2,然后控制
     * eps1p24 eps1m24的精度即可控制x的精度
     * 此变换的前提条件为0 < x < (b / a)，否则变换会失效。
     * 所以第一个fort循环内，if判断可以改写为如下代码以防止出现死循环。
     * double prodi = b / a; // prodi 为二次函数与x轴的另一个交战
     * if ( x >= prodi)   
     *  x *= 0.5;
     * else break;
     */
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
