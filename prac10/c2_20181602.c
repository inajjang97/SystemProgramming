#include <stdio.h>
#include <math.h>


int checkRoots(int, int, int);

int main()
{
	int a, b, c, d;
	float x, y, f;
	float e;

	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c'\n");
	printf("a = ");
	scanf_s("%d", &a);
	printf("b = ");
	scanf_s("%d", &b);
	printf("c = ");
	scanf_s("%d", &c);

	d = checkRoots(a, b, c);

	f = b*b - 4 * a*c;
	e = sqrt(f);


	if (d == 0)
		printf("There are no solutions\n");
	else
	{		x = (float)((-b + e) / (2 * a));
	y = (float)((-b - e) / (2 * a));
	printf("Result :\n");
	printf("x is %0.2f\n", x);
	printf("x is %0.2f\n", y);
}


	return 0;
}


int checkRoots(int a, int b, int c)
{
	if ( b*b < 4 * a*c )
		return 0;
	else
		return 1;
}