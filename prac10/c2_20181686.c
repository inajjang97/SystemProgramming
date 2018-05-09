#include <stdio.h>
#include <math.h>
int temp(int, int, int);

float solution1(int, int, int);

float solution2(int, int, int);

int main()
{
	int a, b, c;
	int check;
	float sol1, sol2;

	printf("Equation : ax^2 + bx + c + 0, input 'a' and 'b' and 'c'\n");
	printf("a = ");
	scanf("%d", &a);
	printf("b = ");
	scanf("%d", &b);
	printf("c = ");
	scanf("%d", &c);
	
	check = temp(a, b, c);
	if (check == 0) 	printf("There are no solutions\n");
	else {
		sol1 = solution1(a, b, c);
		sol2 = solution2(a, b, c);
		printf("Result :\n");
		printf("x is %f\n", sol1);
		printf("x is %f\n", sol2);
	}
}

int temp(int a, int b, int c)
{
	int result;
	int temp;

	if (b ^ 2 - 4 * a * c < 0) result = 0;
	else result = 1;
	return result;
}

float solution1(int a, int b, int c)
{
	double result;
	result = (-b + sqrt(b ^ 2 - 4 * a*c)) / (2 * a);
	return result;
}

float solution2(int a, int b, int c)
{
	float result;
	result = (-b - sqrt(b ^ 2 - 4 * a*c)) / (2 * a);
	return result;
}
