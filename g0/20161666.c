#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<unistd.h>
#include<sys/stat.h>
#include"20161666.h"

node *head = NULL;
hash table[21];

int Loc[lineMax];
int end_line;
int pc, b,x, no_base=1;
inst asmarr[lineMax];

char title[20];
int symbline = -1;
FILE *fp, *lst, *obj;
char file[20];
char file2[20];
char file1[20];
symbol symbtable[100];

int adr_sym(char* sym);
int convert_int(char* cont);
int type_file(char* file);
char convert_hex_char(int n);
int store_const(char* value, int line);
int reg_num(char* s);
void convert_hex_string(int n, char** result);
int make_obj(int line);
void make_obj_file(char* name);
int store_loc(int line, char* m, char* opr, char* opr2);

void assemble_start(char *filename);
int assemble(char *inp, int line);

int check_command(char *inp);//classify commands
int history();//make linked list of commmands
void add_list(char *com);//add command to history linked list
int printhist();//print histories of commands
void listdir(char *dir);

int convert_hex(char *temp2);
void make_hash_table();
void add_hash(int key, char *mntemp, char *optemp, char* typetemp);
int hash_function(char *mnemonic);
int print_opcode(char *mnemonic);
void print_opcode_list();
void print_symbol();

int count = 0;
int cont = 0;
int code_size=0;
char shell[65536][32];
char display[65536][16];

void listdir(char *dir);
void makedisplay();
void printshell(int l1, int h1, int l2, int h2);

int main()
{
	char inp[81], ch;
	char *res;
	int comstate, r, instlen = 0, i, j;

	for(i = 0; i < 65536; i++)
	{
		for(j = 0; j < 32; j++)
		{
			shell[i][j] = '0';
		}
	}// initializing shell array

	for(i = 0; i < 65536; i++)
	{
		for(j = 0; j < 16; j++)
		{
			display[i][j] = '.';
		}
	}//initializing display array
	for(i = 0; i < 20; i++)
	{
		table[i].num = i;
		table[i].next = NULL;
	}// initializing and numbering hash table

	make_hash_table();


	while(1)
	{
	
		printf("sicsim> ");
		
		for(i = 0; i < 80; i++)
			inp[i] = '\0';

		fgets(inp, 80, stdin);
		
		comstate = check_command(inp);
		//initialize input array -> inp[] and get input

		if(comstate == -1) break; //if input is "quit" then return
		
		else if(comstate == wrongcom)
		{
			printf("The command is wrong. Please enter a new command!!\n");
			continue;
		}
		else
		{
			if(comstate == histempty) continue;
		}//if the history linked list == NULL -> continue sicsim>
	}

	return 0;


}

