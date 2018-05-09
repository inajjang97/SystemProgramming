#define histempty 1111
#define no_rem_list 2222
#define wrongcom 3333333
#define lineMax 0xfffff
#define nameMax 30

#define START 10
#define END 11
#define WORD 12
#define BYTE 13
#define RESW 14
#define RESB 15
#define INFO 16

//레지스터 
#define A 0
#define X 1
#define L 2
#define B 3
#define S 4
#define T 5
#define F 6

typedef struct node{
	char *com;
	int num;
	struct node *next;
}node;

#define TYPE1 1
#define TYPE2 2
#define TYPE34 3
//hash table을 만들기 위한 array 구성요소
typedef struct hnode{
	char type[5];
	int typenum;
	char op[3];
	char m[10];
	struct hnode *next;
} hnode;//hash table linked list pointer

//history를 만드는 linked list
typedef struct hash{
	int num;
	struct hnode *next;
} hash;


typedef struct INST{
	int adr;
	int opcode;
	int type;
	int op2_flag;
	int info;
	int n;
	int i;
	int x;
	char word[80];
	char mn[30];
	char opr[30];
	char opr2[30];
	char obj[8];
}inst;

typedef struct SYMBOL{
	int adr;
	int line;
	char name[30];
}symbol;
