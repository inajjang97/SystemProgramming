#include <stdio.h> 
#include <math.h>

int checkRoots(int a, int b, int c);
double solution(int, int, int, int);

int main()
{
	int a, b, c;
	int value;
	double result1, result2;

	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c'\n");
	printf("a : ");
	scanf("%d", &a);
	printf("b : ");
	scanf("%d", &b);
	printf("c : ");
	scanf("%d", &c);

	value = checkRoots(a, b, c);

	if (value==0)
	{
		printf("There are no solutions\n");
	}
	else
	{
		int d = 1;
		result1 = solution(&a, &b, &c, d);
		printf("x is %.2lf\n", result1);

		d = 2;
		result2 = solution(&a, &b, &c, d);
		printf("x is %.2lf\n", result2);

	}
	
	return 0;
}

int checkRoots(int a, int b, int c)
{
	int value;

	if (pow(b, 2) - 4 * a * c < 0)
	{
		value = 0;
	}
	else
	{
		value = 1;
	}

	return value;
}

double solution(int a, int b, int c, int d)
{
	double x;

	if (d == 1)
	{
		x = (-b + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
	}
	else
	{
		x = (-b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
	} 

	return x;
}