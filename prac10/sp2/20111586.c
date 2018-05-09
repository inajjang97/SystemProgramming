#include "20111586.h"
#include "shell.h"
#include "memory.h"
#include "opcode.h"
#include "assembler.h"

//history에 node들을 linked list형식으로 삽입하는 함수
void insert(char *command, NODE** head, NODE** tail){
	NODE *node = (NODE*)malloc(sizeof(NODE));
	
	node->ptr = NULL;
	strcpy(node->data, command);

	if(!(*head)){
		*head = node;
		*tail = node;
	}
	else{
		(*tail)->ptr = node;
		*tail = node;
	}
}

//Error메세지 출력함수
void printError(int number){
	if(number == 0)	printf("Command Not Found!\n");
	else if(number == 1) printf("Boundary Error\n");
	else if(number == 2) printf("Value Error\n");
	else if(number == 3) printf("Parameter Error\n");
	else if(number == 4) printf("Not Hexadecimal Data\n");
	else if(number == 5) printf("Argument Error\n");
	else if(number == 6) printf("start address cannot be larger than end address\n");
}

// main에서 사용자로부터 입력받은 명령어를 판단, 올바른 command일시 해당 command 함수로 넘겨주는 함수
int command_check(char **command, int TOKEN_SIZE, unsigned char* MEMORY, unsigned int* address, OPCODE** ophead, SYMBOL ***symhead, int *symbol_print_flag){
	int i;
	unsigned int start, end, target, data, trash = 0;
	int comma_num = 0;


	// 잘못된 명령어일시 실패 return value 0;
	// quit 명령어일시 종료 return value 1;
	// 올바른 명령어일시 성공 return value 2;

	
	//토큰 1개짜리 명령
	if(TOKEN_SIZE == 1){
		// "quit"
		if(!strcmp(command[0], "q") || !strcmp(command[0], "quit")){
			return 1;
		}
		
		// "help"
		else if(!strcmp(command[0], "h") || !strcmp(command[0], "help")){
			command_help();
			return 2;
		}

		// "dir"
		else if(!strcmp(command[0], "d") || !strcmp(command[0], "dir")){
			command_dir();
			return 2;
		}
	
		// "history"
		else if(!strcmp(command[0], "hi") || !strcmp(command[0], "history")){
			return 3;
		}

		// "reset"
		else if(!strcmp(command[0], "reset")){
			command_reset(MEMORY);
			return 2;
		}

		// "opcodelist"
		else if(!strcmp(command[0], "opcodelist")){
			command_opcodelist(ophead);
			return 2;
		}
		
		// "symbol"
		else if(!strcmp(command[0], "symbol")){
			command_symbol(*symhead, *symbol_print_flag);
			return 2;
		}

		// "dump" 에서 start, end 없는 경우
		else if(!strcmp(command[0], "du") || !strcmp(command[0], "dump")){
			//Boundary Check
			if((*address + 160) >= 0xfffff){
				end = 0xfffff;
				start = *address;
				*address = 0;
			}
			else{
				start = *address;
				end = *address + 159;
				*address = end + 1;
			}
			command_dump(start, end, MEMORY);
			return 2;
		}

		//Command Error
		else printError(0);
	}
	
	//토큰이 두개가 넘어가는 명령
	else if(TOKEN_SIZE >= 2){
		
		// "opcode mnemonic"
		if(!strcmp(command[0], "opcode")){
			if(TOKEN_SIZE == 2)	return command_opcode_mnemonic(command[1], ophead);
			else printError(5);
		}
		
		// "type filename"
		else if(!strcmp(command[0], "type")){
			if(TOKEN_SIZE == 2) return command_type(command);
			else printError(5);
		}

		// "assemble filename.asm"
		else if(!strcmp(command[0], "assemble")){
			if(TOKEN_SIZE == 2) return command_assemble(command, ophead, symhead, symbol_print_flag);
			else printError(5);
		}

		// "dump start" or "dump start,end"
		else if((!strcmp(command[0], "du") || !strcmp(command[0], "dump")) && TOKEN_SIZE <= 4){
			
			// "dump start"
			if(TOKEN_SIZE == 2 && !strchr(command[1], ',')){

				//Hexadecimal Check	
				for(i = 0; i < strlen(command[1]); i++){
					if((command[1][i] >= '0' && command[1][i] <= '9') ||
						(command[1][i] >= 'a' && command[1][i] <= 'f') ||
						(command[1][i] >= 'A' && command[1][i] <= 'F') ||
						command[1][i] == ',') continue;
					printError(3);	return 0;
				}
				sscanf(command[1], "%x", &start);

				//Boundary Check
				if(start <= 0xfffff){
					if((start + 160) >= 0xfffff){
						end = 0xfffff;
						*address = 0;
					}
					else{
						end = start + 159;
						*address = end +1;
					}
					command_dump(start, end, MEMORY);
					return 2;
				}
				//Boundary Error
				else printError(1);
			}

			// "dump start,end"
			else{
				for(i = 1; i < TOKEN_SIZE - 1; i++){
					strcat(command[1], " ");
					strcat(command[1], command[i + 1]);
				}
				for(i = 0; i < strlen(command[1]); i++)
					if(command[1][i] == ',') comma_num++;
				
				//comma가 2개 이상일 시 Parameter Error
				if(comma_num >= 2) printError(3);
				
				else{
					i = sscanf(command[1], "%x , %x %x", &start, &end, &trash);
					
					if(i == 2){
						//Hexadecimal Check
						for(i = 0; i < strlen(command[1]); i++){
							if((command[1][i] >= '0' && command[1][i] <= '9') ||
								(command[1][i] >= 'a' && command[1][i] <= 'f') ||
								(command[1][i] >= 'A' && command[1][i] <= 'F') ||
								command[1][i] == ',' || command[1][i] == ' ') continue;
							printError(3); return 0;
						}
						
						//Boundary check
						if(start <= end){
							if((start >= 0 &&start <= 0xfffff) && (end >= 0 && end <= 0xfffff)){
								command_dump(start, end, MEMORY);
								if(end == 0xfffff) *address = 0;
								else *address = end + 1;
								return 2;
							}
							//Boundary Error
							else printError(1);
						}
						//start > end Error
						else printError(6);
					}
					//Parameter Error
					else printError(3);
				}
			}

		}
		
		// "fill start, end, value"
		else if((!strcmp(command[0], "f") || !strcmp(command[0], "fill")) && TOKEN_SIZE <= 6){
			for(i = 1; i < TOKEN_SIZE - 1; i++){
				strcat(command[1], " ");
				strcat(command[1], command[i + 1]);
			}
			for(i = 0; i < strlen(command[1]); i++)
				if(command[1][i] == ',') comma_num++;
			
			//comma 개수가 2개가 아닐시 Parameter Error
			if(comma_num != 2) printError(3);
		
			else{
				i = sscanf(command[1], "%x , %x , %x %x", &start, &end, &data, &trash);

				if(i == 3){
					//Hexadecimal Check
					for(i = 0; i < strlen(command[1]); i++){
						if((command[1][i] >= '0' && command[1][i] <= '9') ||
							(command[1][i] >= 'a' && command[1][i] <= 'f') ||
							(command[1][i] >= 'A' && command[1][i] <= 'F') ||
							command[1][i] == ',' || command[1][i] == ' ') continue;
						printError(3); return 0;
					}

					//Boundary Check
					if(start <= end){
						if(start <= 0xfffff && end <= 0xfffff){
							//Value Check
							if(data >= 0 && data <= 255){
								command_fill(start, end, data, MEMORY);
								return 2;
							}
							//Value Error
							else printError(2);
						}
						//Boundary Error
						else printError(1);
					}
					//start > end Error;
					else printError(6);
				}
				//Parameter Error
				else printError(3);
			}
		}
			
		// "edit address, value"
		else if(((!strcmp(command[0], "e") || !strcmp(command[0], "edit")) && TOKEN_SIZE <= 4)){
			for(i = 1; i < TOKEN_SIZE - 1; i++){
				strcat(command[1], " ");
				strcat(command[1], command[i + 1]);
			}

			for(i = 0; i < strlen(command[1]); i++)
				if(command[1][i] == ',') comma_num++;
		
			//comma 개수가 1개가 아닐시 Parameter Error
			if(comma_num != 1){
				printError(3); return 0;
			}

			else{
				i = sscanf(command[1], "%x , %x %x", &target, &data,&trash);
				
				if(i == 2){
					//Boundary Check
					if(target <= 0xfffff){
						//Value Check
						if(data >= 0 && data <= 255){
							command_edit(target, data, MEMORY);
							return 2;
						}
						//Value Error
						else printError(2);
					}
					//Boundary Error
					else printError(1);
				}
				//Parameter Error
				else printError(3);
			}
		}

		//Command Error
		else printError(0);
	}
	
	//Command Error
	else	printError(0);

	return 0;
}

