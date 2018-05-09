#include<stdio.h>
#include<math.h>
int checkRoots(int a, int b, int c)
{
	if (b*b - 4 * a*c < 0)
		return 0;
	else
		return 1;
}
double solution_1(int *a, int *b, int *c)
{
	double p;
	double q;
	double x;
	p = pow((double)*b, 2);
	q = sqrt((double)(p - (4 * (*a)*(*c))));
	x = (-*b + q) / 2* (*a);
	return x;
}
double solution_2(int *a, int *b, int *c)
{
	double p;
	double q;
	double x;
	p = pow((double)*b, 2);
	q = sqrt((double)(p - 4 * (*a)*(*c)));
	x = (-*b - q) / 2 * (*a);
	return x;
}
int main()
{
	int a, b, c;
	int x;
	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c'\na = ");
	scanf("%d", &a);
	printf("b = ");
	scanf("%d", &b);
	printf("c = ");
	scanf("%d", &c);
	x = checkRoots(a, b, c);
	if (x == 0)
		printf("There are no solutions\n");
	else
		printf("Result : \nx is %.2lf\nx is %.2lf\n", solution_1(&a, &b, &c), solution_2(&a, &b, &c));

	return 0;
}