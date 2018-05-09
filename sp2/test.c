#include <stdio.h>
#include <string.h>
int main(){
	char* tmp;
	char* r;
	char str[100];
	FILE* fp= ("helo.txt", "wt");
	
	strcpy(str, "	 ADD	 LENGH, X	AA AADSJS	");
	printf("%s\n", str);
	r=str;

	tmp= strtok_r(r, " \r\t\n", &r);
	printf("1 %s!\n", tmp);
	
	tmp= strtok_r(r, " \r\t\n", &r);
	printf("2 %s!\n", tmp);

	printf("R= %s\n", r);
}

int read(){
	FILE* fp = fopen("helo.txt", "r");

}