int main(){
	char command[INPUT_SIZE] = {"\0"}, command_temp[INPUT_SIZE] = {"\0"};			
	char **command_token;					
	char *ptr;						
	int i, TOKEN_SIZE, flag = 0, symbol_print_flag = 0;						
	unsigned int address = 0;
	unsigned char MEMORY[MEMORY_SIZE];
	NODE *head = NULL, *tail = NULL;
	OPCODE *ophead[20];
	SYMBOL **symhead = NULL;

	flag = build_opcodelist(ophead);					
	command_reset(MEMORY);
	//initialize data
	
	while(flag != 1){
		
		TOKEN_SIZE = 1;
		printf("sicsim> ");
		
		fgets(command, sizeof(command), stdin);

		command[strlen(command) - 1] = '\0';
		
		strcpy(command_temp, command);

		ptr = strtok(command_temp, " \t\r\n");		
		//tap, space, enter를 기준으로 token화
		if(!ptr) continue;
		
		while(1){
			ptr = strtok(NULL, " \t\r\n");
			if(!ptr) break;
			TOKEN_SIZE++;
		}
		
		command_token = (char **)malloc(sizeof(char *) * (TOKEN_SIZE + 1));

		strcpy(command_temp, command);

		ptr = strtok(command_temp, " \t\r\n");
		if(!ptr) continue;

		command_token[0] = (char *)malloc(sizeof(char) * (INPUT_SIZE));
		strcpy(command_token[0], ptr);
		
		for(i = 1; i < TOKEN_SIZE; i++){	
			ptr = strtok(NULL, " \t\r\n");
			if(ptr == NULL)	break;
			command_token[i] = (char *)malloc(sizeof(char) * (INPUT_SIZE));
			strcpy(command_token[i], ptr);
		}
		
		flag = command_check(command_token, TOKEN_SIZE, MEMORY, &address, ophead, &symhead, &symbol_print_flag);	//나누어진 token을 명령어 check를 위해 function call

		if(flag == 1) command_quit(&head, ophead);
		//quit 명령어 입력시 프로그램 종료
		else if(flag == 2) insert(command, &head, &tail);				
		//정상적인 명령어일시 history에 command 저장
		else if(flag == 3){							
			//history 명령어 입력시 커맨드 저장후 명령어 수행
			insert(command, &head, &tail);
			command_history(head, tail);
		}

		//다음 명령어 입력을 위해 기존 명령어 buffer 초기화
		for(i = 0; i < TOKEN_SIZE; i++) {			
			free(command_token[i]);
			command_token[i] = NULL;
		}
		strcpy(command, "");
		strcpy(command_temp, "");
	}

	return 0;

}
