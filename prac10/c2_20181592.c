#include <stdio.h>
#include <math.h>

double x, y;

int checkRoots(int, int, int);

void x_Cal(int*, int*, int*);

int main(void)
{
	int a, b, c;

	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c'\n");

	printf("a = ");
	scanf("%d", &a);

	printf("b = ");
	scanf("%d", &b);

	printf("c = ");
	scanf("%d", &c);

	int d;
	d = checkRoots(a, b, c);
	if (d == 0)
		printf("There are no solutions\n");
	else
	{
		x_Cal(&a,&b,&c);

		printf("\nResult :\n");
		printf("x is %.2f\n",x);
		printf("x is %.2f\n",y);

	}

	return 0;
}

int checkRoots(int a,int b,int c)
{
	int d;
	if (b*b - 4 * a*c < 0)
		d = 0;
	else
		d = 1;

	return d;
}

void x_Cal(int *a, int *b, int *c)
{
	double m;

	m = sqrt(*b * *b - 4 * *a * *c);

	x = ((-1)**b + m)/(2**a);
	y = ((-1)**b - m) / (2 * *a);

	return;
}