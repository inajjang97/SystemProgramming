#ifndef MEMORY_H__
#define MEMORY_H__

//정의되는 함수
void command_reset(unsigned char* MEMORY);
void command_edit(unsigned int target, unsigned char data, unsigned char* MEMORY);
void command_fill(unsigned int start, unsigned int end, unsigned char data, unsigned char* MEMORY);
void command_dump(unsigned int start, unsigned int end, unsigned char* MEMORY);

#endif
