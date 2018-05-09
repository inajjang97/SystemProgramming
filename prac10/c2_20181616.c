#include <stdio.h>
#include <math.h>

int checkRoots(int, int, int);
int a, b, c, result;
float x1, x2, root2;

int main(void)
{
	int  z;

	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c'\n");
	printf("a = ");
	scanf_s("%d", &a);
	printf("b = ");
	scanf_s("%d", &b);
	printf("c = ");
	scanf_s("%d", &c);

	z = checkRoots(&a, &b, &c);

	if (z = 0)
	{
		printf("There are no solutions");
	}
	else
		root2 = b * b - 4 * a * c;
		sqrt((double) root2);
		x1 = (-1 * b + root2) / 2 / a;
		x2 = (-1 * b - root2) / 2 / a;

		printf("Reslut : \n");
		printf("x is %.2f\n", x1);
		printf("x is %.2f\n", x2);
	return 0;
}


int checkRoots(int a, int b, int c)
{
	
	if (b * b - 4 * a * c < 0)
		result = 0;
	else
		result = 1;

	return result;
}