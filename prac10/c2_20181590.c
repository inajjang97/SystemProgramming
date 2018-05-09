#include<stdio.h>
#include<math.h>

int checkRoots(int a, int b, int c);
float findRoots(int a, int b, int c);

int Check = 1;

int main(void)
{
	int mA, mB, mC, cR;
	float fR1, fR2;
	
	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c'\na = ");
	scanf_s("%d", &mA);
	printf("b = ");
	scanf_s("%d", &mB);
	printf("c = ");
	scanf_s("%d", &mC);

	cR = checkRoots(mA, mB, mC);
	
	if (cR == 0)
		printf("There are no solutions\n");
	else
	{
		fR1 = findRoots(mA, mB, mC);
		printf("x is %.2f\n", fR1);
		fR2 = findRoots(mA, mB, mC);
		printf("x is %.2f\n", fR2);
	}

	return 0;
}

int checkRoots(int a, int b, int c)
{
	int value, sqr;

	sqr = pow(b, 2);
	
	if ((sqr - (4 * a*c)) < 0)
		value = 0;
	else
		value = 1;

	return value;
}

float findRoots(int a, int b, int c)
{
	float Result1, Result2, Root1, Square1, Root2, Result;
	
	Square1 = (float)(pow(b, 2));
	Root2 = Square1 - 4 * a * c;
	Root1 = (float)(sqrt(Root2));
	
	Result1 = ((-b + Root1) / (2 * a));
	Result2 = ((-b - Root1) / (2 * a));

	if (Check == 1)
		Result = Result1;
	else
		Result = Result2;

	Check = Check + 1;

	return Result;
}