#include <stdio.h>
char aaa;
char bbb[]="";
enum days {MON,TUE,WED};
union utag {
	char bb[9];
	enum days kk[2];
	char* c;
}haha;

struct aa{
	float j;
	char aaa;
	char bbb;
	enum days bb;
	union utag kk;
}test[6][10],lala;

int main(){
	printf("%d %d\n", &aaa, &(test[3][4]));
	test[3][4].kk.c=&aaa;
	test[3][4].bb=TUE;
	printf("sizeof bbb %d aaa %d\n" ,sizeof(bbb));
	return 0;
}
