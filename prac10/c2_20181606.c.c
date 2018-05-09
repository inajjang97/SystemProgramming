#include <stdio.h>
#include <math.h>

int value;
int a, b, c;
int checkRoots(int, int, int);
int solve(double*, double*);

int main()
{
	double x1, x2;
	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c'\n");
	printf("a = ");
	scanf_s("%d", &a);
	printf("b = ");
	scanf_s("%d", &b);
	printf("c = ");
	scanf_s("%d", &c);
	checkRoots(a, b, c);
	if (value = 0)
	{
		printf("There are no solutions\n");
	}

	else
	{
		solve(&x1, &x2);
		printf("Result :\n");
		printf("X is %.2f\n", x1);
		printf("X is %.2f\n", x2);

	}

}

int checkRoots(int a, int b, int c)
{
	if (b*b - 4 * a*c < 0)
	{
		value = 0;
	}
	else
	{
		value = 1;
	}
	return value;
}

int solve(double *x1, double* x2)
{
	double k = pow(b, 2) - 4 * a*c;
	*x1 = (-1 * b + sqrt(k)) / 2 * a;
	*x2 = (-1 * b - sqrt(k)) / 2 * a;

}