int check_command(char *inp)
{
	int res, i, num = 0, index = 0, j, k, l, m, c1 = 0, ii = 0, iii = 0, c2 = 0, c, h1 = 0, h2 = 0, l1 = 0, l2 = 0, cc = 0;
	char *ptr = (char *)malloc(sizeof(char) * 80);
	char *temp1, *temp2, *temp3, *temp4, *temp5;
	char ch;
	temp1 = (char *)malloc(sizeof(char) * 10);
	temp2 = (char *)malloc(sizeof(char) * 10);
	temp3 = (char *)malloc(sizeof(char) * 10);
	temp4 = (char *)malloc(sizeof(char) * 10);
	temp5 = (char *)malloc(sizeof(char) * 10);

	//temp들은 모두 각각의 문자열들을 blank를 기준으로 나눠서 순서대로 가짐

	for(i = 0; i < 10; i++)
	{
		temp1[i] = '\0';
		temp2[i] = '\0';
		temp3[i] = '\0';
		temp4[i] = '\0';
		temp5[i] = '\0';
	}

	strcpy(ptr, inp);
	temp1 = strtok(ptr, " \n");

	while( ptr = strtok(NULL, " ,\n"))
	{
		if(num == 0)
		{
			strcpy(temp2, ptr);
			num++;
		}
		else if(num == 1)
		{
			strcpy(temp3, ptr);
			num++;
		}
		else if(num == 2)
		{
			strcpy(temp4, ptr);
			num++;
		}
		else if(num == 3)
		{
			strcpy(temp5, ptr);
			num++;
		}
	}
	//temp들은 모두 각각의 문자열들을 blank를 기준으로 나눠서 순서대로 가짐

	if((strcmp(temp1, "quit") == 0) || (strcmp(temp1, "q") == 0))		
	{
		if(strcmp(temp2, "\0") == 0)
			return -1;
		else
			return wrongcom;
	} //if command is "quit" -> exit the program
	else if((strcmp(temp1, "h") == 0) || (strcmp(temp1, "help") == 0))
	{
		if(strcmp(temp2, "\0") == 0)
		{
			printf("\nh[elp]\n");
			printf("d[ir]\n");
			printf("q[uit]\n");
			printf("hi[story]\n");
			printf("du[mp] [start, end]\n");
			printf("e[dit] address, value\n");
			printf("f[ill] start, end, value\n");
			printf("reset\n");
			printf("opcode mnemonic\n");
			printf("opcodelist\n\n");
			add_list(inp);
		} //if the command is 'help'
		else
			return wrongcom;
	} 
	else if((strcmp(temp1, "d") == 0) || (strcmp(temp1, "dir") == 0))
	{
		if(strcmp(temp2, "\0") == 0)//dir이후로 다른 문자열을 더 받으면 에러처리
		{
			listdir(".");
			add_list(inp);
		}
		else
			return wrongcom;
	}//print dir & file names
	else if((strcmp(temp1, "hi") == 0) || (strcmp(temp1, "history") == 0))
	{
		if(strcmp(temp2, "\0") == 0)//history 이후로 다른 요소를 더 받으면 에러처리
		{
			add_list(inp);
			res = history();
			return res;
		}
		else
			return wrongcom;
	}  //if the command is 'history' or 'hi'

	//du 
	else if((strcmp(temp1, "du") == 0) || (strcmp(temp1, "dump") == 0))
	{
		if(strcmp(temp2, "\0") == 0)
		{
			printshell(cont%16, cont / 16, (cont + 159) %16, (cont + 159) / 16);
			cont+=160;	
			add_list(inp);//history added
		}//"du" or "dump" 만 입력했을 때
		else if((strcmp(temp2, "\0") != 0) && (strcmp(temp3, "\0") == 0))
		{
			cont=convert_hex(temp2);
			printshell(cont%16, cont / 16, (cont + 159) %16, (cont + 159) / 16);
			cont+=160;	
			add_list(inp);
		} //dump 뒤에 start만 입력했을 경우

		else if((strcmp(temp2, "\0") != 0) && (strcmp(temp3, "\0") != 0) && (strcmp(temp4, "\0") == 0))
		{
			cont=convert_hex(temp2);
			printshell(cont%16, cont / 16, convert_hex(temp3) %16, convert_hex(temp3)/ 16);
			cont=convert_hex(temp3)+1;
			add_list(inp);
		} // dump 뒤에 start, end를 모두 입력했을 경우
		else if((strcmp(temp2, "\0") != 0) && (strcmp(temp3, "\0") == 0))
			return wrongcom;//나머지의 경우는 모두 에러처리
	}
	else if((strcmp(temp1, "e") == 0 || (strcmp(temp1, "edit") == 0)))
	{
		if((strcmp(temp2, "\0") != 0) && (strcmp(temp3, "\0") != 0) && (strcmp(temp4, "\0") == 0))
		{
			add_list(inp);
			if(convert_hex(temp3)>0xff)
				return wrongcom; //command가 범위를 넘으면 에러처리
			shell[convert_hex(temp2)/16][convert_hex(temp2)%16*2] = temp3[0];
			shell[convert_hex(temp2)/16][convert_hex(temp2)%16*2+1] = temp3[1];
			//shell[][]이 char 형 배열인데 가로 크기가 32이므로 가로 index는 *2, *2 + 1을 해줘서 입력받은 값을 넣어줘야함
			//높이는 입력받은 수를 16으로 나눈 값이고 가로길이는 16으로 나누었을 때 나머지이다
			if(temp3[0] >= '0' && temp3[0] <= '9')
			{
				ch += (temp3[0] - '0') *16;
			}//0~9사이의 수일 때 10진수 변환법
			else if(temp3[0] >= 'A' && temp3[0] <= 'F')
			{
				ch += (temp3[0] - 'A' + 10) *16;
			}
			else if(temp3[0] >= 'a' && temp3[0] <= 'f')
			{
				ch += (temp3[0] - 'a' + 10) *16;
			}//16진수의 십의 자리의 경우, 경우별로 나눠서 *16을 해서 더해준다
			else
				return wrongcom;
			
			if(temp3[1] >= '0' && temp3[1] <= '9')
				ch += (temp3[1] - '0');
			else if(temp3[1] >= 'A' && temp3[1] <= 'F')
				ch += (temp3[1] - 'A' + 10);
			else if(temp3[1] >= 'a' && temp3[1] <= 'f')
				ch += (temp3[1] - 'a' + 10);
			else
				return wrongcom;
			//16진법 1의 자리는 그냥 더해준다

			if((ch >= 32) && (ch <= 126))
				display[convert_hex(temp2)/16][convert_hex(temp2)%16] = ch;
			//char 형 ch를 display에 저장(for ascii code)
		}//edit start, end, dest 말고 다른 요소를 더 받으면 에러처리
		else
			return wrongcom;
	}
	else if(strcmp(temp1, "assemble") == 0){
		if(strcmp(temp2, "\0") != 0 && strcmp(temp3, "\0") == 0)
			assemble_start(temp2);
		else
			return wrongcom;
	}

	else if((strcmp(temp1, "f") == 0) || (strcmp(temp1, "fill") == 0))
	{
		if((strcmp(temp2, "\0")) != 0 && (strcmp(temp3, "\0")) != 0 && (strcmp(temp4, "\0") != 0) && (strcmp(temp5, "\0") == 0))
		{
			if(convert_hex(temp2)>0xff || convert_hex(temp3) > 0xff)
				return wrongcom;//범위 넘으면 에러

			add_list(inp);
			for(i = convert_hex(temp2); i <= convert_hex(temp3); i++)
			{
				ch = '\0';
				shell[i/16][i%16*2] = temp4[0];
				shell[i/16][i%16*2+1] = temp4[1];
				if(temp4[0] >= '0' && temp4[0] <= '9')
					ch += (temp4[0] - '0') *16;
				else if(temp4[0] >= 'A' && temp4[0] <= 'F')
					ch += (temp4[0] - 'A' + 10) *16;
				else if(temp4[0] >= 'a' && temp4[0] <= 'f')
					ch += (temp4[0] - 'a' + 10) *16;
				else
					return wrongcom;
			
				if(temp4[1] >= '0' && temp4[1] <= '9')
					ch += (temp4[1] - '0');
				else if(temp4[1] >= 'A' && temp4[1] <= 'F')
					ch += (temp4[1] - 'A' + 10);
				else if(temp4[1] >= 'a' && temp4[1] <= 'f')
					ch += (temp4[1] - 'a' + 10);
				else
					return wrongcom;

				if((ch >= 32) && (ch <= 126))
					display[i/16][i%16] = ch;
			}
		}//edit 과 같은 방식으로 이루어지지만 ch값이 일정하므로 마지막에 다같이 ch로 채워줌
		else
			return wrongcom;
	}
	
	else if(strcmp(temp1, "reset") == 0)
	{
		if((strcmp(temp2, "\0") == 0))
		{
			add_list(inp);
			for(i = 0 ; i < 65536; i++)
			{
				for(j = 0; j < 32; j++)
					shell[i][j] = '0';
				for(j = 0; j < 16; j++)
					display[i][j] = '.';
			}
		}
		else
			return wrongcom;
	}

	else if(strcmp(temp1, "opcode") == 0)
	{
		if(strcmp(temp3, "\0") == 0)
		{
			add_list(inp);
			for(i = 0; i < 10; i++)
			{
				if(temp2[i] != '\0')
					cc++;
				else
					break;
			 }//opcode의 자리수가 cc
			for(i = 0; i < cc; i++)
			{
				if((temp2[i] < 'A') || (temp2[i]) > 'Z')
					return wrongcom;
			}
			print_opcode(temp2);
		}
		else
			return wrongcom;
	}
	else if(strcmp(temp1, "opcodelist") == 0)
	{
		if(strcmp(temp2, "\0") == 0)
		{
			print_opcode_list();
			add_list(inp);
		}
		else
			return wrongcom;
	}

	else if(strcmp(temp1,"type")==0){
		if(strcmp(temp2, "\0") != 0 && strcmp(temp3, "\0") == 0)
		{
			type_file(temp2);
			add_list(inp);
		}
		else
			return wrongcom;
	}

	else if(strcmp(temp1,"symbol")==0)
	{
		if(strcmp(temp2, "\0") == 0)
		{
			print_symbol();
			add_list(inp);
		}
		else
			return wrongcom;
	}
	else
		return wrongcom;

}
	
