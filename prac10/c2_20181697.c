#include <stdio.h>
#include <math.h>

int null = 0;
int checkRoots(int a, int b, int c);
float Roots(int a, int b, int c);

int main(void)
{
	int a, b, c;
	int check;
	float result1, result2;

	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c'\n");
	printf("a = ");
	scanf("%d", &a);
	printf("b = ");
	scanf("%d", &b);
	printf("c = ");
	scanf("%d", &c);

	check = checkRoots(a, b, c);

	if (check == 1)
	{
		printf("\n");

		result1 = Roots(a, b, c);
		result2 = Roots(a, b, c);

		printf("Result : \n");
		printf("x is %3.2f \n", result1);
		printf("x is %3.2f \n", result2);
	}
	else
	{
		printf("There are no solutions\n");
	}

	return 0;
}

int checkRoots(int a, int b, int c)
{
	int val;

	if ((pow(b, 2) - 4 * a*c) < 0)
	{
		val = 0;
	}
	else
	{
		val = 1;
	}

	return val;
}

float Roots(int a, int b, int c)
{
	float result;

	if (null == 0)
	{
		result = (-b + sqrt(pow(b, 2) - 4 * a*c)) / (2 * a);

		null++;
	}
	else
	{
		result = (-b - sqrt(pow(b, 2) - 4 * a*c)) / (2 * a);
	}

	return result;
}