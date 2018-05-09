#include <stdio.h>
#include <stdlib.h>

typedef struct INA_ST{	
	int i;
	float f;
}ina_st;

typedef struct INA_ST2{
	int a;
	double d;
	struct INA_ST* next;
}ina_st2;

union ina_union{
	int a;
	char ch1, ch2;
	char s[4];
};

enum ina_enum {one, two, three, four, five } e1, e2;

int x,y,z;
int overflow3=0;
int overflow2=0;
int overflow1=0;
int overflow=0;
char g_string[5];

int func1(int n);
void func2(int* n);
void recursion(int n);
int main(){
	int x,n=0;
	int local_arr[10][5][7];
	union ina_union uni1;

	x=1;
	y=2;
	z=3;
	n=10;

	n=func1(n);
	


}
void recursion(int n){
	static int a=0;
	if( n==0)
		return;
	
	a= n;
	recursion(n-1);
}

//call by value
int func1(int n){
	int y=100;
	x+=n;
	y+=n;
	z+=n;
	func2(&n);
	return n;
}

//call by reference
void func2(int* n){
	int z=1000;
	x+=*n;
	y+=*n;
	z+=*n;
	*n= 0;
}
