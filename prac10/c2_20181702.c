#include <stdio.h>
int checkRoots(int, int, int);
int result(int*, int*, int*);

int main(void)
{
	int a, b, c;
	int x;
	int res1, res2;

	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c'\n");
	printf("a = ");
	scanf("%d", &a);
	printf("b = ");
	scanf("%d", &b);
	printf("c = ");
	scanf("%d", &c);

	x = checkRoots(a, b, c);

	if (x == 0)
	{
		printf("There are no solutions\n");
	}
	else
	{
		printf("Result: \n");
		printf("x is %d\n", res1);
		printf("x is %d\n", res2);
	}
}
int checkRoots(int a, int b, int c)
{
	int result;
	
	if (b * b - (4 * a * c) < 0)
	{
		result = 0;
	}

	else
	{
		result = 1;
	}
	
	return result;
}