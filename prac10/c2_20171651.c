#include <stdio.h>
#include <math.h>
int checkRoots(int a, int b, int c);
int find(int* a, int* b, int* c, int* result);
int main(void)
{
	int a, b, c;
	int result;

	printf("Equation : ax^2 + bx +c =0, input 'a' and 'b' and 'c'\n");
	printf("a = ");
	scanf("%d", &a);
	printf("b = ");
	scanf("%d", &b);
	printf("c = ")
	scanf("%d", &c);

	result = checkRoots(a, b, c);
	find(&a, &b, &c, &result);
	printf("Result : \n");
	printf("x is %.2f\n", &a);
	printf("x is %.2f\n", &b);
}

int checkRoots(int a, int b, int c)
{
	int result;

	if(doulbepow(b, 2)-4*a*c)<0 :
	result= 0;
	printf("Therer are no soultions\n");

	else:
	result= 1;

	return result;
}

int find(int* a, int* b, int* c, int* result)
{
	float x1, x2;
	if *result ==1;
	x1= (-*b + doublesqrt(doublepow(b, 2)-4 * *a * *c))/2 * *a;
	x2= (-*b - doublesqrt(doublepow(b, 2)-4 * *a * *c))/2 * *a;
	*a=x1;
	*b=x2;

	return 0;
}
