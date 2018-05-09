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
	f = double(double b, double c);

	d = ((-b) + double sqrt(f))/2*a ;
	e = ((-b) - double sqrt(f)) / 2 * a;

	printf("result:");
	printf("x is %d", d);
	printf("x is %d", e);
	
	return 0;



}

double pow(double x, double y)
{

	int k;

	k = x*2;
	return k;
}

double sqrt(double x)
{
	int j;
	j*j == x;
	return j;

}
