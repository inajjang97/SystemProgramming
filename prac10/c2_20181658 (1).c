#include <stdio.h> <math.h>

int checkRoots(int a, int b, int c);

double x1(int* a, int* b, int* c);

double x2(int* a, int* b, int* c);

double pow(double x, double y);

double sqrt(double x);

int main() {
	int a, b, c;
	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c'\na =");
	scanf("%d", &a);
	printf("b = ");
	scanf("%d", &b);
	printf("c = ");
	scanf("%d", &c);

	if (checkRoots(a, b, c) == 1) {
		printf("Result :\nx is %.2lf\nx is %.2lf\n", x1(&a, &b, &c), x2(&a, &b, &c));
	}
	else {
		printf("There are no solutions\n");
	}
}
int checkRoots(int a, int b, int c) {
	if (pow((double)b, (double)2) - 4 * a * c < 0) {
		return 0;
	}
	else {
		return 1;
	}
}

double x1(int* a, int* b, int* c) {
	double root = pow((double)*b, (double)2) - 4 * *a * *c;
	return ((-1)*(double)*b + sqrt(root)) / (2 * *a);
}

double x2(int* a, int* b, int* c) {
	double root = pow((double)*b, (double)2) - 4 * *a * *c;
	return ((-1)*(double)*b - sqrt(root)) / (2 * *a);

}