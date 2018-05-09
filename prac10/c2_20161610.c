#include <stdio.h>
int checkRoots(int a, int b, int c);
int findRoots(int a, int b, int c);
double pow(double x, double y);
double sqrt(double x)

int main(void)
{

	int a, b, c;

	printf("equation: ax^2 + bx +c= 0, input 'a' and 'b' and 'c'");
	printf("a=");
	scanf("%d", &a);
	printf("b=");
	scanf("%d", &b);
	printf("c=");
	scanf("%d", &c);

	checkRoots(a, b, c);
	findRoots(a, b, c);


}

int checkRoots(int a, int b, int c)
{
	if ((b*b) < 4 * a*c)
		printf("there are no solution\n");
	

	else
		return;
		
}

int findRoots(int a, int b, int c)
{
	int d, e, f;
	d=((-b)+(double sqrt(double (b*b)-4*a*c)))/2*a
    e=((-b))


}
