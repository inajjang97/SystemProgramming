#include <stdio.h>
#include <math.h>

int checkRoots(int, int, int);
void findRoots(float *, float *);
int a, b, c;

int main()
{
	float x1, x2;
	int result;

	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c' \n");
	printf("a = ");
	scanf("%d", &a);
	printf("b = ");
	scanf("%d", &b);
	printf("c = ");
	scanf("%d", &c);

	result = checkRoots(a, b, c);

	if (result == 0) {
		printf("There are so nolutions \n");
	}

	else {
		findRoots(&x1, &x2);

		printf("Result : \n");
		printf("x is %.2f \n", x1);
		printf("x is %.2f \n", x2);
	}

	return 0;
}

int checkRoots(int a, int b, int c)
{
	if (pow(b, 2) - 4 * a*c < 0) {
		return 0;
	}

	else {
		return 1;
	}
}

void findRoots(float *x1, float *x2)
{
	*x1 = (-b + sqrt(pow(b, 2) - 4 * a*c)) / (2 * a);
	*x2 = (-b - sqrt(pow(b, 2) - 4 * a*c)) / (2 * a);
}