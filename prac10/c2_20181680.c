#include <math.h>
int checkRoots(int a, int b, int c);
void asdf(int a, int b, int c, float *x, float *y);
int main()
{
	int a, b, c, val;
	float x, y;

	printf("Equation : ax^2 +bx +c = 0, input 'a' and 'b' and'c'\n");
	
	printf("a = ");
	scanf("%d", &a);

	printf("b = ");
	scanf("%d", &b);

	printf("c = ");
	scanf("%d", &c);

	val = checkRoots(a, b, c);

	if (val = 0)
	{
		printf("Ther are no solutions\n");
	}

	else 
	{
		asdf(a, b, c, &x, &y);
		printf("Result :\n");
		printf("x is %10.2f\n", a);
		printf("x is %10.2f\n", b);
	}

	
}

int checkRoots(int a, int b, int c)
{
	if ((b ^ 2) - (4 * a *c) < 0)
		return 0;
	

	else
		return 1;
}

void asdf(int a, int b, int c, float *x, float *y)
{
	float z;

	z * z == (float)((b ^ 2) - (4 *a *c));
	
	*x = (float)((-b) + z) / (2 * a);
	*y = (float)((-b) - z) / (2 * a);

	(float)a = *x;
	(float)b = *y;
}