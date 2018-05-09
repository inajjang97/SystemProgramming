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

//예외처리
#define WRONG_ADR 1
#define WRONG_VAL 2
#define WRONG_PARAMETER 3
#define WRONG_CMM 4
#define INPUT_SIZE_OVER 5
//명령어 num
#define h 1
#define d 2
#define q 3
#define hi 4
#define du 5
#define e 6
#define f 7
#define reset 8
#define op_mn 9
#define op_list 10

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

