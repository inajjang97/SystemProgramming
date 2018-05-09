#include <stdio.h>

float x;
float num1;
float num2;
int checkRoots(int a, int b, int c)

{
	if ((b^2)-(4*a*c) < 0)
{ 
	printf("There are no solutions\n");

	return 0;
}
	else
{
	return 1; 
}
}

int solveRoots(int a, int b, int c,float x,float num1, float num2)
{ 
	
	if ((b^2)-(4*a*c) >0)
{ 
	float x;
	printf("Result : \n ");

	num1 = (-b + ((b^2-4*a*c)^(1/2)))/(2*a);
	
	num2 = (-b - ((b^2-4*a*c)^(1/2)))/(2*a);
	

	printf("x is %.2f\n", num1);
	printf("x is %.2f\n", num2);

	return 1;
}
}
int main(void)

{
	int a,b,c;
	float x, num1;
	float num2;
	printf("Equation : ax^2 + bx +c = 0, input 'a' and 'b' and 'c'\n");
	printf("a = ");
	scanf("%d",&a);
	printf("b = ");
	scanf("%d",&b);
	printf("c = ");
	scanf("%d",&c);

	checkRoots(&a,&b,&c);
	
	solveRoots(&a,&b,&c,x,num1,num2);
	return solveRoots;
}

