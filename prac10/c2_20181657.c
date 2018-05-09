#include <stdio.h>

int checkRoots(int a, int b, int c)
{
	int return_value;

	if (b*b - 4 * a*c < 0)
		return_value = 0;
	else
		return_value = 1;

	return return_value;
}

float c_answer(int a, int b, int c)
{
	int d, e;

	d = b*b;

	return sqrt((double)(d - 4 *a*c));

}
void main(void)

{

	int a, b, c, d, e;


	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c'");
	printf("a = ");
	scanf_s("%d", &a);
	printf("b = ");
	scanf_s("%d", &b);
	printf("c = ");
	scanf_s("%d", &c);
	
	d = checkRoots(a, b, c);
	if (d = 0)
		printf("There are no solutions");

	else
		e = c_answer(a, b, c);
		printf("\n\nResult:\n");
		printf("x is %.2f", ((-1)*b-e)/(2*a));
		printf("x is %.2f", ((-1)*b + e) / (2 * a));

	return;
}