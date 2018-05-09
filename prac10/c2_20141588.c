#include <stdio.h>
#include <math.h>

int checkRoots(int a, int b, int c)
{
	if ((pow(b, 2) - 4 * a*c) < 0)
		return 0;
	else
		return 1;
}
void GetRoots(int *a, int*b, int *c, float *num1, float *num2)
{
	float x = (float)(*a);
	float y = (float)(*b);
	float z = (float)(*c);

	*num1 = (-y + sqrt(pow(y, 2) - 4 * x * z)) / (2 * x);
	*num2 = (-y - sqrt(pow(y, 2) - 4 * x * z)) / (2 * x);
}
int main(void)
{
	int a, b, c;
	int check;
	float val1, val2;

	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c'\n");
	printf("a = ");
	scanf("%d", &a);
	printf("b = ");
	scanf("%d", &b);
	printf("c = ");
	scanf("%d", &c);

	check = checkRoots(a, b, c);

	if (check == 0)
	{
		printf("There are no solutions\n");
		return 0;
	}
	
	GetRoots(&a, &b, &c, &val1, &val2);

	printf("Result :\n");
	printf("x is %.2f\n", val1);
	printf("x is %.2f\n", val2);
	
	return 0;
}