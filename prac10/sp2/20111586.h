#ifndef MAIN_H__
#define MAIN_H__
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/stat.h>

// 정의되는 상수
#define INPUT_SIZE 0xff
#define MEMORY_SIZE 0xfffff

//history 저장에 사용할 linked list node
typedef struct _NODE{
	char data[INPUT_SIZE];
	struct _NODE* ptr;
}NODE;

//opcodelist hash table 작성시 사용할 linked list node
typedef struct _OPCODE{
    int num;
    char name[10];
    char type[5];
    struct _OPCODE* ptr;
}OPCODE;

//objectcode 저장에 사용할 linked list node
typedef struct _OBJECT{
    int first_data, mid_data, end_data, location;
    struct _OBJECT* ptr;
    int format, enter_flag;
    char var[255];
}OBJECT;

//symbol table 작성시 사용할 linked list node
typedef struct _SYMBOL{
    int location;
    char name[10];
    struct _SYMBOL* ptr;
}SYMBOL;

// 정의되는 함수
void insert(char *command, NODE** head, NODE** tail);
void printError(int number);
int command_check(char **command, int TOKEN_SIZE, unsigned char* MEMORY, unsigned int* address, OPCODE** ophead, SYMBOL ***symhead, int *symbol_print_flag);
#endif
