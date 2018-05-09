#include <stdio.h>
#include <math.h>

int checkRoots(int a, int b, int c) {
	int check;
	check = (b*b) - (4 * a*c);
	if (check < 0) {
		return 0;
	}
	else {
		return 1;
	}
}

void gen1(int a, int b, int c, double *x) {
	*x = (sqrt(pow((double)b, (double)2) - (double)(4 * a*c)) - (double)b) / (double)a / 2.0;

}

void gen2(int a, int b, int c, double *x2) {
	*x2 = (-sqrt(pow((double)b, (double)2) - (double)(4 * a*c)) - (double)b) / (double)a / 2.0;
}

int main() {
	int a, b, c;
	int cR;
	double x1,x2;
	printf("Equation : ax^2 + bx + c = 0, input 'a' and 'b' and 'c'\n");
	printf("a = ");
	scanf("%d", &a);
	printf("b = ");
	scanf("%d", &b);
	printf("c = ");
	scanf("%d", &c);
	cR = checkRoots(a, b, c);

	if (cR == 0) {
		printf("There are no solutions\n");
		return 0;
	}
	else {
		gen1(a, b, c,&x1);
		gen2(a, b, c,&x2);
		printf("\nResult :\n");
		printf("x is %.2lf\n", x1);
		printf("x is %.2lf\n", x2);
		
	}
}