void listdir(char *dir)
{
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;

	if((dp = opendir(dir)) == NULL)
	{
		printf("cannot open directory: %s\n", dir);
		return;
	}//directory가 null인 경우
	chdir(dir);

	while((entry = readdir(dp)) != NULL)
	{
		if(!strcmp(entry -> d_name, ".") || !strcmp(entry -> d_name, ".."))
			continue;
		printf("%s", entry -> d_name);
		//파일 이름 출력

		lstat(entry->d_name, &statbuf);

		if(S_ISDIR(statbuf.st_mode))
			printf("/\n");//디렉토리 출력
		else if(statbuf.st_mode & S_IXUSR)
			printf("*\n");//최근에 실행된 파일 
		else printf("\n");
	}
	closedir(dp);
}

int history()
{
	if(head->next == NULL)
		return histempty;
	else
	{
		printhist();
		return 0;
	}
}

void add_list(char *command)
{
	int i;
	node *nnew = NULL, *temp = NULL;

	nnew = (node *)malloc(sizeof(node));
	temp = (node *)malloc(sizeof(node));
	
	nnew->next = NULL;
	nnew->com = (char *)malloc(sizeof(char) * 81);
	for(i = 0; i < 81; i++)
	{
		(nnew->com)[i] = '\0';
	}
	strcpy(nnew->com, command);
	
	if(head == NULL)
	{
		count = 1;
		nnew->num = count;
		head = nnew;
	}//linked list가 null일때
	else
	{
		count++;
		nnew->num = count;
		for(temp = head; temp->next != NULL; temp = temp->next);
		temp->next = nnew;
	}//마지막에 추가해줌
	return;
}

int printhist()
{
	node *temp = NULL;
	temp = (node *)malloc(sizeof(node));
	for(temp = head; temp->next != NULL; temp = temp->next)
	{
		printf("%4d %s\n", temp->num, temp->com);
	}

	printf("%4d %s\n", temp->num, temp->com);
	return 0;
}

