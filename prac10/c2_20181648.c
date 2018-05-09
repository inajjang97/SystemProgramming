#include <stdio.h>
#include <math.h>
int checkRoots(int a, int b, int c);
double pow(double x, double y);
int a, b, c;
double x, y;
int main(void)
{
	printf("Equation: ax^2 + bx + c = 0, input 'a' and 'b' and 'c'\n");
	printf("a =");
	scanf_s("%d", &a);
	printf("b =");
	scanf_s("%d", &b);
	printf("c =");
	scanf_s("%d", &c);
	checkRoots(a, b, c);
	double x = (-b - checkRoots(a,b,c)) / (2.0*a);
	printf("x is %f\n",x);
	double y = (-b + checkRoots(a,b,c)) / (2.0*a);
	printf("y is %f\n", y);
	system("pause");
}
int checkRoots(int a, int b, int c)
{
	if (b ^ 2 - 4 * a*c < 0)
		printf("There are no solutions\n");
	else
		return 0;

}

double pow(double x, double y)
{
	x = b ^ 2 - 4 * a*c;
	y = 2;
	return 0;
}
