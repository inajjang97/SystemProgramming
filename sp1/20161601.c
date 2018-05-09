#include "20161601.h"

//Linked List 
typedef struct LinkedList{
	int cnt;
	struct Node* front;
	struct Node* rear;
}ll;

typedef struct Node{
	char command[COMMAND_MAX];
	struct Node* next;
}node;

typedef struct OP{
	int op_num;
	char op_name[20];
	char op_type[10];
	struct OP* next;
}op;

op* oplist[HASH_MAX];	//opcode hash table을 위한 linked list
ll* hlist;	//history 저장 linked list
char memory[MEMORY_ROW][MEMORY_COL];
int cur_adr=0; // dump 수행시 시작 메모리 번지 

char* remove_space(char* s) {
	int i;
	if (s == NULL)
		return s;

	//앞쪽 공백 제거
	for (i = 0; i < strlen(s); i++) {
		if (s[i] != ' ' && s[i] != '\t' && s[i] != '\n')
			break;
	}
	s += i;
	//뒤쪽 공백 제거
	for (i = strlen(s); i > 0; i--) {
		if (s[i]!='\0' && s[i] != ' ' && s[i] != '\t' && s[i] != '\n')
			break;
		s[i] = '\0';
	}
	return s;
}
void print_error(int error){
	if(error==WRONG_ADR)
		printf("Wrong address\n");
	else if(error==WRONG_VAL)
		printf("Wrong Value\n");
	else if(error==WRONG_PARAMETER)
		printf("Wrong parameter\n");
	else if(error==WRONG_CMM)
		printf("Wrong Command\n");
	else if(error==INPUT_SIZE_OVER)
		printf("Input size is over\n");
}
void insert(char cmm[COMMAND_MAX]){
	//새로운 노드 할당
	node* newNode = (node*)malloc(sizeof(node));
	newNode->next=NULL;
	strcpy(newNode->command, cmm);
	
	//처음으로 노드 추가하는 경우
	if(hlist==NULL){
		hlist = (ll*)malloc(sizeof(ll));
		hlist->front=hlist->rear=newNode;
		hlist->cnt=0;
	}
	else{
		(hlist->rear)->next= newNode;
		hlist->rear=newNode;
	}
	(hlist->cnt)++;
}
//명령어 입력 받고, 해당 명령어에 해당하는 함수 호출
//quit 명령어를 받았을 때만 q(3) return 하고 , 나머지는 return 0
int prompt(){
	int i=0,flag=0;	
	char command_words[10][COMMAND_MAX]; // (공백 제거된)명령어를 이루는 문자열 저장 배열 
	char command[COMMAND_MAX]; // 입력받은 실제 명렁어 
	char cmm[COMMAND_MAX];
	char *tmp =NULL,ch=-1;

	printf("sicsim> ");
	// 명령어 입력 받음(공백 포함)
	while(1){
		scanf("%c", &ch);
		if(ch=='\n')
			break;
		command[i++]=ch;
		//input 최대 글자수 초과인 경우
		if(i==COMMAND_MAX){
			print_error(INPUT_SIZE_OVER);
			flag=1;
		}
	}
	//input size 초과인 경우 종료 
	if(flag==1)
		return 0;
	
	command[i]='\0';
	i=0;	// 여기서부터 i = 명령어에 포함된 문자열 덩어리 개수
	strcpy(cmm,command);
	tmp= strtok(cmm," \r\n\t"); // ' ' 기준으로 자른 문자열 
	strcpy(command_words[i++],tmp);
	
	while(i<6){
		tmp = strtok(NULL,",");	// ',' 단위로 자른 문자열
		if(tmp==NULL)
			break;
		tmp=remove_space(tmp);
		strcpy(command_words[i++],tmp);
	}
	
	//잘못된 명령어(i 의 최대값 = 4)
	if(i>4){
		print_error(WRONG_CMM);
		return 0;
	}

	//해당 명령어에 해당하는 함수 수행
	if( strcmp(command, "q") ==0 || strcmp(command,"quit")==0){
		command_q();
		return q;
	}
	
	if( strcmp(command, "h") ==0 || strcmp(command,"help")==0)
		command_h();

	else if( strcmp(command,"d") ==0 || strcmp(command,"dir")==0)
		command_d();

	else if( strcmp(command_words[0],"du")==0 || strcmp(command_words[0],"dump")==0){
		if(!command_du(command_words, i)) // 예외 발생 시 종료( hisory 기록 안됨)
			return 0;
	}
	
	else if(strcmp(command_words[0],"e")==0 || strcmp(command_words[0],"edit")==0){
		//잘못된 형식으로 입력한 경우 예외처리 후 함수 종료
		if(i!=3){
			print_error(WRONG_PARAMETER);
			return 0;
		}

		if(!command_e(command_words))
			return 0;
	}
	
	else if(strcmp(command_words[0],"f")==0 || strcmp(command_words[0],"fill")==0){
		if(i!=4){
			print_error(WRONG_PARAMETER);
			return 0;
		}
		if(!command_f(command_words))
			return 0;
	}

	else if( strcmp(command, "reset") ==0)
		command_reset();
	
	else if( strcmp(command_words[0], "opcode")==0){
		if(i>2){
			print_error(WRONG_PARAMETER);	//op_mn에 띄어쓰기 포함인 경우 -> 예외처리
			return 0;
		}
		if(!command_op_mn(command_words[1]))	//해당op_mn이 없는 경우
			return 0;
	}
	else if( strcmp(command, "opcodelist") ==0)
		command_op_list();

	//입력 받은 명령어가 history도 아니면, 정의되지 않은 명령어 -> 예외처리
	else if( strcmp(command, "hi") !=0 && strcmp(command,"history")!=0){
		print_error(WRONG_CMM);
		return 0;
	}

	//예외 없이 잘 수행한 명령어들은 hlist에 추가
	insert(command);
	
	if( strcmp(command, "hi") ==0 || strcmp(command,"history")==0)
		command_hi();	

	return 0;
}
//hash key 계산 ( 문자열 아스키 코드 값의 합)
int hash_key(char* s){
	int i,key=0;
	for(i=0; i<strlen(s); i++)
		key+=s[i]-'0';

	return key%HASH_MAX;
}
void create_hash_table(){
	int key,op_num;
	char op_name[20],op_type[10];
	FILE* fp = fopen("opcode.txt","r");
	op* opcode=NULL;

	//파일이 없는 경우
	if(fp==NULL){
		printf("'opcode.txt' dose not exist\n");
		return;
	}
	while(fscanf(fp,"%x %s %s",&op_num,op_name,op_type)!=EOF){
		//새로운 op node 생성
		opcode = (op*)malloc(sizeof(op));
		opcode->op_num=op_num;
		strcpy(opcode->op_name,op_name);
		strcpy(opcode->op_type,op_type);
		opcode->next=NULL;
		key=hash_key(op_name);
		//해당 위치에 처음으로 추가하는 경우
		if(oplist[key]==NULL){
			oplist[key]=opcode;
		}
		else{
			opcode->next=oplist[key];
			oplist[key]=opcode;
		}
	}
}
int main(){
	int command=0;// 명령어 
	
	command_reset(); //memory 초기화
	create_hash_table(); //hash table 생성
	while(command!=q){
		command=prompt();	//q 명령어 입력 전까지 prompt 수행
	}
	return 0;
}
//help 명령어 수행 함수
void command_h(){
	puts("h[elp]");
	puts("d[ir]");
	puts("q[uit]");
	puts("hi[story]");
	puts("du[mp] [start, end]");
	puts("e[dit] address, value");
	puts("f[ill] start, end, value");
	puts("reset");
	puts("opcode mnemonic");
	puts("opcodelist");
}
void command_d(){
	struct dirent* file;
	struct stat type;
	DIR *dir = opendir(".");

	while(1){
		file= readdir(dir);
		if(file==NULL)
			break;

		stat(file->d_name, &type);
		if(S_ISDIR(type.st_mode)){
			//. & .. 은 무시하고 넘어감
			if(strcmp(file->d_name,".")==0 || strcmp(file->d_name, "..")==0)
				continue;
			//directory 일 경우 /
			printf("%s/\n", file->d_name);
		}
		//실행파일의 경우 *
		else if(type.st_mode & S_IXUSR)
			printf("%s*\n", file->d_name);
		else
			printf("%s\n",file->d_name);
	}
	closedir(dir);
}
void command_q(){
	int i;
	node* tmp=NULL;
	node* tmp2=NULL;
	op* t=NULL;
	op* t2=NULL;

	//hlist free
	if(hlist!=NULL){
		for(tmp=hlist->front; tmp!=NULL;){
			tmp2=tmp;
			tmp=tmp->next;
			free(tmp2);
		}
		free(hlist);
	}

	//oplist(hash_table) free
	for(i=0; i<20; i++){
		for(t=oplist[i]; t!=NULL;t=t->next){
			t2=t;
			free(t2);
		}
	}
}

