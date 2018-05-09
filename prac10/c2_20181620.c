#include <stdio.h>

int checkRoots(int a, int b, int c);
{
	int result;
	
	if (b ^ 2 - 4 * a*c < 0)
		printf("There are no solutions");
	else
		return result;

	return result;

}
int calcx(int a, int b, int c);
{
	int result;

	result =





}

int main(void)
{
	int a, b, c, x, r, u;

	checkRoots(a, b, c);

	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c'");
	scanf("%d %d %d", &a, &b, &c);

	printf("a = %d", a);
	printf("b = %d", b);
	printf("c = %d", c);

	printf("Result :\n");

	r = calcx(a, b, c);

	printf("x is %d\n", r);
	printf("x is %d\n", r);

	return 0;
	
}