void make_hash_table()
{
	int i, count = 0, key, j;
	char *com = (char *)malloc(sizeof(char) * 41);
	char *str = (char *)malloc(sizeof(char) * 41);
	char *optemp = (char *)malloc(sizeof(char) * 4);
	char *mntemp = (char *)malloc(sizeof(char) * 10);
	char *typetemp = (char *)malloc(sizeof(char) * 5);
	
	hnode *temp = (hnode *)malloc(sizeof(hnode));
	
	temp->next = NULL;

	FILE *fp;
	fp = fopen("opcode.txt", "r");
	for(i = 0; i < 58; i++)
	{
		fgets(com, 40, fp);
		count = 0;
		str = strtok(com, " \t");
		strcpy(optemp, str);//optemp = opcode
		while(str = strtok(NULL, " \n\r\t"))
		{
			if(count == 0)
			{
				strcpy(mntemp, str);
				count++;
			} else if (count==1){
				strcpy(typetemp,str);
				count++;
			}
		}//mntemp = mnemonic
		key = hash_function(mntemp);
	
		for(j = 0; j < 20; j++)
		{
			if(table[j].num == key)
			{
				add_hash(key, mntemp, optemp, typetemp);
			}
		}//key = j 일 때 table[j]에 노드 추가
	}
	fclose(fp);
}

void add_hash(int key, char *mntemp, char *optemp, char *typetemp)
{
	hnode *nnew = (hnode *)malloc(sizeof(hnode));
	hnode *temp = (hnode *)malloc(sizeof(hnode));
	nnew->next = NULL;
	strcpy(nnew->op, optemp);
	strcpy(nnew->m, mntemp);
	strcpy(nnew->type, typetemp);
	if (!strcmp(typetemp, "1")) {
		nnew->typenum = TYPE1;
	} else if (!strcmp(typetemp, "2")) {
		nnew->typenum = TYPE2;
	} else if (!strcmp(typetemp, "3/4")) {
		nnew->typenum = TYPE34;
	}

	if(table[key].next == NULL) {
		table[key].next = nnew;
	} else {
		nnew->next = table[key].next;
		table[key].next = nnew;
	}
}//hash table에 노드 하나 추가

int hash_function(char *mnemonic)
{
	int i, res, co = 0;
	for(i = 0; i < 12; i++)
	{
		if(mnemonic[i] == '\0')
			break;
		else
			co++;
	} // co는 몇 글자인지 알려준다
	res = ((mnemonic[0] + mnemonic[co - 1]) % 20);
	return res;
	//key = mnemonic의 첫글자와 마지막 글자의 아스키코드를 더해서 20으로 나눈 나머지
}

int print_opcode(char *mnemonic)
{
	int i, key, co = 0;
	hnode *temp = (hnode *)malloc(sizeof(hnode));
	char newtemp[10];
	key = hash_function(mnemonic);
	
	for(i = 0; i < 3; i++)
	{
		(temp->op)[i] = '\0';
	}

	for(i = 0; i < 10; i++)
	{
		(temp->m)[i] = '\0';
	}
	//initialize
	for(temp = table[key].next; temp->next != NULL; temp = temp->next)
	{
		if(strcmp(temp->m, mnemonic) == 0)
			break;
	}
	//mnemonic과 hash table의 m이 같으면 노드를 찾아서 opcode print

	printf("opcode is %s\n", temp->op);
	
	return 0;
}

void print_opcode_list()
{
	int i;
	hnode *temp = (hnode *)malloc(sizeof(hnode));

	for(i = 0; i < 20; i++)
	{
		printf("%3d : ", i);
		for(temp = table[i].next; ; temp = temp->next)
		{
			printf("[%s, %s] ", temp->m, temp->op);
			if(temp->next != NULL)
				printf("-> ");
			else break;
		}//hash table 을 포인터로 접근해서 순서대로 출력
		printf("\n");
	}	
}
void printshell(int l1, int h1, int l2, int h2)
{
	int i = 0, j = 0;
	for(i = h1; i <= h2; i++)
	{
		printf("%05X", i*16);
		printf(" ");
		for(j = 0; j < 16; j++)
		{
			if( (i==h1 && j < l1) || (i==h2 && j >l2))
				printf("   ");
			else
				printf("%c%c ", shell[i][2*j], shell[i][2*j + 1]);
		}//l1, l2 -> 가로의 경계 index, h1, h2 -> 높이의 경계 index
		printf("; ");
		for(j = 0; j < 16; j++)
		{
			if((j >= l1) && (j <= l2))
				printf("%c", display[i][j]);
			else
				printf(".");
		}
		printf("\n");
	}
}

