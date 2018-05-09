#include "20111586.h"
#include "opcode.h"
#include "assembler.h"

// 계산된 objectcode를 linked list node로 연결하는 함수
void objectcode_insert(int format, int first, int mid, int end, int location, OBJECT** head, OBJECT** tail, char *var, int enter_flag){
    OBJECT *node = (OBJECT*)malloc(sizeof(OBJECT));

    node->ptr = NULL;
    node->first_data = first;
    node->mid_data = mid;
    node->end_data = end;
    node->location = location;
    node->format = format;
    strcpy(node->var, var);
    node->enter_flag = enter_flag;
	//자료 복사

    if(!(*head)){
        *head = node;
        *tail = node;
    }
	//head가 비어있다면 head에 연결

    else{
        (*tail)->ptr = node;
        *tail = node;
    }
	//head가 비어있지 않으면 tail에 연결
}

// assemble 과정에서 나타나는 symbol들을 저장하기 위한 symbol table build 함수
int build_symtab(SYMBOL** symhead, SYMBOL* node, int line_num){
    int index = hash_function(node->name, 100);
    SYMBOL *temp, *ptr;

	if(node->name[0] >= '0' && node->name[0] <= '9'){
		printf("LINE : %d SYMBOL NAME ERROR\n", line_num);
		return 1;
	}
	//SYMBOL의 첫글자가 숫자인 경우 ERROR

    if(!symhead[index]){
        symhead[index] = (SYMBOL*)malloc(sizeof(SYMBOL));
        strcpy(symhead[index]->name, node->name);
        symhead[index]->location = node->location;
        symhead[index]->ptr = NULL;
    }
	//head가 비어있다면 head에 연결

    else{
		ptr = symhead[index];
		while(ptr){
			if(!strcmp(ptr->name, node->name)){
				printf("LINE : %d DUPLICATE SYMBOL ERROR\n", line_num);
				return 1;
				//중복된 SYMBOL이 발견되었다면 DUPLICATE SYMBOL ERROR
			}
			ptr = ptr->ptr;
		}
        temp = (SYMBOL*)malloc(sizeof(SYMBOL));
        strcpy(temp->name, node->name);
        temp->location = node->location;
        temp->ptr = symhead[index]->ptr;
        symhead[index]->ptr = temp;
		//head가 비어있지 않다면 head의 뒤에 연결
    }
	return 0;
	//build 성공시 성공 return value 0
	//build 실패시 실패 return value 1
}

// object code 계산을 위해 operand symbol의 location값을 얻기 위한 함수
int find_symbol(SYMBOL **symhead, char *symbol){
    SYMBOL *temp;
    int i, const_flag = 0;

    for(i = 0; i < strlen(symbol); i++){
        if(symbol[i] >= '0' && symbol[i] <= '9') continue;
        else const_flag = 1;
    }
	//OPERAND가 상수인지 체크, 상수면 test = 0, 상수가 아니면 test = 1;

    if(const_flag == 0)   return -2;
	//OPERAND 상수인 경우 상수 return value -2;

    temp = symhead[hash_function(symbol, 100)];

    while(temp){
        if(!strcmp(temp->name, symbol))
            return temp->location;
			//OPERAND SYMBOL 발견시 SYMBOL의 location값 return
        temp = temp->ptr;
    }
    
	return -1;
	//탐색 실패시 실패 return value -1;
}

// assemble과정 중 location을 계산하기 위한 함수
void location_counter(char *format,unsigned int *location, char *mnemonic){
    if(!strcmp(format, "1")) *location += 1;
    else if(!strcmp(format, "2")) *location += 2;
    else if(!strcmp(format, "3")) *location += 3;
    else if(!strcmp(format, "4")) *location += 4;
    else if(!strcmp(format, "3/4")){
        if(mnemonic[0] == '+'){
            *location += 4;
            strcpy(format, "4");
        }
        else{
            *location += 3;
            strcpy(format, "3");
        }
    }
}

