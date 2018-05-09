#include <stdio.h>
#include <math.h>
int checkRoots(int a, int b, int c);
float Func1(int*a, int*b, int*c);
float Func2(int*a, int*b, int*c);

int main(void)

{
	float x, y;
	int a, b, c, d;

	printf("Equation : ax^2 + bx + c =0, input 'a' and 'b' and 'c'\n");
	printf("a = ");
	scanf("%d", &a);
	printf("b = ");
	scanf("%d", &b);
	printf("c = ");
	scanf("%d", &c);

	d = checkRoots(a, b, c);
	x = Func1(&a, &b, &c);
	y = Func2(&a, &b, &c);

	if (d == 0)
	{
		printf("There are no solutions\n");
	}
	else
	{
		printf("x is %5.2f\n", x);
		printf("x is %5.2f\n", y);
	}

	return 0;
}

int checkRoots(int a, int b, int c)
{
	int d;
	if (b*b - 4 * a*c<0)
	{
		d = 0;
	}
	else
	{
		d = 1;
	}

	return d;
}

float Func1(int*a, int*b, int*c)
{
	float x, y;
	y = ((*b)*(*b) - 4 * (*a)*(*c)) ^ (1 / 2);

	x = ((-*b) + y) / 2 * (*a);
	return x;

}

float Func2(int*a, int*b, int*c)
{
	float x, y;
	y = ((*b)*(*b) - 4 * (*a)*(*c)) ^ (1 / 2);

	x = ((-*b) - y) / (2 * (*a));
	return x;

}