void makedisplay()
{
	int i, j;
	int a, b;
	char ch;
	for(i = 0; i < 65536; i++)
	{
		for(j = 0; j < 16; j++)
		{
			if((shell[i][2*j] >= '0') && (shell[i][2*j] <= '9'))
				a = shell[i][2 * j] - '0';
			else if((shell[i][2 * j] >= 'A') && (shell[i][2 * j] <= 'F'))
				a = shell[i][2 * j] - 'A';
			//2*j의 index를 가지는 shell의 char을 경우에 따라 분류해서 숫자로 변경

			if((shell[i][2*j + 1] >= '0') && (shell[i][2*j + 1] <= '9'))
				b = shell[i][2 * j + 1] - '0';
			else if((shell[i][2 * j + 1] >= 'A') && (shell[i][2 * j + 1] <= 'F'))
				b = shell[i][2 * j + 1] - 'A';
			ch = 16 * a + b;
			if((ch >= 32) && (ch <= 126))
				display[i][j] = ch;//ascii code값
			else
				display[i][j] = '.';//나머지는 '.' print
		}
	}

}
int convert_int(char* cont){
	int i,num=0;
	int result=0;
	for(i=0; i<strlen(cont); i++){
		num=cont[i]-'0';
		if( num<0 || num>9)
			return -1;
		result= result*10 + num;
	}
	return result;
}
int convert_hex(char* cont){
	int i,num=0;
	int flag=-1;
	for(i=0; i<strlen(cont); i++){
		num=num*16;//자리수마다 16을 곱해서 더해줌
		if(cont[i] >='A' && cont[i]<='F')
			num += cont[i]-'A'+10;
		
		else if(cont[i]>='a' && cont[i] <='f')
			num+=cont[i]-'a'+10;

		else if(cont[i]>='0' && cont[i] <='9')
			num+=cont[i]-'0';

		else{
			flag=1;
			break;
		}
	}
	//16진수모양 char을 숫자로 바꿔
	if(flag==1)
		return -1; 
	return num;
}
void assemble_start(char *filename)
{
	int i, j;
	char *inp = (char *)malloc(sizeof(char) * 100);
	fp = fopen(filename, "r");
	if( fp==NULL){
		printf("File does not exist\n");
		return;
	}
	for(i = 0; i < 100; i++)
		inp[i] = 0;//1 line of the .asm file

	Loc[0]=0;
	b=0;
	no_base=1;
	x=0;
	code_size=0;
	end_line=0;
	strcpy(title, "JIZERO");

	for(i = 0; i < 200; i++)
	{
		if(fgets(inp, 100, fp)==NULL)
			break;
		
		asmarr[i].n = 1;
		asmarr[i].i = 1;
		asmarr[i].x = 0;
		asmarr[i].type = 0;
		asmarr[i].opcode = 0;
		asmarr[i].op2_flag = 0;
		
		//if the line of the .asm file has two elements -> then op2_flag == 1
		//if there are three elements, -> op2_Flag == 0
		asmarr[i].info = 0;
		
		//if the sentence contains "." -> info == 1 else info == 0
		asmarr[i].adr = 0;
		
		//the locctr of each line
		for(j = 0; j < 30; j++)
		{
			asmarr[i].mn[j] = 0;
			asmarr[i].opr[j] = 0;
			asmarr[i].opr2[j] = 0;
		}
		//if there is "," then the element that is after ","is saved to opr2
		for(j = 0; j < 8; j++)
			asmarr[i].obj[j] = 0;
		//obj code
		for(j = 0; j < 80; j++)
			asmarr[i].word[j] = 0;
		
		//if the line's info == 1 -> all of the information is in word

		assemble(inp, i);
	} //initialize and call assemble func for every lines of .asm file	

	end_line = i;
	
	for(j = 0; j < 20; j++){
		file[j] = 0;
		file1[j] = 0;
		file2[j] = 0;
	}
	for(j = 0; ; j++){
		if(filename[j] == '.')
			break;
		file[j] = filename[j];
	}//file -> part of the filename before "."

	strcpy(file1, file);
	strcpy(file2, file);
	strcat(file1, ".lst");//file1 -> 2_5.lst
	strcat(file2, ".obj");//file2 ->2_5.obj
	printf("%s %s\n", file1, file2);
	lst = fopen(file1, "w");
//	obj = fopen(file2, "w");

	fclose(fp);
	fp = fopen(filename, "r");

	i = 0;
	
//	while(fgets(inp, 100, fp) != NULL)
//	{


	while(fgets(inp, 100,fp)!=NULL)
	{
		if(inp == NULL)
			break;
		
		if(inp[strlen(inp) - 1] == '\n')
			inp[strlen(inp) - 1] = '\0';

		if(asmarr[i].info == 1)
			fprintf(lst, "%04d\t    \t%s", (i + 1)*5, inp);//(i + 1) *5 == line
		else
			fprintf(lst, "%04d\t%04X\t%-50s", (i + 1)*5, asmarr[i].adr, inp);

		if(asmarr[i].info != 1 && (asmarr[i].type <= 4 || asmarr[i].type == WORD || asmarr[i].type == BYTE))
		{
			make_obj(i);
			fprintf(lst, "%10s", asmarr[i].obj);
		}
		fprintf(lst, "\n");
		i++;
	}

	fclose(lst);
	fclose(fp);
	make_obj_file(file2); //make 2_5.obj
	fclose(obj);
}

