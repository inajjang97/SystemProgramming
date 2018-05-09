#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ina.h"
#define INAINA 10

int func(int* a);
void func_struct();
void recursion(int n);
void func_static();
void func_casting(int n, float f);
void func_union();
void func_ext(int a, int b);
void func_pointer();
enum ina_enum {one, two, three, four, five} e1,e2;
union ina_union{
	int a,b;
	char c[4];
	char d,e;
};
typedef struct INA_ST{
	int i;
	float f;
	char c;
	struct INA_ST* next;
}ina_st;


int x=1,y=10,z=100;
char ch[3][5][102];
char s[3][20]= {"INA HOHO", "NANANA", "DIFFERNT_LENGTH"};

int main(){
	int a,b,x;
	float f=2.5;
	a=4;
	b=a+5;
	x= a+b-1;
	
	y= a+func(&a);
	z= a;
	
	ch[2][3][45]=10;

	recursion(b);

	for(int i=0; i<5; i++){
		int b = 2*i;
		func_static(b);
	}

	func_ext(a,b);
	func_casting(a, f);
	func_casting(f,a);
	func_struct();
	return 0;
}

int func(int* a){
	int z=10;
	*a= x+y+z;
	
	return 10;
}

void func_struct(){
	ina_st *node, *node2;
	node = (ina_st*)malloc(sizeof(ina_st));
	node->i=5;
	node->f=1.5;
	node->c='0';
	node->next=NULL;

	node2= (ina_st*)malloc(sizeof(ina_st));
	node2->i=10;
	node2->c='1';
	node2->f= 2.0;
	node2->next=node;

	free(node2);
	free(node);
	return;
}

void recursion(int n){
	if(n==0)
		return;

	recursion(n-1);
}

void func_static(int a){
	static int ans=0;
	ans+=a;
	return;
}

void func_casting(int n, float f){
	float ff;
	int ii;

	ii= n+f;
	ff= n+f;
	
	n= ii+(int)f;
	f= ff+(float)n;
	
	return;
}
void func_union(){
	union ina_union u;
	
	strcpy(u.c, "INA");
	u.d='A';
	u.e='B';
	printf("%s\n",u.c);
}

void func_enum(){
	int a;
	e1=two;
	e2=five;
	
	a= e1+e2;

	if(a>three){
		a= one;
	}
	return;
}

void func_pointer(){
	char* s;
	int* d;
	int arr[10];
	int s_adr, d_adr;

	s="INA";
	d=arr;

	s_adr = (s+1);
	d_adr= (d+1);
}
