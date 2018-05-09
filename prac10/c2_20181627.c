#include <stdio.h>
#include <math.h>

float result1, result2;

int checkRoots(int a, int b, int c);

void findRoots(int *a, int *b, int *c);

int main(void)
{
	int a, b, c;

	printf("Equation : ax ^ 2 + bx + c = 0, Input 'a' and 'b' and 'c'\n");
	printf("a = ");
	scanf("%d", &a);
	printf("b = ");
	scanf("%d", &b);
	printf("c = ");
	scanf("%d", &c);

	if (checkRoots(a, b, c) == 0)
	{
		printf("There is no solutions\n");
	}
	else
	{
		findRoots(&a, &b, &c);

		printf("result :\nx is %.2f\nx is %.2f\n", result1, result2);
	}

	return 0;
}

int checkRoots (int a, int b, int c)
{
	int check;

	if (pow((double)b, (double)2) - 4 * a*c >= 0)
	{
		check = 1;
	}
	else
	{
		check = 0;
	}

	return check;
}

void findRoots(int *a, int *b, int *c)
{
	result1 = (-1 * *b + sqrt(pow((double)*b, (double)2) - (double)4 * *a * *c)) / (2 * *a);
	result2 = (-1 * *b - sqrt(pow((double)*b, (double)2) - (double)4 * *a * *c)) / (2 * *a);

	return;
}