int assemble(char *inp, int line)
{
	int i,num=0;
	char *temp1, *temp2, *temp3, *temp4, *temp5;
	char *ptr = (char *)malloc(sizeof(char) * 80);
	char ch;
	temp1 = (char *)malloc(sizeof(char) * 10);
	temp2 = (char *)malloc(sizeof(char) * 10);
	temp3 = (char *)malloc(sizeof(char) * 10);
	temp4 = (char *)malloc(sizeof(char) * 10);
	temp5 = (char *)malloc(sizeof(char) * 10);

	//temp들은 모두 각각의 문자열들을 blank를 기준으로 나눠서 순서대로 가짐

	for(i = 0; i < 10; i++)
	{
		temp1[i] = '\0';
		temp2[i] = '\0';
		temp3[i] = '\0';
		temp4[i] = '\0';
		temp5[i] = '\0';
	}

	strcpy(ptr, inp);
	
	if(!strcmp(ptr,"\n"))
		return 0;

	temp1 = strtok(ptr, " \n");
	
	if(temp1 == NULL)
		return 0;//temp1 -> the first word that is in the line of the .asm file
	
	while( ptr = strtok(NULL, " ,\n"))
	{
		if(num == 0)
		{
			strcpy(temp2, ptr);//second word
			num++;
		}
		else if(num == 1)
		{
			strcpy(temp3, ptr);//third word
			num++;
		}
		else if(num == 2)
		{
			strcpy(temp4, ptr);
			num++;
		}
		else if(num == 3)
		{
			strcpy(temp5, ptr);
			num++;
		}
	}
/*
	for(i = 0; i < 10; i++)
		title[i] = 0;//the title of the program

	if(line == 0)
	{
		strcpy(title, temp1);
		strcpy(asmarr[line].mn, temp2);
		strcpy(asmarr[line].opr, temp3);
	}//if it is the first line
	*/
	
	if(inp[0] == ' ')
	{
		asmarr[line].op2_flag = 1;//if the first is space ' ' == there is two elements in the line
		strcpy(asmarr[line].mn, temp1);//mnemonic
		strcpy(asmarr[line].opr, temp2);//operand
	}
	else if(temp1[0] == '.')
	{
		asmarr[line].info = 0;//주석
		if(temp2 == NULL)
		{
			strcpy(asmarr[line].word, temp1);
		}
		else
			strcpy(asmarr[line].word, temp2);
	}
	
	else
	{
		asmarr[line].op2_flag = 0;
		strcpy(asmarr[line].mn, temp2);
		strcpy(asmarr[line].opr, temp3);
		
		if(strcmp(temp2, "START")==0){
			strcpy(title, temp1);
		}
		else{
			symbline++;//symbline is the global variable and it means each of the symbol table line
			strcpy(symbtable[symbline].name, temp1);
			symbtable[symbline].adr = Loc[line];//symbtable -> symbol table
		}
	}
	
	if(line != 0)
	{
		if(asmarr[line].op2_flag == 1 && temp3 != NULL)//it means the operand contains ","
		{
			strcpy(asmarr[line].opr2, temp3);
			//if(strcpy(temp3, "X") == 0)
			if(temp3[0] =='X' )
				asmarr[line].x = 1;
		}
	
		else if(asmarr[line].op2_flag == 0 && temp4 != NULL)//it means the operand contains ","
		{
			strcpy(asmarr[line].opr2, temp4);	
			
			//if(strcpy(temp4, "X") == 0)
			if(temp4[0] == 'X')
				asmarr[line].x = 1;
		}

		if(asmarr[line].opr[0] == '@')
		{
			asmarr[line].n = 1;
			asmarr[line].i = 0;
		}
		else if(asmarr[line].opr[0] == '#')
		{
			asmarr[line].n = 0;
			asmarr[line].i = 1;
		}
	}

	//Loc, type 
	store_loc(line, asmarr[line].mn, asmarr[line].opr, asmarr[line].opr2);
}
void print_symbol(){
	int i, j, temp2;
	char temp[30];

	if( symbline==-1){
		printf("Symbol table does not exist\n");
		return;
	}

	for(i=0; i<=symbline; i++){
		for(j=i+1; j<=symbline; j++){
			if( strcmp(symbtable[i].name, symbtable[j].name)<0){
				strcpy(temp,symbtable[j].name);
				strcpy(symbtable[j].name, symbtable[i].name);
				strcpy(symbtable[i].name, temp);
				temp2=symbtable[j].adr;
				symbtable[j].adr=symbtable[i].adr;
				symbtable[i].adr= temp2;
			}
		}
	}

	for(i=0; i<=symbline; i++){
		printf("\t%s\t%04X\n", symbtable[i].name, symbtable[i].adr);
	}
}

int type_file(char* file){
	FILE* fp;
	char s[300];
	if(file==NULL)
		return 0;

	fp= fopen(file,"r");
	if(fp==NULL){
		printf("Does not exist\n");
		return 0;
	}

	while(fgets(s, sizeof(s), fp)!=NULL)
		printf("%s", s);

	return 0;	
}

char convert_hex_char(int n){
	if( n>=0 && n<=9)
		return n+'0';

	else if( n>=10 && n<16){
		return (n-10)+'A';
	}
}//decimal number -> returns hex char

