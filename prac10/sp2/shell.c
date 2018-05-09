#include "20111586.h"
#include "shell.h"

// "type filename" command 수행 해당하는 file을 프롬프트에 출력
int command_type(char **command){
    FILE *fp = fopen(command[1], "rt");
    char temp[255];

    if(!fp){
         printf("There is no %s file\n",command[1]);
         return 0;
    }
    //없는 파일을 불렀다면 ERROR

    while(fgets(temp, sizeof(temp), fp))
        printf("%s", temp);

    fclose(fp);
    return 2;
}

// "dir" command 수행 현재 디렉토리 내의 파일목록 출력을 위한 함수
void command_dir(){
    DIR *dirp = opendir(".");
    struct dirent *entry;
    struct stat file_type;

    while((entry = readdir(dirp)) != NULL){
        stat(entry->d_name, &file_type);
        if(S_ISDIR(file_type.st_mode)){
            if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
            // ".", ".."은 무시
            printf("%s/\n", entry->d_name);
            // 폴더일시 폴더명 뒤에 / 추가
        }
        else if(file_type.st_mode & S_IXUSR)
            printf("%s*\n", entry->d_name);
            // 실행파일일시 파일명 뒤에 * 추가
        else printf("%s\n", entry->d_name);
            // 그 외 파일일시 기본출력양식
    }

    closedir(dirp);
}

// "history" command 정상 수행된 command들의 list를 순차적으로 출력하는 함수
void command_history(NODE* head, NODE* tail){
    NODE *temp = head;
    int i = 1;

    while(temp){
        printf("%3d  %s\n", i++, temp->data);
        temp = temp->ptr;
    }
}

// "help" command 수행 셸에서 사용가능한 모든 명령어를 출력하는 함수
void command_help(){
    printf("h[elp]\n");
    printf("d[ir]\n");
    printf("q[uit]\n");
    printf("hi[story]\n");
    printf("du[mp] [start, end]\n");
    printf("e[dit] address, value\n");
    printf("f[ill] start, end, value\n");
    printf("reset\n");
    printf("opcode mnemonic\n");
    printf("opcodelist\n");
    printf("assemble filename\n");
    printf("type filename\n");
    printf("symbol\n");
}

// "quit" command 수행 프로그램 종료 전 동적할당한 메모리를 모두 free시켜주는 함수
void command_quit(NODE** head, OPCODE** ophead){
    NODE *ptr1;
    OPCODE *ptr2;
    int i;

    ptr1 = *head;
    while(*head){
        ptr1 = *head;
        *head = (*head)->ptr;
        free(ptr1);
    }

    for(i = 0; i < 20; i++){
        ptr2 = ophead[i];
        while(ophead[i]){
            ptr2 = ophead[i];
            ophead[i] = ophead[i]->ptr;
            free(ptr2);
        }
    }
}