// "symbol" command 수행 가장 최근에 assemble 성공한 file의
// symbol table을 symbol 기준 내림차순으로 출력하는 함수
void command_symbol(SYMBOL **symhead, int flag){
    SYMBOL *node = NULL, *list = NULL, *ptr = NULL, *follow = NULL, *temp;
    int i;

    if(!flag){
        printf("SYMTAB IS EMPTY\n");
        return;
    }
	//assemble한 적이 없거나 최근에 assemble한 file이 실패한 file일 경우
	//symtab이 존재하지 않으므로 비어있다고 알림
	
	//symtab이 존재하는 경우 내림차순으로 symbol 정렬
    for(i = 0; i < 100; i++){
        node = symhead[i];
		//내림차순이기 때문에 ASCII값을 비교하는 반복문
		while(node){
            ptr = follow = list;
            while(ptr && strcmp(ptr->name, node->name) > 0){
                //strcmp값이 양수이면 다음값과 비교
                follow = ptr;
                ptr = ptr->ptr;
            }
            //strcmp값이 음수이면 그 전 노드에 연결
            temp = allocate(node);
            temp->ptr = ptr;
            if(ptr == list){
                temp = allocate(node);
                temp->ptr = ptr;
                list = temp;
            }
            else    follow->ptr = temp;
        node = node->ptr;
        }
        ptr = list;
    }
	
	//내림차순으로 정렬된 SYMBOL 출력
    while(ptr){
        printf("\t%s\t%04X", ptr->name, ptr->location);
        ptr = ptr->ptr;
        printf("\n");
    }

}

// "symbol" command 수행 시 내림차순으로 정렬할 때 새로운 node에 메모리를 할당하는 함수
SYMBOL* allocate(SYMBOL* data){
    SYMBOL *node = (SYMBOL*)malloc(sizeof(SYMBOL));

    node->location = data->location;
    strcpy(node->name, data->name);
    node->ptr = data->ptr;

    return node;
}

// 2형식 object code계산을 위해 REGISTER NAME에 해당하는 REGISTER NUMBER를 반환하는 함수
int reg_num(char *reg){
	if(!strcmp(reg, "A")) return 0;
	else if(!strcmp(reg, "X")) return 1;
	else if(!strcmp(reg, "L")) return 2;
	else if(!strcmp(reg, "B")) return 3;
	else if(!strcmp(reg, "S")) return 4;
	else if(!strcmp(reg, "T")) return 5;
	else if(!strcmp(reg, "F")) return 6;
	else if(!strcmp(reg, "PC")) return 8;
	else if(!strcmp(reg, "SW")) return 9;
	else return -1;
}