int store_const(char* value, int line){
	int i,size=1,asc=0;
	char c, *tmp;
	
	if(asmarr[line].type==WORD)
		size=3;

	//문자
	if(value[0]=='C' || value[0]=='c'){
		value++;
		if(value[0]!='\'')
			return 0;
		value++;
		tmp= strtok(value, "'");
		if( tmp==NULL)
			return 0;
		
		if(strtok(NULL," \r\t\n")!=NULL)
			return 0;

		for(i=0; i<strlen(tmp); i++){
			asc= (int)tmp[i];
			c=convert_hex_char(asc/16);
//////			printf("@@@@@%d %c\n\n\n",asc/16, c); 
			if(c=='\0')
				return 0;
			(asmarr[line].obj)[2*i]=c;
		
			c=convert_hex_char(asc%16);
			if(c=='\0')
				return 0;
			(asmarr[line].obj)[2*i+1]= c;
		}
		(asmarr[line].obj)[2*i]='\0';
		return 1;
	}

	else if(value[0]=='x' || value[0] =='X'){
		value++;
		if(value[0]!='\'')
			return 0;
		value++;
		tmp= strtok(value, "'");
		if( tmp==NULL)
			return 0;
		if(strtok(NULL," \r\t\n")!=NULL)
			return 0;

		if(convert_hex(tmp)==-1)
			return 0;
		for(i=0; i<strlen(tmp); i++){
			(asmarr[line].obj)[i] = tmp[i];
		}
	}
	return 1; 
}
int reg_num(char* s){
	if(!strcmp(s,"A"))
		return A;
	if(!strcmp(s,"X"))
		return X;
	if(!strcmp(s,"L"))
		return L;
	if(!strcmp(s,"B"))
		return B;
	if(!strcmp(s,"S"))
		return S;
	if(!strcmp(s,"T"))
		return T;
	if(!strcmp(s,"F"))
		return F;
}
void convert_hex_string(int n, char** result){
	int i=0,tmp=n,digit=1, exp=1;
	
	while(tmp >= 16){
		tmp = tmp/16;
		exp *= 16;
		digit++;
	}	
	for(i=0; i<digit; i++){
		(*result)[i]= convert_hex_char(n/exp % 16);
		exp= exp/16;
	}
	(*result)[i]='\0';
}
int adr_sym(char* sym){
	int i;
	for(i=0; i<=symbline; i++){
		if( strcmp(symbtable[i].name, sym)==0){
			return symbtable[i].adr;
		}
	}

	return -1;
}
int make_obj(int line){
	int base,p,e, flag=0, max=0xfff, immediate, indexed;
	int opni=0, num=0, disp, tmp, k;
	char disp_c_buffer[10], *opr, *opr2;
	char *disp_c = disp_c_buffer;

	//obj code 따로 생성할 필요 없는 경우 (상수는 미리 obj 코드 생성)
	if( asmarr[line].type >4)
		return 1;

	if(asmarr[line].type<3){
		asmarr[line].n=0;
		asmarr[line].i=0;
	}
	opr= asmarr[line].opr;
	opr2= asmarr[line].opr2;
	opni= asmarr[line].opcode + asmarr[line].n*2 + asmarr[line].i;
	asmarr[line].obj[0]= convert_hex_char(opni/16);
	asmarr[line].obj[1]= convert_hex_char(opni%16);

	//base 설정 
	if(strcmp(asmarr[line].mn, "LDB")==0){
		if( adr_sym(asmarr[line].opr + 1)==-1)
			return 0;
		b = adr_sym(asmarr[line].opr + 1);
		no_base = 0;
		disp= b;
	}

	if( strcmp(asmarr[line].mn, "RSUB")==0){
		if( opr[0]!='\0')
			return 0;

		for(k=0; k<4; k++)
			(asmarr[line].obj)[k+2] = '0';
		return 1;
	}

	//format 1,2
	if( asmarr[line].type <3){
		//foramt 1
		if(asmarr[line].type==1){
			asmarr[line].obj[2]='\0';
			return 1;
		}

		asmarr[line].obj[2]= reg_num(asmarr[line].opr)+'0';
		asmarr[line].obj[3]= '0';

		//매개변수 2개인 경우 
		if((asmarr[line].opr2)[0]!='\0')
			asmarr[line].obj[3]= reg_num(asmarr[line].opr2)+'0';

		asmarr[line].obj[4]= '\0';
		return 1;
	}

	//disp계산
	immediate = asmarr[line].n==0 && asmarr[line].i ==1;
	indexed = asmarr[line].n==1 && asmarr[line].i ==0;
	if (immediate || indexed) opr++;

	if (adr_sym(opr) == -1 && !immediate) {
		return 0;
	}

	disp= adr_sym(opr);		
	disp-= asmarr[line+1].adr;
	base=0;
	p=1;
	e = (asmarr[line].type) == 4;

	if(e == 1) {
		max=0xfffff;
	}

	if(disp>max|| (disp<0 && -disp>max)){
		//base 지정하지 않은 경우
		if (no_base) {
			return 0;
		}

		disp = adr_sym(opr)-b;
		b=1;
		p=0;

		//base relative에서 disp 음수면 에러
		if( disp<0) {
			return 0;
		}
	}
	//pC 에서 음수이면 2's
	else if(disp<0){
		disp = 0x1000+disp;
	}	

	if(asmarr[line].x==1)
		disp+= x;
	

	if (immediate && adr_sym(opr) == -1) {
		if (convert_int(opr) == -1) {
			return 0;
		}
		disp = convert_int(opr);
		b = 0; p = 0;
	}
	
	//operand 주소값 그대로 입력하는 경우 (format 4의 simple addressing
	if(asmarr[line].n==1 && asmarr[line].i==1 && asmarr[line].type==4) {
		disp = adr_sym(opr);
		b = 0; p = 0;
	}

	// 범위 초과
	if(disp>max || (disp<0 && -disp>max))
		return 0;

	tmp= (asmarr[line].x) * 8 + base * 4 + p * 2 + e;
	asmarr[line].obj[2] = convert_hex_char(tmp);

	if (e) sprintf(asmarr[line].obj+3, "%05X", disp);
	else sprintf(asmarr[line].obj+3, "%03X", disp);

	return 1;
}
int store_loc(int line, char* m, char* opr, char* opr2){
	int opcode, size=0, i, flag = 0;
	char* temp2;
	asmarr[line].adr= Loc[line];

	Loc[line+1]=Loc[line];
	
	if(strcmp(m,"START")==0){
		Loc[0]= convert_hex(opr);
		Loc[1]= Loc[0];
		asmarr[0].type= START;
		return 1;
	}
	if(strcmp(m,"END")==0){
		asmarr[line].type= END;
		end_line=line;
		return 1; 
	}
	
	if(strcmp(m, "BYTE")==0){
		asmarr[line].type = BYTE;
		store_const(opr, line);
		Loc[line+1]+=strlen(asmarr[line].obj)/2;
		return 1;
	}

	if(strcmp(m,"WORD")==0){
		asmarr[line].type = WORD;
		Loc[line+1]+= 3;
		store_const(opr,line);
		return 1;
	}

	if(strcmp(m,"RESB")==0){
		asmarr[line].type=RESB;
		
		for (i = 0; i < 10; ++i) {
			printf("OPR?? %s\n", opr);
		}
		size = convert_int(opr);
		printf("HEX: %d\n", size);
		
		Loc[line+1]+= size;
		return 1;
	}

	if(strcmp(m,"RESW")==0){
		asmarr[line].type=RESW;
		size= convert_hex(opr);
		Loc[line+1]+=size*3;
		return 1;
	}

	//format 4
	if(m[0] =='+'){
		asmarr[line].type=4;
		m++;
	}
	hnode *temp = (hnode *)malloc(sizeof(hnode));
	for(i = 0; i < 20; i++){
		for(temp = table[i].next; temp; temp = temp->next){
			if(strcmp(m,temp->m)==0){
				printf("FOUND %s - %d\n", m, temp->typenum);
				// temp->typenum = TYPE1 or TYPE2 or TYPE34
				if (asmarr[line].type != 4) {
					asmarr[line].type = temp->typenum;
				}
				asmarr[line].opcode= convert_hex(temp->op);
				Loc[line+1]+= asmarr[line].type;
				flag = 1;
			}
		}
	}
	if(!flag)
		asmarr[line].info = 1;
	
	return 1;
}

