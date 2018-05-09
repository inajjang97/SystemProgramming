#include <stdio.h>

int a;
int fun(){
	a=10;
	return 20;
}
int main(){
	int b;
	a=2;
	b=a+2+fun();
	printf("%d\n", b);
	return 0;
}
