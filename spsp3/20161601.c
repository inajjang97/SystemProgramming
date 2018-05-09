#include "20161601.h"

op* oplist[HASH_MAX];	//opcode hash table을 위한 linked list
ll* hlist=NULL;			//history 저장 linked list
sym* symtable[26];		//symbol table
code code_set[CODE_MAX]; 
es_node* estab[3];		//estab (최대 파일 개수 3)

int memory[MEMORY_ROW][MEMORY_COL];
int lc[MEMORY_ROW];
int cur_adr=0; // dump 수행시 시작 메모리 번지 
int start_check=0, end_check=0;
int a=0,t=0,f=0,l=0,sw=0,pc=0,base=0,x=0,s=0,nobase=1;	// 레지스터
int reg[10];
int last_line=0,total_size=0, code_max=0;	//코드 라인 개수, 총 코드  사이즈, 코드 한 줄 최대 길이
int progaddr=0, csaddr=0 , cslth=0, exeaddr=0,exeaddr_run=0, prog_end=0;
int bp[CODE_MAX], bp_index=0, cur_bp=0;

int main() {
	int command = 0;// 명령어 

	command_reset(); //memory 초기화
	create_hash_table(); //hash table 생성
	while (command != q) {
		command = prompt();	//q 명령어 입력 전까지 prompt 수행
	}
	return 0;
}
//명령어 입력 받고, 해당 명령어에 해당하는 함수 호출quit 명령어를 받았을 때만 q(3) return 하고 , 나머지는 return 0
int prompt(){
	int i=0,flag=0, j, cut=0;	
	char command_words[10][COMMAND_MAX]; // (공백 제거된)명령어를 이루는 문자열 저장 배열 
	char command[COMMAND_MAX]; // 입력받은 실제 명렁어 
	char cmm[COMMAND_MAX];
	char *tmp =NULL,ch=-1, tmp_arr[COMMAND_MAX];	
	//초기화 
	for(i=0; i<10; i++){
		command_words[i][0]='\0';
	}

	i=0;
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
	strcpy(command, remove_space(command));
	i=0;	// 여기서부터 i = 명령어에 포함된 문자열 덩어리 개수
	strcpy(cmm,command);
	tmp= strtok(cmm," \r\n\t"); // ' ' 기준으로 자른 문자열 
	strcpy(command_words[i++],tmp);
	
	if( strcmp(command_words[0], "loader")==0)
		cut= 1;

	while(i<6){
		if( cut==1)
			tmp= strtok(NULL, " \r\t\n");	//공백 단위로 매개변수 구분하는 경우(loader)
		else
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
		if(i>3){	//너무 많은 parameter 받은 경우
			print_error(WRONG_PARAMETER);
			return 0;
		}
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
		if(i!=2){
			print_error(WRONG_PARAMETER);	//너무 많은 mnemonic을 받았거나, 띄어쓰기가 포함된 경우 
			return 0;
		}
		if(!command_op_mn(command_words[1]))	//해당op_mn이 없는 경우
			return 0;
	}
	else if( strcmp(command, "opcodelist") ==0)
		command_op_list();

	else if(strcmp(command_words[0],"type")==0){
		//파일 이름 잘못 입력한 경우
		if(i!=2){
			print_error(WRONG_PARAMETER);
			return 0;
		}
		//해당 이름의 파일이 없는 경우 history에는 저장
		command_file(command_words[1]);
			
	}

	else if(strcmp(command_words[0], "assemble")==0){
		//파일 이름 잘못 입력한 경우
		if(i!=2){
			print_error(WRONG_FILE);
			return 0;
		}
	
		//파일 이름 확인하기 (.asm 파일인지/ 올바른 형식인지)
		tmp= strtok(command_words[1], ".");
		if( tmp ==NULL){
			print_error(WRONG_FILE);
			return 0;
		}
		//파일이름 ('.' 앞 문자열) 저장하고 tmp에 확장자명 저장
		strcpy(command_words[1],tmp);
		tmp = strtok(NULL," \r\n\t");
		
		//확장자명이 asm 이 아니거나 파일 입력을 잘못한 경우
		if(strcmp(tmp, "asm")!=0){
			print_error(WRONG_FILE);
			return 0;
		}
		//어셈블이 실패한 경우 (history 에는 저장x)
		if(!command_asm(command_words[1])){
			free_symbol();
			return 0;
		}
	}
	else if(strcmp(command, "symbol")==0){
		//잘못된 입력
		if( i!=1){
			print_error(WRONG_CMM);
			return 0;
		}
		if(!command_sym()){
			print_error(NO_SYMBOL);
		}
	}

	else if( strcmp(command_words[0], "progaddr")==0){
		//잘못된 입력
		if( i!=2){
			print_error(WRONG_CMM);
			return 0;
		} 
		if( make_hex(command_words[1])==-1){
			print_error(WRONG_PARAMETER);
			return 0;
		}
		progaddr= make_hex(command_words[1]);
	}

	else if( strcmp(command_words[0], "loader")==0){
		//파일 입력 안한경우
		if( i==1){
			print_error(WRONG_PARAMETER);
			return 0;
		}	

		//파일 이름 확인하기 (.obj 파일인지/ 올바른 형식인지)
		for(j=1; j<i; j++){
			strcpy(tmp_arr, command_words[j]);
			tmp= strtok(tmp_arr, ".");
			if( tmp ==NULL){
				print_error(WRONG_FILE);
				return 0;
			}
			
			//tmp에 확장자명 저장
			tmp = strtok(NULL," \r\n\t");	
			//확장자명이 obj 이 아니거나 파일 입력을 잘못한 경우
			if(strcmp(tmp, "obj")!=0){
				print_error(WRONG_FILE);
				return 0;
			}
		}

		if(!command_loader(i-1, command_words))
			return 0;
	}

	else if(!strcmp(command,"run")){
		command_run();
	}
	else if( strcmp(command_words[0],"bp") ==0){
		//bp 출력
		if( i==1)
			command_bp(-1);

		//잘못된 형식
		else if(i>2){
			printf("Wrong Command\n");
			return 0;
		}

		//bp clear
		else if(strcmp(command_words[1],"clear")==0)
			command_bp(-2);

	//	else if( make_hex(command_words[1])==-1 || make_hex(command_words[1])>cslth)
	//		return 0;
		else
		command_bp(make_hex(command_words[1]));
	}

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
		if (s[i] != '\0' && s[i] != ' ' && s[i] != '\t' && s[i] != '\n')
			break;
		s[i] = '\0';
	}
	return s;
}
void print_error(int error) {
	if (error == WRONG_ADR)
		printf("Wrong address\n");
	else if (error == WRONG_VAL)
		printf("Wrong Value\n");
	else if (error == WRONG_PARAMETER)
		printf("Wrong parameter\n");
	else if (error == WRONG_CMM)
		printf("Wrong Command\n");
	else if (error == INPUT_SIZE_OVER)
		printf("Input size is over\n");
	else if(error==WRONG_FILE)
		printf("Wrong file name\n");
	else if(error==ASSEMBLE_FAIL)
		printf("Assemble is failed\n");
	else if(error==NO_SYMBOL)
		printf("Symbol table does not exist\n");
}
void insert(char cmm[COMMAND_MAX]) {
	//새로운 노드 할당
	node* newNode = (node*)malloc(sizeof(node));
	newNode->next = NULL;
	strcpy(newNode->command, cmm);

	//처음으로 노드 추가하는 경우
	if (hlist == NULL) {
		hlist = (ll*)malloc(sizeof(ll));
		hlist->front = hlist->rear = newNode;
		hlist->cnt = 0;
	}
	else {
		(hlist->rear)->next = newNode;
		hlist->rear = newNode;
	}
	(hlist->cnt)++;
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
int make_hex(char* s) {
	int i, result = 0;
	char c;
	//앞에 0x(0X) 붙여준 경우그부분  짤라줌
	if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
		s += 2;
	//한 digit씩 체크
	for (i = 0; s[i]; i++) {
		c = s[i];
		result *= 16;
		if (c >= 'A' && c <= 'F')
			result += c - 'A' + 10;
		else if (c >= 'a' && c <= 'f')
			result += c - 'a' + 10;
		else if (c >= '0' && c <= '9')
			result += c - '0';
		//16진수 형식에 맞지 않으면 -1 return 
		else
			return -1;
	}
	return result;
}
int make_int(char* s){
	int i, result=0;
	for(i=0; s[i]; i++){
		result*=10;
		if(s[i]<'0' || s[i]>'9')
			return -1;
		result+=s[i]-'0';
	}
	return result;
}
char make_char(int n) {
	char c;
	if (n<0 || n >= 16)
		return '\0';
	if (n<10)
		c = n + '0';
	else
		c = (n - 10) + 'A';
	return c;
}

//hash key 계산 ( 문자열 아스키 코드 값의 합)
int hash_key(char* s) {
	int i, key = 0;
	for (i = 0; i<strlen(s); i++)
		key += s[i] - '0';

	return key%HASH_MAX;
}
void create_hash_table() {
	int key, op_num;
	char op_name[20], op_type[10];
	FILE* fp = fopen("opcode.txt", "r");
	op* opcode = NULL;

	//파일이 없는 경우
	if (fp == NULL) {
		printf("'opcode.txt' dose not exist\n");
		return;
	}
	while (fscanf(fp, "%x %s %s", &op_num, op_name, op_type) != EOF) {
		//새로운 op node 생성
		opcode = (op*)malloc(sizeof(op));
		opcode->op_num = op_num;
		strcpy(opcode->op_name, op_name);
		strcpy(opcode->op_type, op_type);
		opcode->next = NULL;
		key = hash_key(op_name);
		//해당 위치에 처음으로 추가하는 경우
		if (oplist[key] == NULL) {
			oplist[key] = opcode;
		}
		else {
			opcode->next = oplist[key];
			oplist[key] = opcode;
		}
	}
}
//help 명령어 수행 함수
void command_h() {
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
	puts("assemble filename");
	puts("type filename");
	puts("symbol");
	puts("progaddr");
	puts("loader");
	puts("run");
	puts("bp");
}
void command_d() {
	struct dirent* file;
	struct stat type;
	DIR *dir = opendir(".");

	while (1) {
		file = readdir(dir);
		if (file == NULL)
			break;

		stat(file->d_name, &type);
		if (S_ISDIR(type.st_mode)) {
			//. & .. 은 무시하고 넘어감
			if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0)
				continue;
			//directory 일 경우 /
			printf("%s/\n", file->d_name);
		}
		//실행파일의 경우 *
		else if (type.st_mode & S_IXUSR)
			printf("%s*\n", file->d_name);
		else
			printf("%s\n", file->d_name);
	}
	closedir(dir);
}
void command_q() {
	int i;
	node* tmp = NULL;
	node* tmp2 = NULL;
	op* t = NULL;
	op* t2 = NULL;

	//hlist free
	if (hlist != NULL) {
		for (tmp = hlist->front; tmp != NULL;) {
			tmp2 = tmp;
			tmp = tmp->next;
			free(tmp2);
		}
		free(hlist);
	}

	//oplist(hash_table) free
	for (i = 0; i<20; i++) {
		for (t = oplist[i]; t != NULL;) {
			t2 = t;
			t=t->next;
			free(t2);
		}
	}
	free_symbol();
}
void command_hi() {
	int i;
	node* tmp;
	if (hlist == NULL) // 빈 리스트의 경우
		return;

	tmp = hlist->front;
	for (i = 0; i<hlist->cnt; i++) {
		printf("%d	%s\n", i + 1, tmp->command);
		tmp = tmp->next;
	}
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
		print_error(WRONG_PARAMETER);
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
int command_file(char* name){	
	FILE* fp= fopen(name, "r");
	char c[LINE_MAX];	
	//해당 이름의 파일이 현재 디렉토리에 없는 경우 에러메시지
	if(fp==NULL){
		print_error(WRONG_FILE);
		return 1;
	}
	//파일이 존재하면 내용 출력
	while(fgets(c,LINE_MAX,fp)!=NULL)
		printf("%s",c);
	fclose(fp);
	return 1;
}
int command_asm(char* name){
	FILE *fp;
	FILE *fp_lst, *fp_obj;
	char asmfile[LINE_MAX],*lstfile,objfile[LINE_MAX];
	char c[LINE_MAX],*tmp, *tmp2,prog_name[7]={"INAINA"};
	char inst[5][LINE_MAX];
	int i,line=-1,tnum;
	
	//name.asm 파일 열기
	strcpy(asmfile, name);
	strcat(asmfile, ".asm");
	fp=fopen(asmfile, "r");
	
	if(fp==NULL){
		print_error(WRONG_FILE);
		return 0;
	}

	free_symbol();
	nobase=1;
	lc[0]=0;
	//first pass
	while(fgets(c, LINE_MAX,fp)!=NULL){
		line++;
		tnum=3;
		lc[line+1]=lc[line];
		
		if(c[0]=='.'){
			code_set[line].type=COMMENT;
			continue;	
		}
		//symbol 아닌경우, 코드 한줄을 2부분으로(tnum=2) 나눈다(inst parameter)
		if(c[0]==' '||c[0]=='\t' || c[0]=='\n')
			tnum=2;
	
		if( code_max < (int)strlen(c))
			code_max= (int)strlen(c);
		//tmp2에는 tmun-1번 문자열을 짜르고 난 뒤의 남은 문자열을 저장할 것
		tmp2=c;
		for(i=0; i<tnum-1; i++){
			tmp=strtok_r(tmp2," \r\t\n",&tmp2);
			if(tmp==NULL)
				break;
			strcpy(inst[i],remove_space(tmp));
		}
		tmp2=remove_space(tmp2);
		
		//symbol인 경우
		if(tnum==3){
			if(!insert_code(inst[1],tmp2, line)){	// 에러발생시 종료 
				printf("Error! Line %d\n", (line+1)*5);
				return 0;
			}
			//프로그램 이름이면
			if(line==0 && strcmp(inst[1],"START")==0)
				strncpy(prog_name,inst[0],6);
			else if(!insert_symbol(inst[0],lc[line])){
				printf("Error! Line %d\n", (line+1)*5);
				return 0;
			}		
		}	

		//그외
		else if(tnum==2){				
			if(!insert_code(inst[0],tmp2,line)){
				printf("Error! Line %d\n", (line+1)*5);
				return 0;
			}
		}
	}

	//END 없는 경우
	if( end_check==0){
		printf("There dose not exist END directive\n");
		return 0;
	}
	

	//lst, obj 파일 생성
	lstfile=name;
	strcpy(objfile, name);
	strcat(lstfile, ".lst");
	fp_lst=fopen(lstfile,"wt");
	strcat(objfile,".obj");
	fp_obj=fopen(objfile, "wt");
	
	//파일 생성 오류시 에러 메시지 출력 후 종료
	if(fp_lst==NULL || fp_obj ==NULL){
		printf("File Error\n");
		return 0;
	}
	
	//2nd pass
	last_line=line-1;
	total_size= lc[last_line]-lc[0];
	code_max= code_max + 4;
	fclose(fp);
	fp=fopen(asmfile,"r");
	
	//lst 파일 생성
	for(i=0; i<=last_line; i++){
		fgets(c, LINE_MAX, fp);
		if(c[0]!='.'){
			if(!create_lst(fp_lst, c, i)){
				printf("Error! Line %d\n", (i+1)*5);
				printf("Error line => %s\n", c);
				fclose(fp_lst);
				fclose(fp_obj);
				fclose(fp);
				//파일 삭제
				remove(lstfile);
				remove(objfile);
				return 0;
			}
			else
				fprintf(fp_lst, "\n");
		}	

		else
			fprintf(fp_lst,"%d\t%s",(i+1)*5, c);
	}
	//obj 파일 생성
	create_obj(fp_obj,prog_name);

	start_check=0;
	end_check=0;
	fclose(fp_lst);
	fclose(fp_obj);
	fclose(fp);
	
	printf("output file : [%s], [%s]\n",lstfile,objfile);
	return 1;
}
int command_sym() {
	int i,flag=1;
	sym* tmp;
	for (i = 25; i >= 0; i--) {
		if(symtable[i]!=NULL)
			flag=0;
		for (tmp = symtable[i]; tmp != NULL; tmp = tmp->next) {
			//마지막 라인의 경우 엔터 제거
			if (i == 0 && tmp->next == NULL)
				printf("\t%s\t%04X", tmp->name, tmp->adr);

			else
				printf("\t%s\t%04X\n", tmp->name, tmp->adr);
		}
	}
	//sybol table 이 비어있는 경우
	if (flag==1) 
		return 0;

	return 1;
}
int create_obj(FILE* fp, char* prog_name) {
	int i, j = 0, k, size = 0, start = -1, end = -1;

	fprintf(fp, "H%s", prog_name);
	for (i = (int)strlen(prog_name); i<6; i++)
		fprintf(fp, " ");
	fprintf(fp, "%06X%06X\n", lc[0], total_size);

	for (i = 1; i <= last_line;) {
		//obj code 없는 문장
		if (code_set[i].type>4 && code_set[i].type != CONST) {
			i++;
			continue;
		}

		//상수 중간에 짤라서 출력한 경우, 남은 obj 코드 출력
		if (end != -1) {
			size = (int)strlen(code_set[i].obj) / 2 - end;
			end = -1;
			j = 1;
		}
		while (i + j <= last_line) {
			//obj code가 없는 경우
			if (code_set[i + j].type == DIREC || code_set[i + j].type == BASE || code_set[i + j].type == COMMENT) {
				j++;
				continue;
			}

			//변수를 선언한 경우 변수 선언이 끝나고 난 뒤부터 시작
			if (code_set[i + j].type == VAR) {
				while (code_set[i + j].type == VAR) {
					if (i + j<last_line)
						j++;
				}
				break;
			}
			//상수인 경우
			if (code_set[i + j].type == CONST) {
				//0x1E 넘어가면 나눠서 출력
				if ((int)strlen(code_set[i + j].obj) / 2 + size >30) {
					end = 30 - size;
					size = 30;
					break;
				}
				else {
					size += (int)strlen(code_set[i + j].obj) / 2;
					j++;
				}
				continue;
			}
			if (size + code_set[i + j].type>30)
				break;

			size += code_set[i + j].type;
			j++;
		}

		//사이즈 만큼 objcode 출력
		fprintf(fp, "T%06X%02X", code_set[i].adr, size);

		if (start != -1) {
			for (k = start; k<(int)(strlen(code_set[i].obj) / 2); k++)
				fprintf(fp, "%c%c", (code_set[i].obj)[2 * k], (code_set[i].obj)[2 * k + 1]);
			start = -1;
			i++;
		}

		for (k = i; k<i + j; k++) {
			if ((code_set[k].obj)[0] != '\0')
				fprintf(fp, "%s", code_set[k].obj);
		}

		//상수가 중간에 짤려서 출력된 경우
		if (end != -1) {
			for (k = 0; k<end; k++) {
				fprintf(fp, "%c%c", (code_set[i + j].obj)[2 * k], (code_set[i + j].obj)[2 * k + 1]);
			}
			start = k;
		}
		fprintf(fp, "\n");
		i += j;
		size = 0;
		j = 0;
	}
	
	for(i=0; i<=last_line; i++){
		if(code_set[i].type==4){
			//immediate mode 아닌 경우
			if((code_set[i].operand)[0]!='#'){
				fprintf(fp,"M%06X05\n", code_set[i].adr+1);
			}
		}
	}
	//End Record obj 코드 생성된 첫 inst의 주소 출력
	for (i = 0; i<last_line; i++) {
		if (code_set[i].num != -1) {
			fprintf(fp, "E%06X\n", code_set[i].adr);
			break;
		}
	}

	return 1;
}
int create_lst(FILE* fp, char* inst, int line) {
	int opcode, i, offset = 123456789;
	int nixbpe[6];
	int obj[8];	//object code
	char* operand;

	if (inst == NULL)
		return 0;

	//엔터 제거
	if (inst[strlen(inst) - 1] == '\n') {
		//줄간격 맞추기
		for (i = (int)strlen(inst) - 1; i <= code_max; i++)
			inst[i] = ' ';
		inst[i] = '\0';
	}
	//line 출력
	fprintf(fp, "%d\t", (line + 1) * 5);

	//lc 출력 안해도 되는 경우
	if (inst[0] == '.' || code_set[line].type == COMMENT || line == last_line)
		fprintf(fp, "    \t");

	//LoC
	else if (lc[line]>0xffff)
		fprintf(fp, "%05X   ", lc[line]);
	else
		fprintf(fp, "%04X    ", lc[line]);

	fprintf(fp, "%s\t", inst);		//Inst	

	opcode = code_set[line].num;
	operand = code_set[line].operand;
	pc = code_set[line + 1].adr;	//PC는 다음 inst adr

	//COMMENT인 경우
	if (code_set[line].type == COMMENT) {
		return 1;
	}

	//object code 생성할 필요 없는 경우
	if (opcode == -1) {
		if (code_set[line].type == CONST) {
			fprintf(fp, "%s", code_set[line].obj);
		}
		return 1;
	}
	
	if(strcmp(code_set[line].inst,"LDB")==0){
		base=0;
		//LDB 연산은 오직 immediate mode 에서만 가능 #
		if(operand[0]=='#'){
			//symbol이면 Loc 그대로 입력
			if( read_symbol(operand+1)!=-1)
				base = read_symbol(operand+1);
			else if(make_int(operand+1)!=-1)
				base = make_int(operand+1);
			else
				return 0;
		}
		nobase=0;
	}
	//obj code 생성!
	//nixbpe 모두 0으로 초기화
	for (i = 0; i<6; i++)
		nixbpe[i] = 0;

	//format 1 or 2
	if (code_set[line].type == 1 || code_set[line].type == 2) {
		//format 1의 겨우 operand 있으면 오류
		if (code_set[line].type == 1 && operand[0] != '\0')
			return 0;
		(code_set[line].obj)[0] = make_char(opcode / 16);
		(code_set[line].obj)[1] = make_char(opcode % 16);
		(code_set[line].obj)[2] = '\0';

		//format 2인 경우
		if (code_set[line].type == 2) {
			//parameter 1개인 inst
			if (!strcmp(code_set[line].inst, "CLEAR") || !strcmp(code_set[line].inst, "TIXR") || !strcmp(code_set[line].inst, "SVC")) {
				if ((code_set[line].operand2)[0] != '\0')
					return 0;
				(code_set[line].obj)[2] = '0' + read_reg(code_set[line].operand);
				(code_set[line].obj)[3] = '0';
			}
			//parameter 2개 
			else {
				if ((code_set[line].operand2)[0] == '\0')
					return 0;
				//reg, n 형식 연산
				if (!strcmp(code_set[line].inst, "SHIFTL") || !strcmp(code_set[line].inst, "SHIFTR")) {
					if (make_int(code_set[line].operand2) == -1 || make_int(code_set[line].operand2)>0xf)
						return 0;
					(code_set[line].obj)[2] = '0' + read_reg(code_set[line].operand);
					(code_set[line].obj)[3] = (code_set[line].operand2)[0];

				}
				else {
					(code_set[line].obj)[2] = '0' + read_reg(code_set[line].operand);
					(code_set[line].obj)[3] = '0' + read_reg(code_set[line].operand2);
				}
			}
			(code_set[line].obj)[4] = '\0';
		}
		fprintf(fp, "%s", code_set[line].obj);
		return 1;
	}

	//operand 없는 경우( RSUB 아닌 경우 에러)
	if (operand[0] == '\0') {
		if (strcmp(code_set[line].inst, "RSUB") != 0)
			return 0;
		(code_set[line].obj)[0] = make_char((opcode + 3) / 16);
		(code_set[line].obj)[1] = make_char((opcode + 3) % 16);
		strcpy(code_set[line].obj + 2, "0000");
		fprintf(fp, "%X0000", opcode + 3);
		return	1;
	}

	//immediate mode
	if (operand[0] == '#') {
		nixbpe[1] = 1;
		operand++;	//#제거

					//숫자인 경우
		if (read_symbol(operand) == -1) {
			//decimal
			if (make_int(operand) != -1)
				offset = make_int(operand);

			//잘못된 형식
			else
				return 0;

			//range 넘어가는 경우
			if (code_set[line].type == 3 && offset>0xFFF)
				return 0;
			if (code_set[line].type == 4 && offset>0xFFFFF)
				return 0;

		}
	}

	//indirect
	else if (operand[0] == '@') {
		operand++;
		nixbpe[0] = 1;
		//해당 symbol없는 경우
		if (read_symbol(operand) == -1)
			return 0;
	}

	//simple addressing 
	else {
		nixbpe[0] = 1;
		nixbpe[1] = 1;
		//format 4인 경우 offset 에 read adr 저장 	
		if (code_set[line].type == 4) {
			nixbpe[5] = 1;
			offset = read_symbol(operand);
		}

	}
	//offset 계산해야 하는 경우(immediate mode 중상수 대입을 제외하곤 모두 offset 계산필요)
	if (offset == 123456789) {
		pc = lc[line + 1];
		offset = read_symbol(operand) - pc;

		//base relative 
		if (offset>0xFFF || offset*(-1)> 0xFFF) {
			//base 값이 없는 경우 
			if(nobase==1)
				return 0;
			nixbpe[3] = 1;
			offset = read_symbol(operand) - base;
			//offset 음수인 경우 에러
			if (offset<0)
				return 0;
		}
		//PC
		else {
			nixbpe[4] = 1;
			//음수인 경우 2의 보수의 decimal 값으로 ( a+(-a) = 0x1000 -> -a= 0x1000-a)
			if (offset<0)
				offset = 0x1000 + offset;
		}
	}
	
		//index 모드인 경우
	if (code_set[line].type>2 && strcmp(code_set[line].operand2, "X") == 0) {
		//simple addressing인 경우에만 index 모드 가능하다
		if (!(nixbpe[0] == 1 && nixbpe[1] == 1))
			return 0;
		nixbpe[2] = 1;
		offset += x;
	}

	//최종offset 체크
	if ((code_set[line].type == 4 && offset>0xFFFFF) || (code_set[line].type == 3 && offset >0xFFF))
		return 0;

	//obj code 계산
	obj[0] = opcode / 16;
	obj[1] = opcode % 16 + nixbpe[0] * 2 + nixbpe[1];
	obj[2] = 0;

	//format 3
	if (code_set[line].type == 3) {
		obj[3] = offset / 0x100;
		obj[4] = (offset % 0x100) / 0x10;
		obj[5] = (offset % 0x100) % 0x10;
		obj[6] = '\0';

		for (i = 2; i<6; i++)
			obj[2] = obj[2] * 2 + nixbpe[i];

		//obj code 저장
		for (i = 0; i<6; i++)
			(code_set[line].obj)[i] = make_char(obj[i]);
		(code_set[line].obj)[6] = '\0';
	}

	//format 4
	else {
		nixbpe[5]=1;
		for (i = 2; i<6; i++)
			obj[2] = obj[2] * 2 + nixbpe[i];

		obj[3] = offset / 0x10000;
		obj[4] = (offset % 0x10000) / 0x1000;
		obj[5] = ((offset % 0x10000) % 0x1000) / 0x100;
		obj[6] = (((offset % 0x10000) % 0x1000) % 0x100) / 0x10;
		obj[7] = (((offset % 0x10000) % 0x1000) % 0x100) % 0x10;

		for (i = 0; i<8; i++)
			(code_set[line].obj)[i] = make_char(obj[i]);
		(code_set[line].obj)[8] = '\0';
	}
	fprintf(fp, "%s", code_set[line].obj);
	return 1;
}
int store_symbol(int adr , int size, char* s,int line){
	int i,len;
	int tmp2;
	char* tmp;
	tmp=strtok(s,"'");
	//문자인 경우
	if(!strcmp(tmp,"C")){
		//문자인데 WORD로 선언한 경우 에러
		if(size==3)
			return 0;
		tmp = strtok(NULL,"'");
		
		//올바른 형식이 아닌 경우
		if(strtok(NULL, " \r\t\n")!=NULL)
			return 0;

		len=(int)strlen(tmp);
		if(len>30)
			return 0;

		//아스키 코드값 저장(최대상수문자열길이는30= 3byte)	
		for(i=0; i<len ; i++){
			tmp2=(int)(tmp[i]);
			(code_set[line].obj)[2*i]=make_char(tmp2/16);
			(code_set[line].obj)[2*i+1]=make_char(tmp2%16);
		}
		(code_set[line].obj)[2*len]='\0';
		lc[line+1]=lc[line]+strlen(tmp);
		return 1;
	}
	
	//16진수
	else if(!strcmp(tmp, "X")|| !strcmp(tmp,"x")){
		tmp = strtok(NULL,"'");
		//올바른 입력 아닌 경우
		if(tmp==NULL || strtok(NULL," \r\t\n")!=NULL)
			return 0;
		tmp2=make_hex(tmp);
		
		//올바른 16진수 형태가 아닌경우
		if( tmp2==-1||( size==1 &&tmp2>0xff ) || (size==3 && tmp2>0xffffff ))
			return 0;

		for(i=0; tmp[i];i++){
			(code_set[line].obj)[i]= tmp[i];
		}
		(code_set[line].obj)[i]='\0';
		lc[line+1]=lc[line]+size;
	}
	//형식에 맞지 않음
	else
		return  0;

	return 1;
}
int insert_code(char* inst, char* operand,  int line){
	int key,flag=-1;
	op* tmp=NULL;
	char para1[LINE_MAX],*para2=NULL;
	
	strcpy(code_set[line].inst, inst);
	if(*operand!=0)
		strcpy(code_set[line].operand,operand);
	else
		(code_set[line].operand)[0]='\0';

	(code_set[line].operand2)[0]='\0';
	(code_set[line].obj)[0]='\0';
	code_set[line].adr=lc[line];
	code_set[line].type=3;	
	code_set[line].num=-1;	

	//directives인 경우
	if(strcmp(inst, "START")==0){
		lc[0]=make_int(operand);
		lc[1]=lc[0];
		code_set[line].type = DIREC;
		start_check=1;
		return 1;
	}
	if(strcmp(inst, "END")==0){
		code_set[line].type = DIREC;
		end_check=1;
		return 1;
	}
	
	//상수인 경우
	if(strcmp(inst, "WORD")==0){
		code_set[line].type = CONST;
		lc[line+1]=lc[line]+3;
		if(!store_symbol(lc[line], 3, operand,line))
			return 0;
		return 1;
	}
	if(strcmp(inst, "BYTE")==0){
		code_set[line].type = CONST;
		lc[line+1]=lc[line]+1;
		if(!store_symbol(lc[line],1,operand,line))
			return 0;
		return 1;
	}
	//변수
	if(strcmp(inst, "RESW")==0){
		code_set[line].type = VAR;
		lc[line+1]= lc[line]+ make_int(operand)*3;
		return 1;
	}
	if(strcmp(inst, "RESB")==0){
		code_set[line].type = VAR;	
		lc[line+1]=lc[line]+make_int(operand);
		return 1;
	}

	/*
	if(strcmp(inst, "BASE")==0){
		code_set[line].type = BASE;	
		
		return 1;
	}*/

	//format 4인 경우, 체크하고 +제거
	if(inst[0]=='+'){
		flag=1;
		inst++;
	}

	key= hash_key(inst);
	for(tmp=oplist[key]; tmp!=NULL; tmp=tmp->next){
		// 입력받은 명령어를 찾은 경우
		if(strcmp(tmp->op_name,inst)==0){
			//+ 입력 했는데 format 1이나 2인 경우 에러
			if(flag==1 && strlen(tmp->op_type)==1)
				return 0;
			//operand가 있으면(0이 아니면) parameter ','단위로 나누기
			if( *operand!=0){
				strcpy(para1,operand);
				strcpy(code_set[line].operand,remove_space(strtok(para1,",")));
				para2=strtok(NULL," ");
			}
			//parameter가 2개인 경우 operand2에 저장
			if(para2!=NULL){
				strcpy(code_set[line].operand2,remove_space(para2));
				//잘못된 형식(매개변수 너무많음)
				if(strtok(NULL," \r\t\n")!=NULL)
					return 0;
			}
			code_set[line].num= tmp->op_num;
			
			//해당 inst type 저장(op_type은char배열 타입)
			if(strlen(tmp->op_type)==1)
				code_set[line].type= (int)(tmp->op_type)[0] -'0';
			else if( flag==1)
				code_set[line].type= 4;
			else
				code_set[line].type=3;
			//Locaton count 계산
			code_set[line].adr=lc[line];
			lc[line+1]=lc[line]+code_set[line].type;

			return 1;
		}
	}

	//명령어 없는 경우 주석
	code_set[line].type=COMMENT;
	return 1;
}
int comp_symbol(char *name, char* tmp){
	int i,len= (int)strlen(name);
	if( len>(int)strlen(tmp))
		len = (int)strlen(tmp);
	for(i=1; i<len; i++){
		if( name[i] > tmp[i])
			return 0;
		if(name[i]< tmp[i])
			return 1;
	}
	if( (int)strlen(name) > (int)strlen(tmp))
		return 0;
	
	return 1;
}

//Loc가 ard인 'name' symbol을 symbol table 에 저장
int insert_symbol(char* name, int adr){
	int key=name[0]-'A';
	sym *node, *tmp=NULL, *tmp2=NULL;

	//이름 첫글자가 숫자이면 에러
	if( name[0]>='0' && name[0]<'9')
		return 0;

	//새로운 노드 생성
	node= (sym*)malloc(sizeof(sym));
	strcpy(node->name,name);
	node->adr=adr;

	//처음 삽입
	if( symtable[key]==NULL){
		node->next= symtable[key];
		symtable[key] = node;
	}

	//정렬하여 삽입
	else{
		tmp2= NULL;
		for(tmp=symtable[key]; tmp!=NULL; tmp=tmp->next){
			if(  comp_symbol(name, tmp->name)==0)
				break;
			tmp2=tmp;
		}
		//맨뒤에 삽입
		if(tmp==NULL){
			tmp2->next= node;
			node->next=NULL;
		}
		//맨처음 삽입
		else if(tmp2==NULL){
			node->next= tmp;
			symtable[key]= node;
		}
		//중간에 삽입
		else{
			tmp2->next = node;
			node->next= tmp;
		}
	}
	return 1;
}
void free_symbol(){
	sym* tmp, *tmp2;
	int i;
	for(i=0; i<26; i++){
		if(symtable[i]==NULL)
			continue;
		for(tmp= symtable[i]; tmp!=NULL;){
			tmp2= tmp;
			tmp=tmp->next;
			free(tmp2);
		}
		symtable[i]=NULL;
	}
}
//name에 해당하는 sym의 adr return (없으면 -1 return )
int read_symbol(char* name){
	int key= name[0]-'A';
	sym* tmp;

	if(symtable[key]!=NULL){
		for(tmp=symtable[key]; tmp!=NULL; tmp=tmp->next){
			if( strcmp(tmp->name, name)==0)
				return tmp->adr;
		}
	}
	return -1;
}
int read_reg(char* reg){
	if(!strcmp(reg, "A"))
		return A;
	if(!strcmp(reg, "B"))
		return B;
	if(!strcmp(reg, "S"))
		return S;
	if(!strcmp(reg, "T"))
		return T;
	if(!strcmp(reg, "F"))
		return F;
	if(!strcmp(reg, "L"))
		return L;
	if(!strcmp(reg, "PC"))
		return PC;
	if(!strcmp(reg, "X"))
		return X;
	if(!strcmp(reg, "SW"))
		return SW;
	return -1;
}

int command_loader(int num, char file[10][COMMAND_MAX]){
	FILE* f[3];
	int i, j,result= 1;

	//같은 파일 이름 체크
	//파일 열기
	for(i=0; i<num; i++){
		f[i]= fopen(file[i+1], "r");
		if( f[i]==NULL){
			printf("Dose not exist!!!!!!!!!!!!\n");
			for(j=0; j<i; j++)
				fclose(f[j]);
			return 0;
		}
	}
	
	if(result){
		
		//first pass
		csaddr= progaddr;
		for(i=0; i<num; i++){
			if(!create_estab(f[i], i)){
				result=0;
				break;
			}
		}
		print_estab();
		csaddr=progaddr;
		exeaddr=progaddr;

		for(i=0; i<num; i++){
			fclose(f[i]);
			f[i]=fopen(file[i+1],"r");
			//second pass
			if(result){
				if(!load_obj(f[i],i)){
					result=0;
				}
			}
			csaddr+= cslth;
		}
	}

	prog_end= csaddr;
	
	for(i=0; i<num; i++)
		fclose(f[i]);

	free_estab();
	return result;
}
int load_obj(FILE* fp, int f_index){
	int ref[LINE_MAX];
	char c[LINE_MAX], tmp[LINE_MAX], tmp2[10];
	int obj,t_adr, t_size,m_adr, i,row=0, col=0 ;

	ref[1]= csaddr;
	
	while(fgets(c, LINE_MAX, fp)){
		if(c[0]=='D')
			continue;

		if(c[0]=='R'){
			i=0;
			while(1){
				if( 1+9*i >= (int)strlen(c))
					break;
				strncpy(tmp,c+1+8*i,2);
				tmp[2]='\0';
				strncpy(tmp2, c+3+8*i, 6);
				tmp2[6]='\0';
				strcpy(tmp2, remove_space(tmp2));
			
				//external ref 저장 (reference number)
				ref[make_hex(tmp)]= read_estab(tmp2);
				i++;
			}
		}

		if(c[strlen(c)-1]=='\n')
			c[strlen(c)-1]='\0';

		//head record 
		if(c[0]=='H'){
			strncpy(tmp, c+13,6);
			tmp[6]='\0';
			cslth= make_hex(tmp);
		}
		
		//text record
		else if( c[0]=='T'){
			//시작주소 & 길이 저장
			strncpy(tmp, c+1,6);
			tmp[6]='\0';
			t_adr= make_hex(tmp)+csaddr;
			
			strncpy(tmp2, c+7,2);
			tmp2[2]='\0';
			t_size = make_hex(tmp2);
			
			row= t_adr/16;
			col= 2*(t_adr%16);
			for(i=0; i< t_size*2; i++){
				if( col>31){
					col=0;
					row++;
				}
				memory[row][col]= c[i+9];
				col++;
			}
		}	

		else if( c[0]=='M'){
			//수정주소사이즈
			char start_address[LINE_MAX];
			char length[10];

			strncpy(start_address,c+1,6);
			start_address[6]='\0';
			t_adr= make_hex(start_address) + csaddr;

			strncpy(length,c + 7,2);
			length[2] = '\0';
			
			t_size=make_hex(length);

			row=t_adr / 16;
			col=(t_adr % 16) * 2;
			
			//수정 half byte개수가 홀수인 경우 col++
			if (t_size % 2 == 1)
				col++;

			//더하거나 빼줄 symbol 이름
			char reference_string[10];
			strcpy(reference_string, c + 10);
			m_adr = ref[make_hex(reference_string)];

			if (m_adr == -1)
				return 0;

			if (c[9] == '-')
				m_adr *= -1;
			
			char value_string[100];
			//start_address에 수정할 부분(obj 코드) 얻어오기
			for(i=0; i<t_size; i++){
				value_string[i]= memory[row][col+i];
			}
			value_string[t_size] = '\0';

			obj = make_hex(value_string);
			obj= (obj + m_adr) % (1 << t_size * 4);
			unsigned int obj_front = obj & ((1 << (t_size * 4)) - 1);

			sprintf(value_string,"%06X", obj_front);
			//메모리에 수정 obj load
			for(i=0; i<t_size; i++){
				memory[row][col+i] = value_string[i];
			}
		}
		
		else if(c[0]=='E'){
			//exeaddr 없는 경우
			if( strlen(c) ==1)
				break;

			strncpy(tmp,c+1, 6);
			tmp[6]='\0';
			exeaddr=make_hex(tmp)+ ref[1];
			break;
		}
	}

	pc= exeaddr;
	exeaddr_run = exeaddr;

	for(i=0; i<10; i++)
		reg[i]=0;

	return 1;


}
int create_estab(FILE* fp, int f_index){
	char name[10];
	char c[LINE_MAX], tmp[LINE_MAX];
	int adr,size,i;
	
	while(fgets(c,LINE_MAX,fp)!=NULL){

		if(c[0]=='R' || c[0]=='T'|| c[0]=='R')
			continue;

		//haad record => 프로그램 이름 & 시작 주소 & 길이 estab 추가
		if(c[0]=='H'){
			//CS 이름, 시작주소, 길이 저장
			strncpy(name, c+1, 6);
			name[6]='\0';
			strcpy(name ,remove_space(name));
			
			strncpy(tmp, c+7, 6);
			tmp[6]='\0';
			adr = make_hex(tmp)+csaddr;
			
			strncpy(tmp, c+13, 6);
			tmp[6]='\0';
			size = make_hex(tmp);
			cslth=size;
			
			//중복된 CS인 경우 오류  
			if(read_estab(name)!=-1){
				printf("There exists duplicate control sections\n");
				return 0;
			}	
			
			insert_estab(f_index, name, size,adr);		
		}

		//define record => estab에 추가(이름, 주소)
		else if(c[0]=='D'){	
			//이름& 길이 저장
			size=-1;
			i=0;
			while(1){
				strncpy(name, c+1+ i*12, 6);
				name[6]='\0';
				strncpy(tmp, c+7+i*12, 6);
				tmp[6]='\0';
				strcpy(name ,remove_space(name));
				
				i++;
				if(1+(12*i) >=(int)strlen(c))
					break;

				adr = make_hex(tmp)+csaddr;
				if(read_estab(name)!=-1){
					printf("There exist duplicate external sybols\n");
					return 0;
				}
				insert_estab(f_index, name, size,adr);
			}

		}
	
	}
	
	csaddr+=cslth;
	return 1;
}

void insert_estab(int f_index, char* name, int  size, int adr){
	es_node* node= (es_node*)malloc(sizeof(es_node));
	es_node* tmp;
	//새로운 es_node 할당
	
	strcpy(node->name , name);
	node->size= size;
	node->adr=adr;
	node->next=NULL;

	//처음 삽입하는 경우
	if(estab[f_index]==NULL){
		estab[f_index]= node;
	}

	else{
		for(tmp=estab[f_index]; tmp->next;)
			tmp=tmp->next;
		tmp->next= node;
	}
}

int read_estab(char* name){
	int i;
	es_node *tmp=NULL;
	
	for(i=0; i<3; i++){
		if(estab[i]==NULL)
			continue;
	
		for(tmp=estab[i]; tmp; tmp=tmp->next){
			//같은 이름의 노드가 있는 경우 주소값 return 
			if( !strcmp(name, tmp->name))
				return tmp->adr;
		}
	}
	return -1;
}
void free_estab(){
	int i;
	es_node *tmp, *tmp2;

	for(i=0; i<3; i++){
		if(estab[i]==NULL)
			continue;
		
		for(tmp=estab[i]; tmp;){
			tmp2= tmp;
			tmp=tmp->next;
			free(tmp2);
		}
		estab[i]=NULL;
	}
}
void print_estab(){
	int i,len=0;
	es_node* tmp;
	
	printf("control		symbol		address		length\n");
	printf("section		name\n");
	printf("--------------------------------------------------------\n");
	for(i=0; i<3; i++){
		if( estab[i]==NULL)
			continue;

		for(tmp=estab[i]; tmp; tmp = tmp->next){
			if(tmp->size!=-1){
				len+=tmp->size;
				printf("%s\t\t\t\t%04X\t\t%04X\n", tmp->name, tmp->adr, tmp->size);
			}
			else
				printf("\t\t%s\t\t%04X\n",tmp->name, tmp->adr);	
		}
		printf("\n");
	}
	printf("--------------------------------------------------------\n");
	printf("				total length	%04X\n", len);
}

void read_opcode(int n, int* type){
	int i;
	op* tmp;
	
	for(i=0; i<20; i++){
		if(oplist[i]==NULL)
			continue;
		for(tmp= oplist[i]; tmp;tmp= tmp->next){
			if(tmp->op_num==n){
				if(strlen(tmp->op_type)==1)
					*type= tmp->op_type[0]-'0';
				else
					*type= 3;

				printf("INST %s \n",tmp->op_name);
				return;
			}
		}
	}
}

void what_is_op_code(char* obj, int* op, int* n, int* i,int* x, int* b, int* p , int* e, int* type){
	int tmp, tmp2;
	//opcode, n,i 계산
//	tmp = make_hex(obj);
	sscanf(obj,"%03X",&tmp);
	tmp2= tmp%16;
	tmp= tmp/16;

	*op = (tmp&0xFC);
	*n= (tmp&0x02)>>1;
	*i= tmp&0x01;

	*x= (tmp2&0x8)>>3;
	*b= (tmp2&0x4)>>2;
	*p= (tmp2&0x2)>>1;
	*e= tmp2&0x1;

	//operation, format 구하기
	read_opcode(*op,type);
	if( *type==3 && *e==1)
		*type=4;
}

void command_run(){

	//남은 bp 없는 경우 프로그램 끝까지
	if( cur_bp==bp_index){
		run_run_run(prog_end);
		cur_bp=0;
	}

	else{
		run_run_run(bp[cur_bp++]);
	}
	return;
}

void run_run_run(int end){
	int rg1, rg2,row, col, op,n,i,x,b,p,e, type, k;
	int operand, ina,song;
	char opr[5];
	char tmp[5];
	int target_adr=0,imm=0;
	while(pc<prog_end){
		row= pc/16;
		col= 2*(pc%16);

		tmp[0]= memory[row][col];
		tmp[1]= memory[row][col+1];
		tmp[2]= memory[row][col+2];
		tmp[3]= '\0';
		what_is_op_code(tmp, &op,&n,&i,&x,&b,&p,&e,&type);
		
		if( pc<=end && end<pc+type)
			break;
		pc+= type;


	if(pc>0x0400&&pc<0x1000){
				printf("imm: %X tar %X	opr %X\n",imm, target_adr, operand);
				printf(" PC %X op %x type %d nixbpe %d %d %d %d %d %d \n", pc, op,type,  n,i,x,b,p,e);
				printf("Operand %X \n", operand);
			break;
		}

		//format 1,2
		if( type<3){
			rg1=memory[row][col+2]-'0';
			rg2=memory[row][col+3]-'0';
			
			//CLEAR
			if(op==0xB4){
				reg[rg1]=0;	
			}
			
			//COMPR
			else if( op==0xA0){
				if( reg[rg1]< reg[rg2])
					reg[SW]=-1;
				else if(reg[rg1]==reg[rg2])
					reg[SW]=0;
				else
					reg[SW]=1;
			}

			//TIXR
			else if(op==0xB8){
				reg[X]++;

				if( reg[X]< reg[rg1])
					reg[SW]=-1;
				else if(reg[X]==reg[rg1])
					reg[SW]=0;
				else
					reg[SW]=1;

			}
			continue;
		}

		//operand 복사
		for(k=0; k< 2*type-3; k++)
			tmp[k]=memory[row][col+3+k];
		tmp[k]='\0';

		target_adr= make_hex_neg(tmp);
		
		operand= target_adr;
		imm=operand;
		
		if(b==1 && p==0)
			operand+= reg[B];	

		else if(b==0 && p==1)
			operand+=pc;	

		target_adr= operand;

		//immediate
		if( n==0 && i==1){
			if( b==0 && p==0)
				operand = imm;
		}

		//simple
		if( n==1 && i==1){
			row= operand/16;
			col= (operand%16)*2;
			
			for(k=0; k<6; k++)
				tmp[k]= memory[row][col+k];
			tmp[k]='\0';	
			operand= make_hex_neg(tmp);
			
		}

		//indirect
		else if( n==1 && i==0){	
			row= operand/16;
			col= (operand%16)*2;
	
			for(k=0; k<6; k++)
				tmp[k]= memory[row][col+k];
			tmp[k]='\0';
		//	operand= make_hex_neg(tmp);
				
				printf("imm: %X tar %X	opr %X\n",imm, target_adr, operand);
				printf(" PC %X op %x type %d nixbpe %d %d %d %d %d %d \n", pc, op,type,  n,i,x,b,p,e);
				printf("Operand %X \n", operand);
			sscanf(tmp,"%06X",&operand);
			printf("Dong Sueng%06X\n",operand);
				row= operand/16;
			col= (operand%16)*2;
			for(k=0; k<6; k++)
				tmp[k]= memory[row][col+k];
			tmp[k]='\0';
			operand= make_hex_neg(tmp);
			printf("Dong Suengo1212%06X\n",operand);
			target_adr = operand;
		}

		printf("imm %X	tar %X	opr %X\n", imm, target_adr, operand);
		printf(" PC %X op %x type %d nixbpe %d %d %d %d %d %d \n", pc, op,type,  n,i,x,b,p,e);
		printf("Operand %X \n", operand);

		printf("		PC %X L %X T %X X %X\n", pc, reg[L], reg[T],reg[X]);
		ina= operand/16;
		song = (operand%16)*2;

		//LDA
		if( op==0x00){
			reg[A]=operand;
		}

		//LDB
		else if( op== 0x68){
			reg[B]=operand;
		}

		//LDT 1073 
		else if(op==0x74){
			reg[T]= operand;
			printf("hahahahahahah	opr %X\n", operand);
		}

		//LDX
		else if(op==0x04){
			reg[X]=operand;
		}
		
		//STL
		else if(op==0x14){
			memory[ina][song]=(reg[L]/16);
			memory[ina][song+1]= (reg[L]%16);
		}
		
		//JSUB
		else if(op==0x48){
			reg[L]=pc;
			pc= target_adr;
		}

		//COMP
		else if(op==0x28){
			if(reg[A]<operand)
				reg[SW]=-1;
			else if(reg[A]==operand)
				reg[SW]=0;
			else
				reg[SW]=1;
		}

		//RSUB
		else if(op==0x4C){
			pc= reg[L];	
		}

		//JEQ
		else if(op==0x30){
			if( reg[SW]==0){
				pc=target_adr;
			}
		}

		//J
		else if(op==0x3C){
			pc=target_adr;
		}

		//STA
		else if(op==0x0C){
			memory[ina][song]=reg[A]/16;
			memory[ina][song+1]=reg[A]%16;
		}

		//TD
		else if(op==0xE0){
			reg[SW]=1;
		}

		//RD
		else if(op==0xD8){
			reg[A]=reg[A]&0x00;
			opr[2]='\0';
			reg[A]+= make_hex(opr)%0x100;
		}

		//STCH
		else if(op==0x54){
			memory[ina][song]=reg[A]%16;	
		}

		//LDCH
		else if(op==0x50){
			reg[A]=reg[A]&0x00;
			opr[2]= '\0';
			reg[A]+=make_hex(opr);
		}
		//WD
		else if(op==0xDC){
			//hello~~
		}

		//JLT
		else if(op==0x38){
			if( reg[SW]==-1){
				pc=target_adr;
			}
		}

		//JGT
		else if(op==0x34){
			if(reg[SW]==1){
				pc=target_adr;	
			}
		}
	}

	//register 출력
	printf("		A : %06X X: %06X\n",reg[A],reg[X]);
	printf("		L : %06X PC: %06X\n",reg[L],pc);
	printf("		B : %06X S: %06X\n",reg[B],reg[S]);
	printf("		T : %06X\n",reg[T]);
	printf("	Stop at checkpoint[%X]\n", end);
	return;
}
int make_hex_neg(char *s){
	int i,exp=1,tmp=make_hex(s);
	if((s[0]>='A' && s[0]<='F')||(s[0]>='8' && s[0]<='9')){
		for(i=0; i<strlen(s); i++)
			exp*=16;
		tmp=exp-tmp;
		tmp=-tmp;
	}
		
	return tmp;
}
void command_bp(int adr){
	int i;
	//break point 출력
	if( adr==-1){
		printf("\n		breakpoint\n");
		printf("		----------\n");
		for(i=0; i<bp_index; i++)
			printf("		%04X\n", bp[i]);
		return;
	}
	
	//break point clear
	if(adr==-2){
		printf("		[ok] clear all breakpoints\n");
		for(i=0; i<bp_index;i++)
			bp[i]=0;
		bp_index=0;
		return;
	}

	bp[bp_index++]= adr;
	printf("		[ok] create breakpoint %04X\n", adr);
	return;
}
