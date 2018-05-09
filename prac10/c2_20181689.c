#include <stdio.h>
#include <math.h>

int checkRoots(int a, int b, int c) {
	if (pow(b, 2) - 4 * a * c < 0) { return 0; }
	return 1;
}

void getkey(int a, int b, int c, float* t1, float* t2) {
	*t1 = (-b + (sqrt(pow(b, 2) - 4 * a * c))) / (2 * a);
	*t2 = (-b - (sqrt(pow(b, 2) - 4 * a * c))) / (2 * a);
}

void main() {
	int a, b, c;
	float t1, t2;

	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c'\n");

	printf("a = ");
	scanf("%d", &a);
	
	printf("b = ");
	scanf("%d", &b);

	printf("c = ");
	scanf("%d", &c);

	if (!checkRoots(a, b, c)) {
		printf("There are no solutions\n");
		return;
	}

	getkey(a, b, c, &t1, &t2);

	printf("\nResult :\n");
	printf("x is %.2f\n", t1);
	printf("x is %.2f\n", t2);
}