void make_obj_file(char*name){
	int i,j, size, k;
	
	obj = fopen(name, "w");
	size= Loc[end_line]- Loc[0];
	fprintf(obj,"H%-6s%06X%06X\n", title,Loc[0],size);
	
	for(i=1; i<end_line;){
		if( asmarr[i].type >4 && asmarr[i].type!=WORD && asmarr[i].type !=BYTE)
		{
			i++;
			continue;
		}
		
		size=0;
		for(j=i; j<end_line; j++){
			if( asmarr[j].type==RESB ||asmarr[j].type ==RESW){
				break;
			}

			if(asmarr[j].type ==BYTE){
				if(size+strlen(asmarr[j].obj)/2>0x1E)
					break;
				size+=strlen(asmarr[j].obj)/2;
			}
			if(asmarr[j].type==WORD){
				if(size+3>0x1E)
					break;
				size+=3;
			}
			if(asmarr[j].type==2){
				if(size+2>0x1E)break;
				size+=2;
			}
			else if(asmarr[j].type==3){
				if(size+3>0x1E)break;
				size+=3;
			}
			else if(asmarr[j].type==4){
				if(size+4>0x1E)break;
				size+=4;
			}
		}
		fprintf(obj,"T%06X%02X",asmarr[i].adr,size);
		for(k=i;k<j;k++){
			fprintf(obj,"%s",asmarr[k].obj);
		}

		fprintf(obj, "\n");
		i=j;

	}

	for(i=1;i<end_line;i++){
		if(asmarr[i].type==4){
			fprintf(obj,"M%06X05\n",asmarr[i].adr+1);
		}
	}
	fprintf(fp, "E%06X\n", Loc[0]);
}

