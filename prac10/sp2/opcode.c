#include "20111586.h"
#include "opcode.h"

// "assemble file.asm" command 수행 시 나타나는 mnemonic의 opcode를 반환하는 함수
int check_mnemonic(char *mnemonic, char *format, int *opcode, OPCODE **ophead){
    OPCODE *temp;
    char data[10];

    if(mnemonic[0] == '+')  strcpy(data, mnemonic + 1);
	//4형식일 경우 +문자 지우고 mnemonic만 받기
    else strcpy(data, mnemonic);
	//아닐 경우 그대로 진행
    temp = ophead[hash_function(data, 20)];

    while(temp){
        if(!strcmp(temp->name, data)){
                *opcode = temp->num;
                strcpy(format, temp->type);
                return 1;
				//중도 발견시 성공 return value 1
        }
        temp = temp->ptr;
    }
	
    return 0;
	//실패시 실패 return value 0
}

// 처음 프로그램 실행시 opcode table을 build하기 위한 함수
int build_opcodelist(OPCODE** ophead){
    FILE *fp = fopen("opcode.txt", "rt");
    OPCODE *node;
    int i;

    if(!fp){
        printf("There is no opcode.txt\n");
        return 1;
    }
	//opcode.txt 파일 없을시 ERROR

    node = (OPCODE*)malloc(sizeof(OPCODE));
    for(i = 0; i < 20; i++) ophead[i] = NULL;
    while(fscanf(fp, "%x %s %s", &node->num, node->name, node->type) != EOF)
        opcode_insert(node, ophead);
	//opcode의 data를 hash table을 이용 opcode table을 build
    fclose(fp);

    return 0;
}

// opcode table build시 node insert를 위한 함수
void opcode_insert(OPCODE* node, OPCODE** ophead){
    int index = hash_function(node->name, 20);
    OPCODE *temp;

    if(!ophead[index]){
        ophead[index] = (OPCODE*)malloc(sizeof(OPCODE));
        ophead[index]->num = node->num;
        strcpy(ophead[index]->name,node->name);
        strcpy(ophead[index]->type,node->type);
        ophead[index]->ptr = NULL;
    }
	//head가 비어있다면 head에 노드 연결

    else{
        temp = (OPCODE*)malloc(sizeof(OPCODE));
        temp->num = node->num;
        strcpy(temp->name,node->name);
        strcpy(temp->type,node->type);
        temp->ptr = ophead[index]->ptr;
        ophead[index]->ptr = temp;
    }
	//head가 비어있지 않다면 head뒤에 노드 연결
}

// hash table의 index를 얻기 위한 hash function 함수
int hash_function(char *s, int size){
    int i, sum;

    for(i = 0, sum = 0; i < strlen(s); i++)
        sum += s[i];
    return sum % size;
}

// "opcode mnemonic" command 수행 opcode table에서 mnemonic 탐색하는 함수
int command_opcode_mnemonic(char *command, OPCODE** ophead){
    OPCODE *temp;

    temp = ophead[hash_function(command, 20)];

    while(temp){
        if(!strcmp(temp->name, command)){
            printf("opcode is %02X\n", temp->num);  
			//탐색성공시 결과 출력
            return 2;
        }
        temp = temp->ptr;
    }

    printf("%s is not in opcodelist\n", command);           
	//탐색실패시 에러 출력

    return 0;
}

// "opcodelist" command 수행 opcode table의 모든 내용을 출력하는 함수
void command_opcodelist(OPCODE** ophead){
    int i;
    OPCODE *temp;

    for(i = 0; i < 20; i++){
        temp = ophead[i];
        printf("%2d : ", i);

        if(!temp) printf("Empty!");
        while(temp){
            printf("[%s,%02X]", temp->name, temp->num);
            if(temp->ptr) printf(" -> ");
            temp = temp->ptr;
        }
        printf("\n");
    }
}


