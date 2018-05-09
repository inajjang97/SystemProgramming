#include <stdio.h>
#include <math.h>

int checkRoots(int a, int b, int c);

void root (int, int, int);

float x, y;

int main (void)
{
	int a, b, c;

	printf("Equation ax^2 + bx + c = 0, intput 'a' and 'b' and 'c'\na = ");
	scanf("%d",&a);
	printf("b = ");
	scanf("%d",&b);
	printf("c = ");
	scanf("%d", &c);

	if (checkRoots(a, b, c) == 1)
	{
		root(a, b, c);
		printf("\nResult :\nx is %.2f\nx is %.2f\n", x, y);
	}

	else
	{
		printf("There are no solutions\n");
	}

	return 0;
}

int checkRoots(int a, int b, int c)
{
	b = pow((double)b,2);
	
	if (b - (double)(4 * a*c) >= 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void root(int a, int b, int c)
{
	(double)a;
	(double)b;
	(double)c;

	x = (-b + sqrt(b*b - 4 * a*c)) / (2 * a);

	y = (-b - sqrt(b*b - 4 * a*c)) / (2 * a);
}