void command_hi(){
	int i;
	node* tmp;
	if(hlist==NULL) // 빈 리스트의 경우
		return;

	tmp= hlist->front;
	for(i=0; i<hlist->cnt;i++){
		printf("%d	%s\n", i+1, tmp->command);
		tmp= tmp->next;
	}
}
//한 digit(char) -> int
int one_digit_to_int(char c){ 	
	int result=0; 
	if(c>='A' && c<='F')
		result= c-'A'+10;
	else if(c>='a' && c<='f')
		result=c-'a'+10;
	else if(c>='0' && c<='9')
		result=c-'0';
	//16진수 형식에 맞지 않으면 -16 return 
	else
		result=-1;
	return result;
}
int make_hex(char* s){
	int i,result=0;
	char c;
	//앞에 0x(0X) 붙여준 경우그부분  짤라줌
	if(s[0]=='0' && ( s[1]=='x' || s[1]=='X'))
		s+=2;
	//한 digit씩 체크
	for(i=0; s[i]; i++){
		c=s[i];
		result*=16;
		if(c>='A' && c<='F')
			result+= c-'A'+10;
		else if(c>='a' && c<='f')
			result+=c-'a'+10;
		else if(c>='0' && c<='9')
			result+=c-'0';
		//16진수 형식에 맞지 않으면 -1 return 
		else
			return -1;
	}
	return result;
}
int command_du(char cmm[10][COMMAND_MAX], int cnt){
	int i,j,tmp;
	int s=0,end=0; // s: start, e: end
	int srow=-1,scol=-1, ecol=-1, erow=-1; // 출력 row/col 범위
	if( cnt>=2)	
		s= make_hex(cmm[1]);	//start 입력 받음
	if(cnt ==3)
		end= make_hex(cmm[2]);//end 입력 받음
	
	//입력 주소 체스 ( 입력받은 start/end가16진수 형식이 아니거나 범위 넘어간 경우 or s>e 
	if(s==-1||end==-1||s>0xfffff || end>0xfffff||( cnt==3 && s>end))//start > end 인 경우
	{
		print_error(WRONG_ADR);
		return 0;
	}
	//start 안받은 경우
	if(cnt<2){
		s=cur_adr;
		if(s>0xfffff)
			s=0;
	}
	//end 안 받은 경우
	if(cnt<3){
		end= s+159;
		if(end>0xfffff)
			end=0xfffff;
	}
	cur_adr=end+1;
	srow=s/16;
	scol=(s%16)*2;
	erow=end/16;
	ecol=(end%16)*2;
	
	for(i=srow; i<=erow; i++){
		printf("%05X ", i*16);
		for(j=0; j<32; j+=2){
			if((i==srow && j<scol)||(i==erow && j>ecol))	//범위아닌 메모리 값은 출력 x
				printf("   ");
			else
				printf("%02X ", one_digit_to_int(memory[i][j])*16+ one_digit_to_int(memory[i][j+1]));
		}
		printf("; ");
		for(j=0; j<32; j+=2){
			if((i==srow && j<scol)||(i==erow && j>ecol))	//범위아닌 메모리 값은 출력 x
				printf(".");
		
			else{
				tmp=one_digit_to_int(memory[i][j])*16 + one_digit_to_int(memory[i][j+1]); // 메모리에 저장된 값 
				if( tmp>=0X20 && tmp <=0X7E) // 20~7E 사이 값이면 해당 값의 아스키코드 출력
					printf("%c",tmp);
				else
					printf(".");
			}
		}
		printf("\n");
	}
	return 1;
}
int command_e(char cmm[10][COMMAND_MAX]){
	int adr,val;
	adr= make_hex(cmm[1]);
	val= make_hex(cmm[2]);
	if(adr<0 || adr >0Xfffff){
		print_error(WRONG_ADR);
		return 0;
	}
	
	if(val>0xff){
		print_error(WRONG_VAL);
		return 0;
	}
	memory[adr/16][(adr%16)*2]=cmm[2][0];
	memory[adr/16][(adr%16)*2+1]=cmm[2][1];
	return 1;
}
int command_f(char cmm[10][COMMAND_MAX]){
	int i,j,s,end,val;
	int srow,scol,erow,ecol;
	s=make_hex(cmm[1]);
	end=make_hex(cmm[2]);
	val=make_hex(cmm[3]);

	//잘못된 주소를 입력한 경우 (16진수 타입이 아님, end가 범위를 벗어남, start> end)
	if(s==-1 || end==-1 ||end>0xfffff|| s>end ){
		print_error(WRONG_ADR);
		return 0;
	}
	//value로 1byte 벗어나는 값을 입력한 경우
	if(val>0xff){
		print_error(WRONG_VAL);
		return 0;
	}
	srow=s/16;
	scol=(s%16)*2;
	erow=end/16;
	ecol=(end%16)*2;
	
	for(i=srow; i<=erow; i++){
		for(j=0; j<32; j+=2){
			if((i==srow && j<scol )||( i==erow && j>ecol))
				continue;
			memory[i][j] = cmm[3][0];
			memory[i][j+1]=cmm[3][1];
		}
	}
	return 1;
}
void command_reset(){
	int i,j;
	for(i=0; i<MEMORY_ROW; i++){
		for(j=0; j<32; j++){
			memory[i][j]='0';
		}
	}
}
int command_op_mn(char *s){
	int key,flag=-1;
	op* tmp=NULL;
	key=hash_key(s);
	
	for(tmp=oplist[key]; tmp!=NULL; tmp=tmp->next){
		// 입력받은 명령어를 찾은 경우
		if(strcmp(tmp->op_name,s)==0){
			printf("opcode %s is %X\n", s, tmp->op_num);
			flag=1;
		}
	}
	//해당 명령어가 존재하지 않는 경우
	if(flag==-1){
		printf("There is no %s\n", s);
		return 0;
	}
	return 1;
}
void command_op_list(){
	int i,flag=-1;
	op* tmp;
	for(i=0; i<20; i++){
		printf("%02d : ",i);
		if(oplist[i]==NULL)	//비어있는 경우
			printf("empty");
		for(tmp=oplist[i]; tmp!=NULL; tmp=tmp->next)
		{	
			if(flag==-1){
				printf("[%s,%X]", tmp->op_name,tmp->op_num);
				flag=1;
			}
			else
				printf("-> [%s,%X]", tmp->op_name,tmp->op_num);
		}
		flag=-1;
		printf("\n");
	}
}
