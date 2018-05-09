#include <stdio.h>
#include <math.h>

int checkRoots(int a, int b, int c) {
	if ((int)pow((double)b, (double)2) - 4 * a * c < 0) {
		return 0;
	}
	return 1;
}

void getHae(int *A, int *B, int *C) {
	int a = *A; int b = *B; int c = *C;

	*A = 1000 * (-b + sqrt((double)(pow((double)b, (double)2) - 4 * a*c))) / (2 * a);
	*B = 1000 * (-b - sqrt((double)(pow((double)b, (double)2) - 4 * a*c))) / (2 * a);
	return;
}

int main() {

	int a, b, c;

	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c'\n");
	printf("a = "); scanf("%d", &a);
	printf("b = "); scanf("%d", &b);
	printf("c = "); scanf("%d", &c);
	if (!checkRoots(a, b, c)) {
		printf("There are no solutions\n");
		return 0;
	}
	getHae(&a, &b, &c);

	printf("\nResult :\n");
	printf("x is %.2f\n", (double)a/1000);
	printf("x is %.2f\n", (double)b/1000);
	
	return 0;

}