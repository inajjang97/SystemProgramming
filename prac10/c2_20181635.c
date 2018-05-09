#include <stdio.h> 
#include <math.h>

int checkRoots(int, int, int);
float sol(int* a, int* b, int* c);
double pow(double x, double y);
double sqrt(double x);

float q;
float r;

int main()

{
	int a, b, c;
	int e;

	printf("Equation: ax^2 + bx + c = 0, input 'a' and 'b' and 'c' \n");
	printf("a = ");
	scanf("%d", &a);
	printf("b = ");
	scanf("%d", &b);
	printf("c = ");
	scanf("%d", &c);

	e = checkRoots(a, b, c);

	if (e == 1)
	{
		
		sol(&a, &b, &c);
		
		printf("Result : \n");
		printf("x is %.2f \n", q);
		printf("x is %.2f \n", r);



	}

	else
	{
		printf("There are no solutions \n");


	}



	return 0;


}

int checkRoots(int a, int b, int c)
{
	int f;
	int g=2;
	int h;
	f = pow(b,g);

	if (f - 4 * a*c < 0)
	{
		h = 0;

		return h;
	}

	else
	{
		h = 1;

		return h;
	}


}



float sol(int* a, int* b, int* c)

{
	int l;
	int m=2;
	float o;
	float p;
	
	l = pow(*b, m);
	p = l - 4 * *a * *c;
	o = sqrt(p);

	 q = ( (-*b) + o )/ (2 * *a);
	 r = ((-*b) - o) / (2 * *a);

	





}