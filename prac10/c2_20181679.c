#include <math.h>

int checkRoots(int a, int b, int c) {

	int value;

	if ((b*b) - (4 * a*c) < 0) {
		return 0;
	}
	else {
		return 1;
	}
}

float result(int *a,int *b, int *c) {

	float result = (-(*b) + double sqrt(double pow(b, 2) - 4 * (*a)*(*c)))/2*(*a);
}

float result2(int *a, int *b, int *c) {

	float result2 = (-(*b) - double sqrt(double pow(b, 2) - 4 * (*a)*(*c)))/2*(*a);
}


void main() {

	printf("Equation : ax^2 + bx +c =0, input 'a' and 'b' and 'c'\n");

	int a, b, c;
	printf("a = ");
	scanf("%d", &a);
	printf("b = ");
	scanf("%d", &b);
	printf("c = ");
	scanf("%d", &c);

	checkRoots(a, b, c);
	if (0) {
		printf("There are no solutions\n");
	}
	else {
		printf("Result : \n");

		printf("x is %.2f\n", result(&a, &b, &c));

		printf("x is %.2f\n", result2(&a, &b, &c));

	}
}