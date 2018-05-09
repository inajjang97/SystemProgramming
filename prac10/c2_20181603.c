#include <stdio.h>
#include <math.h>

double x1, x2;

int checkRoots(int a, int b, int c);
double Result(double* x);

double pow(double x, double y);
double sqrt(double x);

double main(void)
{
	int a, b, c;
	double result1, result2;

	printf("Equation : ax^2 + bx _ c = 0, input 'a' and 'b' and 'c'\n");
	
	printf("a = ");
	scanf("%d", &a);

	printf("b = ");
	scanf("%d", &b);

	printf("c = ");
	scanf("%d", &c);

	checkRoots(a, b, c);
	result1 = Result(&x1);
	result2 = Result(&x2);

	if (checkRoots(a, b, c))
		printf("There are no solutions");
	else
	{
		printf("Result : \n");
		printf("x is %lf\n", result1);
		printf("x is %ls\n", result2);

	}

	return 0;
}

int checkRoots(int a, int b, int c)
{
	if (b*b - 4 * a*c < 0)
		return 0;
	else
		return 1;
}

double Result (double* x)
{
	double result;
	double a, b, c, x;

	result = *x;
	return result;
}

double pow(double x, double y)
{
	x = y*y;
	return y;

}

double sqrt(double x)
{
	;
}