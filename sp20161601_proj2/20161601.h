#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

//명령어글자 최대 길이
#define COMMAND_MAX 256
#define MEMORY_ROW 0xfffff
#define MEMORY_COL 35
#define HASH_MAX 20
#define LINE_MAX 256
#define CODE_MAX 0xfffff

//예외처리
#define WRONG_ADR 1
#define WRONG_VAL 2
#define WRONG_PARAMETER 3
#define WRONG_CMM 4
#define INPUT_SIZE_OVER 5
#define WRONG_FILE 6
#define ASSEMBLE_FAIL 7
#define NO_SYMBOL 8

//명령어 num
#define q 3

#define VAR 9
#define CONST 10
#define BASE 11
#define DIREC 12
#define COMMENT 13

#define A 0
#define X 1
#define L 2
#define B 3
#define S 4
#define T 5
#define F 6
#define PC 8
#define SW 9

//Linked List 
typedef struct LinkedList {
	int cnt;
	struct Node* front;
	struct Node* rear;
}ll;

typedef struct Node {
	char command[COMMAND_MAX];
	struct Node* next;
}node;

typedef struct OP {
	int op_num;
	char op_name[20];
	char op_type[10];
	struct OP* next;
}op;

typedef struct SYM{
	int adr;
	char name[30];
	struct SYM* next;
}sym;

typedef struct CODE{
	int adr;
	int num;
	char inst[20];
	char operand[30];
	char operand2[30];
	int type;
	char obj[100];
}code;

void command_h();
void command_d();
void command_q();
void command_hi();
int command_du(char cmm[10][COMMAND_MAX],int cnt);
int command_e(char cmm[10][COMMAND_MAX]);
int command_f(char cmm[10][COMMAND_MAX]);
void command_reset();
int command_op_mn(char* s);
void command_op_list();
int hash_key(char* s);
void create_hash_table();

int command_file(char* name);
int command_asm(char* name);
int command_sym();

int create_lst(FILE* fp,char* inst, int line);
int create_obj(FILE* fp,char* prog_name);

int insert_code(char* inst, char* operand, int line);
int store_symbol(int adr, int size, char* s, int line);
int comp_symbol(char *name, char* tmp);
int insert_symbol(char* name, int adr);
int read_symbol(char* name);
void free_symbol();
int read_reg(char* s);

int prompt();
void print_error(int error);
void insert(char cmm[COMMAND_MAX]);

char* remove_space(char* s);
int one_digit_to_int(char c);
int make_hex(char* s);
char make_char(int n);
int make_int(char* s);
