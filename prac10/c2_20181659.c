#include <stdio.h>
#include <math.h>

float checkRoots(float a, float b, float c)
{
	if (pow(b, 2) - 4 * a*c < 0)
	{
		return 0;
	}
	else
		return 1;
}

float getRoots(float *a, float *b, float *c)
{
	float num1, num2;
	num1 = ((-*b + (sqrt((pow(*b, 2) - 4 * (*a)*(*c))))) / (2 * (*a)));
	num2 = ((-*b - (sqrt((pow(*b, 2) - 4 * (*a)*(*c))))) / (2 * (*a)));
	*a = num1;
	*b = num2;
	return *a, *b;
}

int main(void)
{
	float a, b, c, checknum;
	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c'\n");
	printf("a = ");
	scanf_s("%f", &a);
	printf("b = ");
	scanf_s("%f", &b);
	printf("c = ");
	scanf_s("%f", &c);
	checknum = checkRoots(a, b, c);
	if (checknum == 0)
	{
		printf("There is no solutions\n");
		return 0;
	}
	else
	{
		a,b = getRoots(&a, &b, &c);
		printf("Result : \n");
		printf("x is %3.2f\nx is %3.2f\n", a, b);
	}
	return 0;
}