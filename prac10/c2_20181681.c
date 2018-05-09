#include <stdio.h>
#include <math.h>

int checkRoots(int, int, int);
void root1(int*, int*, int*, float*);
void root2(int*, int*, int*, float*);


void main() {
	int a, b, c;
	float x1, x2;
	
	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c'\na = ");
	scanf("%d", &a);
	printf("b = ");
	scanf("%d", &b);
	printf("c = ");
	scanf("%d", &c);

	if (checkRoots(a, b, c) == 0) {
		printf("There are no solutions\n");
	}
	else {
		root1(&a, &b, &c, &x1);
		root2(&a, &b, &c, &x2);

		printf("\nResult :\nx is %.2f\nx is %.2f\n", x1, x2);
	}
	return;
}
int checkRoots(int a, int b, int c) {
	int chR;
	if (b*b - 4 * a*c < 0) {
		chR = 0;
	}
	else {
		chR = 1;
	}
	return chR;
}
void root1(int* a, int* b, int* c, float* x1) {
	double d, e, f;
	d = (double)*a;
	e = (double)*b;
	f = (double)*c;

	*x1 = (-e + sqrt(pow(e, 2) - 4 * d*f)) / (2 * d);
	return;
}
void root2(int* a, int* b, int* c, float* x2) {
	double d, e, f;
	d = (double)*a;
	e = (double)*b;
	f = (double)*c;

	*x2 = (-e - sqrt(pow(e, 2) - 4 * d*f)) / (2 * d);
	return;
}