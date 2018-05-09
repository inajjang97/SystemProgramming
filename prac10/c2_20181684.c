#include <stdio.h>

int checkRoots(int a, int b, int c);
float wowowo(double a, double b, double c);
float wowowowo(double a, double b, double c);

int main(void)
{
	int a, b, c;

	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c'\n");
	printf("a = ");
	scanf_s("%d", &a);
	printf("b = ");
	scanf_s("%d", &b);
	printf("c = ");
	scanf_s("%d", &c);

	if (checkRoots(a, b, c) == 0)
	{
		printf("There are no solutions\n");

		return 0;
	}

	printf("\n \nResult : \n");
	printf("x is %.2f\n", wowowo(a, b, c));
	printf("x is %.2f\n", wowowowo(a, b, c));
}

int checkRoots(int a, int b, int c)
{
	if(b*b - 4*a*c < 0)
	{
		return 0;
	}

	else
	{
		return 1;
	}
}

#include <math.h>

float wowowo(double a, double b, double c)
{
	double x = ((double)-b + (double)sqrt(pow(b, 2) - 4 * a*c)) / (2 * a);

	return x;

}

float wowowowo(double a, double b, double c)
{
	double x = (-b - sqrt(pow(b, 2) - 4 * a*c)) / (2 * a);

	return x;
}