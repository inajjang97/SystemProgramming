#include<math.h>
#include<stdio.h>

int checkRoots(int, int, int);
void answer(int a, int b, int c, int *x1, int *x2);

int main(void)
{
	int a, b, c;
	float x1, x2;

	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c'\n");
	printf("a = ");
	scanf("%d", &a);
	printf("b = ");
	scanf("%d", &b);
	printf("c = ");
	scanf("%d", &c);

	if (checkRoots(a, b, c) == 1)
	{
		answer(a, b, c, &x1, &x2);
		printf("Result :\n");
		printf("x is %.2f\n", x1);
		printf("x is %.2f\n", x2);
	}
	else
	{
		printf("There are no solutions.\n");
	}
	return 0;
}

int checkRoots(int a, int b, int c)
{
	int check;

	if (b * b - 4 * a * c >= 0)
	{
		check = 1;
	}
	else
	{
		check = 0;
	}
	return check;
}
void answer(int a, int b, int c, float *x1,float *x2)
{
	float temp;

	a = (float)a;
	b = (float)b;
	c = (float)c;

	temp = sqrt(b * b - 4 * a * c);

	*x1 = (-b + temp) / 2 / a;
	*x2 = (-b - temp) / 2 / a;
}