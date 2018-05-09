#include <stdio.h>

int checkRoots(int a, int b, int c);

int main(void)
{
	int a, b, c;
	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c'\n");
	pritnf("a = ");
	scanf_s("%d", &a);
	printf("b = ");
	scanf_s("%d", &b);
	printf("c = ");
	scanf_S("%d", &c);

	if (checkRoots == 0) {
		printf("There are no solution");
	}
	else {

		printf("Result : \n");
		printf("x is \n");
		printf("x is \n");
	}
	return 0;

	
}

int checkRoots(int a, int b, int c)
{
	if (b*b - 4 * a*c<0)
	{
		return 0;
	}
	else {
		return 1;
	}
}