// "assemble filename.asm" command 수행 해당 파일을 SIC/XE머신으로 assemble하는 함수
int command_assemble(char **command, OPCODE **ophead, SYMBOL ***output, int *symbol_print_flag){
    FILE *fp = fopen(command[1], "rt");
    FILE *lst, *obj, *mid;
    char *lstname = (char*)malloc(sizeof(char) * strlen(command[1]));
    char *objname = (char*)malloc(sizeof(char) * strlen(command[1]));
    char temp[INPUT_SIZE];
    char operator[10], operand1[255], operand2[10], format[5], optemp[10], name[10];
    int i, opcode, reg1, reg2, num, disp, pc = 0, base = 0, base_flag = 0, var;
    int first_data = 0, mid_data = 0, symbol_flag = 0, end_flag = 0, start_flag = 0, error_flag = 0;
    unsigned int line_num = 5, location = 0, length;
    SYMBOL **symhead = (SYMBOL**)malloc(sizeof(SYMBOL*) * 100), *node = (SYMBOL*)malloc(sizeof(SYMBOL));
    OBJECT *head = NULL, *tail = NULL, *ptr1, *ptr2;

    if(!fp){
        printf("There is no %s file\n", command[1]);
        return 0;
    }
	//없는 파일 load시 ERROR

    if(strcmp(command[1] + strlen(command[1]) - 4, ".asm")){
        printf("%s file is not assemble file\n", command[1]);
        return 0;
    }
	//.asm 파일 아닌 파일 load시 ERROR

    
	
	//pass1
    
	for(i = 0; i < 100; i++) symhead[i] = NULL;

    mid = fopen("mid.txt", "wt");

    *symbol_print_flag = 0;

    while(fgets(temp, sizeof(temp), fp)){
		
		if(error_flag) return 0;
		//ERROR_flag = 1 : error존재 따라서 종료

        strcpy(operand1, "");
        strcpy(operand2, "");
        strcpy(operator, "");
        strcpy(format, "");
        strcpy(node->name, "");
        strcpy(optemp, "");
		//initialize data

        //주석, 엔터 무시
        if(temp[0] == '.'){
            fprintf(mid, "=%4d\t%s",line_num, temp);
            line_num += 5;
            continue;
        }
        else if(temp[0] == '\n') continue;

        //SYMBOL 존재X
        else if(temp[0] == ' ' || temp[0] == '\t'){
            num = sscanf(temp, "%s %s %s", operator, operand1, operand2);
            if(num == 2 && strchr(operand1, ',')){
                strcpy(operand2, strchr(operand1, ',') + 1);
                for(i = 0; i < strlen(operand1); i++)
                    if(operand1[i] == ','){
                        operand1[i + 1] = '\0';
                        break;
                    }
                num = 3;
            }
        }

        //SYMBOL 존재O
        else{
            num = sscanf(temp, "%s %s %s %s", node->name , operator, operand1, operand2);
			if(!strcmp(operator, "BYTE") && operand1[0] == 'C' && num == 4){
				sscanf(temp, "%s %s %[^\n]s", node->name, operator, operand1);
				num = 3;
			}
            if(num == 3 && strchr(operand1, ',')){
                strcpy(operand2, strchr(operand1, ',') + 1);
                for(i = 0; i < strlen(operand1); i++)
                    if(operand1[i] == ','){
                        operand1[i + 1] = '\0';
                        break;
                    }
                num = 4;
            }
        }


		// intermediate 설계 방법
		// 파일 내의 각 줄의 첫문자로 모든 CASE를 분류
		//  '='   :   object code 계산 필요없다고 알림
		//  'S'   :   START 라고 알림
		//  'E'   :   END 라고 알림
		//  '!'   :   BYTE, WORD라고 알림
		//  '~'   :   RESW, RESB라고 알림
		//  'B'   :   BASE 라고 알림
		//  'N'   :   NOBASE 라고 알림
		//  '\t'  :   object code를 계산해야 한다고 알림
		// 나머지 :   실제 object code계산을 위한 데이터


		//directive 명령어 + RSUB + BASE + NOBASE 처리
        if(!strcmp(operator, "START")){
            fprintf(mid, "=%4d\t%04X\t%-10s\t%-10s\t0\nS%s\n", line_num, location, node->name, operator,node->name);
            line_num += 5;
			location = 0;
            node-> location = location;
            error_flag = build_symtab(symhead, node, line_num);
            continue;
        }

        else if(!strcmp(operator, "END")){
            fprintf(mid, "=%4d\t    \t%-10s\t%-10s\t%-20s\nE\n", line_num, node->name, operator, operand1);
            line_num += 5;
            break;
        }
        else if(!strcmp(operator, "BYTE")){
            node->location = location;
            if(operand1[0] == 'X'){
                if((strlen(operand1) - 3) % 2){
                    printf("LINE : %d HEXA DATA ERROR\n", line_num);
                    return 0;
                	//16진수 data인데 홀수개만 선언한 경우 ERROR
				}

                fprintf(mid, "%4d\t%04X\t%-10s\t%-10s\t%-20s\n!", line_num, location, node->name, operator, operand1);
                for(i = 2; i < strlen(operand1) - 1; i++){
					if(operand1[i] >= '0' && operand1[i] <= '9');
					else if(operand1[i] >= 'A' && operand1[i] <= 'F');
					else if(operand1[i] >= 'a' && operand1[i] <= 'f');
					else {
						printf("LINE : %d HEXA DATA ERROR\n", line_num);
						return 0;
					}
					fprintf(mid, "%c", operand1[i]);
				}
				fprintf(mid, "\n");
                error_flag = build_symtab(symhead, node, line_num);
                node->location = location = location + (strlen(operand1) - 3) / 2;
            }
            else if(operand1[0] == 'C'){
				fprintf(mid, "%4d\t%04X\t%-10s\t%-10s\t%-20s\n!", line_num, location, node->name, operator, operand1);
                for(i = 2; i < strlen(operand1) - 1; i++)   fprintf(mid, "%X", operand1[i]);
                fprintf(mid, "\n");
                error_flag = build_symtab(symhead, node, line_num);
                node->location = location = location + strlen(operand1) - 3;
            }
            line_num += 5;
            continue;
        }
        else if(!strcmp(operator, "WORD")){
            fprintf(mid, "%4d\t%04X\t%-10s\t%-10s\t%-20s\n!", line_num, location, node->name, operator, operand1);
            sscanf(operand1, "%d", &var);
			if(var >= -0x800000 && var <= 0x7FFFFF){
				if(var <= -1 && var >= -0x800000){
					var += 0x1000000;
				}
			}
			else {
				printf("LINE : %d WORD value overflow!\n", line_num);
				return 0;
			}
			//WORD value overflow처리
            fprintf(mid, "%06X\n", var);
            error_flag = build_symtab(symhead, node, line_num);
            node->location = location = location + 3;
            line_num += 5;
            continue;
        }
        else if(!strcmp(operator, "RESW")){
            sscanf(operand1, "%d", &var);
            fprintf(mid, "=%4d\t%04X\t%-10s\t%-10s\t%-20s\n~\n", line_num, location, node->name, operator, operand1);
            error_flag = build_symtab(symhead, node, line_num);
            node->location = location = location + 3 * var;
            line_num += 5;
            continue;
        }
        else if(!strcmp(operator, "RESB")){
            sscanf(operand1, "%d", &var);
            fprintf(mid, "=%4d\t%04X\t%-10s\t%-10s\t%-20s\n~\n", line_num, location, node->name, operator, operand1);
            error_flag = build_symtab(symhead, node, line_num);
            node->location = location = location + var;
            line_num += 5;
            continue;
        }

        else if(!strcmp(operator, "BASE")){
            fprintf(mid, "=%4d\t    \t%-10s\t%-10s\t%-20s\nB%s\n", line_num, node->name, operator, operand1, operand1);
            line_num += 5;
            continue;
        }
		
		else if(!strcmp(operator, "NOBASE")){
			fprintf(mid, "=%4d\t    \t%-10s\t%-10s\t\nN\n", line_num, node->name, operator);
			line_num += 5;
			continue;
		}
	
		//directive 이외에 opcode table에 없는 잘못된 mnemonic 입력시 ERROR
        else{
            if(!check_mnemonic(operator, format, &opcode, ophead)){
                printf("LINE : %d %s is not in OPCODE TABLE\n", line_num, operator);
                return 0;
            }
        }

		//directive 이외 opcode table에 있는 mnemonic일 경우 정상 처리
        node->location = location;
        if(strcmp(node->name, ""))      error_flag = build_symtab(symhead, node, line_num);

        location_counter(format, &location, operator);
        strcpy(optemp, operand1);

        if(strcmp(operand2, "")){
            optemp[strlen(optemp) - 1] = '\0';
            strcat(optemp, "\t");
            strcat(optemp, operand2);
            strcat(operand1, " ");
            strcat(operand1, operand2);
        }

        //intermediate 파일에 필요한 자료 모두 입력
        fprintf(mid, "%4d\t%04X\t%-10s\t%-10s\t%-20s\n\t%02X\t%2s\t%10s\t%10s\n", line_num, node->location, node->name, operator, operand1, opcode, format, operator, optemp);
        line_num += 5;
    }

    length = location;
    fclose(mid);

    strcpy(lstname, command[1]);
    lstname[strlen(command[1]) - 4] = '\0';
    lst = fopen(strcat(lstname, ".lst"), "wt");


    //pass2

	//LISTING FILE 작성

    mid = fopen("mid.txt", "rt");
    while(fgets(temp, sizeof(temp), mid)){

		//엔터있을 경우 출력양식 맞추기 위해 제거
        if(temp[strlen(temp) - 1] == '\n') temp[strlen(temp) - 1 ] = '\0';
        
		//첫문자가 !  : BYTE, WORD
        if(temp[0] == '!'){
            fprintf(lst, "%s\n", temp + 1);
            objectcode_insert(5, 0, 0, 0, location, &head, &tail, temp + 1, 0);
        }

		//첫문자가 S  : START
        else if(temp[0] == 'S'){
			strcpy(name, temp + 1);
			start_flag = 1;
		}
		
		//첫문자가 =  : objectcode 계산 불필요 따라서 바로 출력
		else if(temp[0] == '=') fprintf(lst, "%s\n", temp + 1);

		//첫문자가 N  : NOBASE
		else if(temp[0] == 'N') base_flag = 0;

		//첫문자가 B  : BASE
        else if(temp[0] == 'B') base_flag = 1;

		//첫문자가 E  : END
        else if(temp[0] == 'E') end_flag = 1;

		//첫문자가 ~  : RESB, RESW
        else if(temp[0] == '~') tail->enter_flag = 1; //링크드리스트에서 개행해야된다고 알려주기

		//첫문자가 탭 : obectcode 계산
        else if(temp[0] == '\t'){

            num = sscanf(temp, " %x %s %s %s %s ", &opcode, format, operator, operand1, operand2);

			//operand가 없는 경우 : RSUB, 1형식
            if(num == 3){
				
				//RSUB
                if(!strcmp(operator, "RSUB")){
                    fprintf(lst, "%-10s\n", "4F0000");
                    objectcode_insert(3, 0x4f, 0, 0, location, &head, &tail, "", 0);
                }

				//1형식
                else if(!strcmp(format, "1")){
                    pc = location + 1;
                    fprintf(lst, "%02X\n", opcode);
                    objectcode_insert(1, opcode, 0, 0, location, &head, &tail, "", 0);
                }

				//이외 경우 ERROR
                else{
                    printf("LINE : %d OPERAND ERROR\n", line_num);
					fclose(lst);
					remove(lstname);
                    return 0;
                }
            }
			
			//operand가 있는 경우 : 2, 3, 4형식
            else if(num >= 4){

				//2형식
                if(!strcmp(format, "2")){
                    pc = location + 2;
                    
					//operand가 하나인 2형식
					if(num == 4){
						if(strcmp(operator, "TIXR") && strcmp(operator, "CLEAR") && strcmp(operator, "SVC")){
							printf("LINE : %d FORMAT2 OPERAND ERROR\n", line_num);
							fclose(lst);
							remove(lstname);
							return 0;
						}

						if(!strcmp(operator, "SVC")) sscanf(operand1, "%d", &reg1);
						else reg1 = reg_num(operand1);

                        if(reg1 == -1){
                            printf("LINE : %d REGISTER ERROR\n", line_num);
							fclose(lst);
							remove(lstname);
                            return 0;
                        }
                        fprintf(lst, "%02X%01X0\n", opcode, reg1);
                        objectcode_insert(2, opcode, reg1*16, 0, location, &head, &tail, "", 0);
                    }

					//operand가 두개인 2형식
                    else if(num == 5){
						if(!strcmp(operator, "TIXR") || !strcmp(operator, "CLEAR") || !strcmp(operator, "SVC")){
							printf("LINE : %d FORMAT2 OPERAND ERROR\n", line_num);
							fclose(lst);
							remove(lstname);
							return 0;
						}

						if(!strcmp(operator, "SHIFTL") || !strcmp(operator, "SHIFTR")){
							reg1 = reg_num(operand1);
							sscanf(operand2, "%d", &reg2);
						}
						else{
							reg1 = reg_num(operand1);
	                        reg2 = reg_num(operand2);
						}

                        if(reg1 == -1 || reg2 == -1){
                            printf("LINE : %d REGISTER ERROR\n", line_num);
							fclose(lst);
							remove(lstname);
                            return 0;
                        }
                        fprintf(lst, "%02X%01X%01X\n", opcode, reg1, reg2);
                        objectcode_insert(2, opcode, reg1*16 + reg2, 0, location, &head, &tail, "", 0);
                    }
                }

				//3형식, 4형식
                else if(!strcmp(format, "3") || !strcmp(format, "4")){

                    if(!strcmp(format, "3")) pc = location + 3;
                    else if(!strcmp(format, "4")) pc = location + 4;
					

					//immediate addressing
                    if(operand1[0] == '#'){
                        first_data = opcode + 1;
                        strcpy(operand1, operand1 + 1);
                    }

					//indirect addressing
                    else if(operand1[0] == '@'){
                        first_data = opcode + 2;
                        strcpy(operand1, operand1 + 1);
                    }

					//simple addressing
                    else{
                        first_data = opcode + 3;
                    }
					
					//operand symbol location값 계산
                    symbol_flag = find_symbol(symhead, operand1);
                    

					//잘못된 operand symbol일 경우 ERROR
					if(symbol_flag == -1){
                        printf("LINE : %d SYMBOL ERROR\n", line_num);
						fclose(lst);
						remove(lstname);
                        return 0;
                    }

					//operand symbol이 상수인경우
                    else if(symbol_flag == -2){
                        sscanf(operand1, "%d", &disp);

						//4형식 overflow 체크
						if(!strcmp(format, "4")){
							if(disp < 0 || disp >= 0x100000){
								printf("LINE : %d disp overflow\n", line_num);
								fclose(lst);
								remove(lstname);
								return 0;
							}
							mid_data = 1;
						}

						//3형식 overflow 체크
						else if(!strcmp(format, "3")){
							if(disp < 0 || disp >= 0x1000){
	                            printf("LINE : %d disp overflow\n", line_num);
								fclose(lst);
								remove(lstname);
	                            return 0;
							}
                        }
                    }
					
					//operand symbol 상수가 아니고 정상 확인된 경우
                    else{

						//4형식
                        if(!strcmp(format, "4")){
                            disp = symbol_flag;
                            mid_data = 1;
                        }

						//3형식
						// PC -> BASE relative overflow check
                        else if(!strcmp(format, "3")){
                            disp = symbol_flag - pc;
                            if(disp >= -0x800 && disp <= 0x7FF){
                                if(disp >= -0x800 && disp <= -1){
                                    disp += 0x1000;
                                }
                                mid_data = 2;
                            }
                            else{
                                if(!base_flag){
                                    printf("LINE : %d BASE FLAG IS FALSE\n",line_num);
									fclose(lst);
									remove(lstname);
                                    return 0;
                                }
                                symbol_flag = find_symbol(symhead, operand1);
                                disp = symbol_flag - base;
                                if(disp >= 0 && disp <= 0xFFF)   mid_data = 4;
                                else{
                                    printf("LINE : %d disp overflow\n", line_num);
									fclose(lst);
									remove(lstname);
									return 0;
                                }
                            }
                        }
                    }
					
					//indexed mode
                    if(num == 5){
                        if(!strcmp(operand2, "X") || !strcmp(operand2, "x")) mid_data += 8;
                        else{
                            printf("LINE : %d %s must be X register\n", line_num, operand2);
							fclose(lst);
							remove(lstname);
                            return 0;
                        }
                    }

					//3형식 출력
                    if(!strcmp(format, "3")){
                        fprintf(lst, "%02X%01X%03X\n", first_data, mid_data, disp);
                        if(symbol_flag == -2)   objectcode_insert(3, first_data, mid_data, disp, location, &head, &tail, "#", 0);
                        else    objectcode_insert(3, first_data, mid_data, disp, location, &head, &tail, "", 0);
                    }

					//4형식 출력
                    else if(!strcmp(format, "4")){
                        fprintf(lst, "%02X%01X%05X\n", first_data, mid_data, disp);
                        if(symbol_flag == -2)   objectcode_insert(4, first_data, mid_data, disp, location, &head, &tail, "", 0);
                        else    objectcode_insert(4, first_data, mid_data, disp, location, &head, &tail, "#", 0);
                    }
                    first_data = mid_data = 0;
                }

                else{
                    printf("LINE : %d FORMAT ERROR\n", line_num);
					fclose(lst);
					remove(lstname);
                    return 0;
				}

				//LDB인 경우 B register값 저장 -> 차후 BASE 명령어 입력시 relative mode 활성화 위해
                if(strstr(operator, "LDB"))		base = symbol_flag;
            }
        }

        // 나머지 : objectcode 계산이 필요
        else{
            sscanf(temp, "%d %x", &line_num, &location);
            fprintf(lst, "%s", temp);
        }
    }
	
	//파일 내에 START가 없을 시 ERROR
	if(!start_flag){
		printf("START NOT FOUND\n");
		fclose(lst);
		remove(lstname);
		return 0;
	}

	//파일 내에 END가 없을 시 ERROR
    if(!end_flag){
        printf("END NOT FOUND\n");
		fclose(lst);
		remove(lstname);
        return 0;
    }


	//OBJECT FILE 작성

    ptr1 = ptr2 = head;
	
	strcpy(objname, command[1]);
	objname[strlen(command[1]) - 4] = '\0';
	obj = fopen(strcat(objname, ".obj"), "wt");
	
	//HEADER RECORD
	fprintf(obj, "H%-6s000000%06X\n", name, length);

	length = 0;
	

	//TEXT RECORD
    while(ptr1){

        fprintf(obj, "T%06X", ptr1->location);
	
		//한 LINE출력을 위해 LENGTH 계산
        while(ptr2){
            if(ptr2->format == 5){
                if(length + strlen(ptr2->var) / 2 > 30) break;
                else{
                    length = length + strlen(ptr2->var) / 2;
                    if(ptr2->enter_flag){
                        ptr2 = ptr2->ptr;
                        break;
                    }
                }
            }
            else{
                if(length + ptr2->format > 30) break;
                else{
                    length = length + ptr2->format;
                    if(ptr2->enter_flag){
                        ptr2 = ptr2->ptr;
                        break;
                    }
                }
            }
            ptr2 = ptr2->ptr;
        }
        //한 LINE 계산 완료시
		
		//출력
		fprintf(obj, "%02X", length);
        while(ptr1 != ptr2){
            if(ptr1->format == 1) fprintf(obj, "%02X", ptr1->first_data);
            else if(ptr1->format == 2) fprintf(obj, "%02X%02X", ptr1->first_data, ptr1->mid_data);
            else if(ptr1->format == 3) fprintf(obj, "%02X%01X%03X", ptr1->first_data, ptr1->mid_data, ptr1->end_data);
            else if(ptr1->format == 4) fprintf(obj, "%02X%01X%05X", ptr1->first_data, ptr1->mid_data, ptr1->end_data);
            else if(ptr1->format == 5) fprintf(obj, "%s", ptr1->var);
            ptr1 = ptr1->ptr;
        }
        fprintf(obj, "\n");
        length = 0;

    }

	ptr1 = head;

	//MODIFICATION RECORD
    while(ptr1){
        if(ptr1->format == 4 && strcmp(ptr1->var, "")) fprintf(obj, "M%06X05\n",ptr1->location + 1);
        ptr1 = ptr1-> ptr;
    }
	
	//END RECORD
    fprintf(obj, "E000000\n");
    

	// "assemble" command 정상 수행 종료 알림
    printf("output file : [%s], [%s]\n", lstname, objname);

    free(lstname);
    free(objname);
    lstname = objname = NULL;
    fclose(mid);
    fclose(fp);
    fclose(lst);
    fclose(obj);
    *output = symhead;
    *symbol_print_flag = 1;

    return 2;
}
