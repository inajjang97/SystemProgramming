#include <stdio.h>
#include <string.h>

typedef struct INA{
	char a[5];
	char b[10];
}ina;
int main(){
	ina aa;

	strcpy(aa.a,"ABCDE");
	strcpy(aa.b,"HELLLLLLO");
	printf("%s! %s!\n", aa.a, aa.b);

	strcpy(aa.a,"ABCDE");
	printf("%s! %s!\n", aa.a, aa.b);
	return 0;
}
