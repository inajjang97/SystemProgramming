#include <stdio.h>
#include <math.h>

int checkRoots(int, int, int);
double cal1(int*, int*, int*);
double cal2(int*, int*, int*);

int main(void) {
	int a, b, c;

	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c'\n");
	printf("a = ");
	scanf("%d", &a);
	printf("b = ");
	scanf("%d", &b);
	printf("c = ");
	scanf("%d", &c);

	if (checkRoots(a, b, c) == 0) {
		printf("There are no solutions\n");
		return 0;
	}
	
	printf("\nResult :\n");
	printf("x is %6.2f\n", cal1(&a, &b, &c));
	printf("x is %6.2f\n", cal2(&a, &b, &c));

	return 0;
}

int checkRoots(int a, int b, int c) {
	if (pow((double)b, 2.0) - 4 * a * c < 0)
		return 0;
	else
		return 1;
}

double cal1(int *a, int *b, int *c) {
	return (*b * (-1) + sqrt(pow((double)*b, 2.0) - 4 * *a * *c))/ (2 * *a);
}

double cal2(int *a, int *b, int *c) {
	return (*b * (-1) - sqrt(pow((double)*b, 2.0) - 4 * *a * *c)) / (2 * *a);
}