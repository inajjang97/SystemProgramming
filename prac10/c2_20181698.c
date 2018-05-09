#include<stdio.h>
#include<math.h>
int a, b, c;
float x1, x2;
int checkRoots(int a, int b, int c)
{
	if (b*b - 4 * a*c < 0) return 0;
	return 1;
}

void ans(int a, int b, int c)
{
	x1=(-b + sqrt(b*b - 4 * a*c)) / (float)(2 * a);
    x2=(-b - sqrt(b*b - 4 * a*c)) / (float)(2 * a);
	return;
}


int main()
{
	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c'\n");
	printf("a = ");
	scanf("%d", &a);

	printf("b = ");
	scanf("%d", &b);

	printf("c = ");
	scanf("%d", &c);

	if (checkRoots(a, b, c) == 0)
	{
		printf("There are no solutions\n");

		return 0;
	}
	printf("\nResult :\n");
	ans(a, b, c);
	printf("x is %.2lf\n", x1);
	printf("x is %.2lf\n", x2);


	return 0;
}