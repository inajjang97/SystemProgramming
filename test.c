#include <stdio.h>
#include <string.h>
int main(){
	int i;
	float f;
	char ch='C';
	char s[5];
	strcpy(s,"INASOE");
	printf("%s\n", s);
	strcpy(s,"INASOE");
	printf("%s\n", s);
	f=i=ch+1;
	printf("%c %d %f\n", i,i,f);
	return 0;
}
