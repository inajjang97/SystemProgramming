#include <stdio.h>
#include <math.h>


int checkRoots(int, int, int);

void getRoots(int, int, int, float*, float*);

int main(void) {

	int a, b, c, check;
	float x1, x2;

	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c'\n");
	printf("a = ");
	scanf("%d", &a);
	printf("b = ");
	scanf("%d", &b);
	printf("c = ");
	scanf("%d", &c);
	check = checkRoots(a, b, c);
	if (check == 0)
		printf("There are no solutions\n");
	else {
		getRoots(a, b, c, &x1, &x2);
		printf("\nResult :\nx is %.2f\nx is %.2f\n", x1, x2);
	}

	
	return 0;
}

int checkRoots(int a, int b, int c) {
	int x;
	if (pow(b, 2) -4 * a * c < 0)
		x = 0;
	else
		x = 1;
	return x;
}

void getRoots(int a, int b, int c, float *x1, float *x2) {
	*x1 = (float)(-b + sqrt(pow(b,2) -4 * a * c)) / (2 * a);
	*x2 = (float)(-b - sqrt(pow(b,2) -4 * a * c)) / (2 * a);

}