#include <stdio.h>
#include <math.h>

int df(int a, int b, int c);

float c1f(int a, int b, int c);

float c2f(int a, int b, int c);

int main()
{
	int a, b, c;

	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c'\n");
	printf("a = ");
	scanf_s("%d", &a);
	printf("b = ");
	scanf_s("%d", &b);
	printf("c = ");
	scanf_s("%d", &c);
	printf("\nResult :\n");


	if ( df(a, b, c) == 1)
	{
		printf("x is %.2f\n", c2f(a,b,c));
		printf("x is %.2f\n", c1f(a,b,c));
	}
	else
	{
		printf("There are no solutions\n");
	}

	return 0;
}

int df(int a, int b, int c)
{
	int result;

	if (pow(b,2) - 4*a*c < 0)
	{
		result = 0;
	}
	else
	{
		result = 1;
	}

	return result;
}

float c1f(int a, int b, int c)
{
	float result;

	result = ( -b - (float)sqrt( pow(b,2) - 4*a*c )) / (2 * a);

	return result;
}

float c2f(int a, int b, int c)
{
	float result;

	result = (-b + (float)sqrt(pow(b, 2) - 4 * a*c)) / (2 * a);
	return result;
}