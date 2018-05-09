#include "20111586.h"
#include "memory.h"

// "reset" command 수행 메모리 초기화 함수
void command_reset(unsigned char* MEMORY){
    int i;

    for(i = 0; i < MEMORY_SIZE; i++) MEMORY[i] = 0;
}

// "edit address, value" command 수행 메모리 편집 함수
void command_edit(unsigned int target, unsigned char data, unsigned char* MEMORY){
    MEMORY[target] = data;
}


// "fill start, end, value" command 수행 메모리 채우기 함수
void command_fill(unsigned int start, unsigned int end, unsigned char data, unsigned char* MEMORY){
    int i;

    for(i = start; i <= end; i++)
        MEMORY[i] = data;
}

// "dump [start], [end]" command 수행 메모리 출력 함수
void command_dump(unsigned int start, unsigned int end, unsigned char* MEMORY){
    int i, j;
    int start_row, end_row;

    start_row = start / 16;
    end_row = end / 16;
    for(i = 0; i <= end_row - start_row; i++){
        printf("%05X ", (start_row + i) * 16);
        for(j = 0; j < 16; j++){
            if(((start_row + i) * 16 + j) < start || ((start_row + i) * 16 + j) > end)
                printf("   ");
            else if(((start_row + i) * 16 + j) >= start && ((start_row + i) * 16 + j) <= end)
                printf("%02X ", MEMORY[(start_row + i) * 16 + j]);
        }
        printf("; ");
        for(j = 0; j < 16; j++){
            if(MEMORY[(start_row + i) * 16 + j] >= 20 && MEMORY[(start_row + i) * 16 + j] <= 127)
                printf("%c", MEMORY[(start_row + i) * 16 + j]);
            else printf(".");
        }

        printf("\n");
    }

}

