/*
#include <stdio.h>

#include <math.h>

int checkRoots(int a, int b, int c);

double equation(int *a, int *b, int *c);



int main()

{

	int a, b, c, result;

	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c'\n");

	scanf("a = %d\nb = %d\nc = %d\n", &a, &b, &c);



	checkRoots(a, b, c);

	equation(&a, &b, &c);

	result = equation(&a, &b, &c);

	printf("Result :\n x is ")



}



int checkRoots(int a, int b, int c)

{

	if (b ^ 2 - (4)(a)(c) < 0)

	{

		printf("There are no solutions\n");

		return 0;

	}

	else

	{

		return 1;

	}



}



double equation(int *a, int *b, int *c)

{

	double num1, num2, num3, x;

	num1 = pow(*b, 2);

	num2 = num1 - 4 * (*a) * (*c);

	num3 = sqrt(num2);



	x = -(*b) + num3 / 2 (*a);



	return x;

}
*/