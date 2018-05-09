#include "20161601.h"

op* oplist[HASH_MAX];	//opcode hash table을 위한 linked list
ll* hlist=NULL;	//history 저장 linked list
sym* symlist=NULL;
code code_set[LINE_MAX]; 

int memory[MEMORY_ROW][MEMORY_COL];
int lc[MEMORY_ROW];
int cur_adr=0; // dump 수행시 시작 메모리 번지 
int pc=0,base=0;

int make_int(char* s);
int command_sym(); 
int store_symbol(int adr, int size, char* s, int line);
int insert_symbol(char* name, int adr );
int read_symbol(char* name);
int command_file(char* name);
int command_asm(char* name);
int insert_code(char* inst,char* operand, int line);
char make_char(int n);

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
		//해당 이름의 파일이 없는 경우 history 저장하지 않고 prompt 종료
		if(!command_file(command_words[1]))
			return 0;
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
		//어셈블이 실패한 경우
		if(!command_asm(command_words[1]))
			return 0;
	}
	else if(strcmp(command, "symbol")==0){
		if(!command_sym())
			return 0;
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
		for (t = oplist[i]; t != NULL; t = t->next) {
			t2 = t;
			free(t2);
		}
	}
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
	char c[LINE_MAX],*tmp;
	char inst[10][LINE_MAX];
	int i,line=-1,pnum=0,flag=0;	//line = line수, pnum = inst의 단어 개수
	
	//name.asm 파일 열기
	strcpy(asmfile, name);
	strcat(asmfile, ".asm");
	fp=fopen(asmfile, "r");

	if(fp==NULL){
		print_error(WRONG_FILE);
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
		print_error(ASSEMBLE_FAIL);
		return 0;
	}
	
	lc[0]=0;	
	//first pass
	while(flag!=1 && fgets(c, LINE_MAX,fp)!=NULL){
		/*
		if(c[0]=='.'){
			line++;
			lc[line+1]=lc[line];
			continue;
		}
		if( c[0]==' ' || c[0]=='\t')
			pnum=2;
		else 	//symbol table에 저장해야 하는 경우 
			pnum=3;
	
		//공백 단위로 
		strcpy(inst[0],strtok(c," \r\t\n"));
		
		if(inst[0]==NULL)
			continue;
		
		line++;
		i=1;
		lc[line+1]=lc[line];
		
		//inst 한줄 공백 단위로 쪼개기 
		while(i<pnum){
			tmp=strtok(NULL," \r\t\n");
			if(tmp==NULL){
				//print_error(ASSEMBLE_FAIL);
				break;
			}
			strcpy(inst[i++], tmp);
		}
		if(strcmp("END", inst[0]))
				flag=1;
		//symbol table에 추가해야되는 경우 	
		if(pnum==3){
			if(!insert_code(inst[1],inst[2], line))	// 에러발생시 종료 
				return 0;
			if(!insert_symbol(inst[0],lc[line]))
				return 0;
		}	
		//symbol 아닌 단순 instruction 인 경우
		else if(pnum==2){
			if(!insert_code(inst[0],inst[1],line))
				return 0;
		}*/
	}

	//2nd pass
	line=0;
	fclose(fp);
	fp=fopen(asmfile,"r");
	while(fgets(c, LINE_MAX, fp)!=NULL){
		create_lst(fp_lst, c, line);
		create_obj(fp_obj, c, line++);
		//if(strcmp(code_set[line-1].inst,"END")==0)
		//	break;
	}
	
	//파일 삭제
	//파일 삭제
	//파일 삭제
	fclose(fp_lst);
	fclose(fp_obj);
	fclose(fp);
	printf("output file : [%s], [%s]\n",lstfile,objfile);
	
	return 1;
}
int store_symbol(int adr , int size, char* s,int line){
	int i,len;
	int row=adr/16, col=(adr%16)*2, tmp2;
	char* tmp;
	tmp=strtok(s,"'");
	
	//문자인 경우
	if(!strcmp(tmp,"C")){
		tmp = strtok(NULL,"'");
		//올바른 형식이 아닌 경우
		if(strtok(NULL, "'")!=NULL)
			return 0;
		//아스키 코드값 저장	
		for(i=0; tmp[i]; i++){
			tmp2=(int)(tmp[i]);
			memory[row][col+i*2]=make_char(tmp2/16);
			memory[row][col+i*2+1]=make_char(tmp2%16);
		}
		lc[line+1]=lc[line]+strlen(tmp);
		return 1;
	}
	
	//16진수
	else if(!strcmp(tmp, "X")){
		tmp = strtok(NULL,"'");
		//숫자도 'F1F2F3' 이러면 3byte?
		//올바른 16진수 형태가 아닌경우
		if( make_hex(tmp)>0xff|| make_hex(tmp)==-1)
			return 0;
		//올바른 입력이 아닌경우
		if(strtok(NULL,"'")!=NULL)
			return 0;
		s=tmp;
	}
	
	len=strlen(s);
	if(len>size)
		return 0;
	
	for(i=0; i<size- len; i++)
		memory[row][col+i]='0';
	for(i=0; i<len; i++)
		memory[row][col+size-len+i]= s[i];
	lc[line+1]=lc[line]+size/2;

	return 1;
}
int insert_code(char* inst, char* operand,  int line){
	int key,flag=-1;
	op* tmp=NULL;

	strcpy(code_set[line].inst, inst);
	strcpy(code_set[line].operand,operand);
	code_set[line].adr=lc[line];
	code_set[line].type=0;	
	code_set[line].num=-1;
	
	//directives인 경우
	if(strcmp(inst, "START")==0){
		lc[0]=make_hex(operand);
		code_set[line].type=-1;
		return 1;
	}
	if(strcmp(inst, "END")==0){
		code_set[line].type=-1;
		return 1;
	}
	
	if(strcmp(inst, "WORD")==0){
		lc[line+1]=lc[line]+3;
		store_symbol(lc[line], 6, operand,line);
		return 1;
	}
	if(strcmp(inst, "BYTE")==0){
		lc[line+1]=lc[line]+1;
		store_symbol(lc[line],2,operand,line);
		return 1;
	}
	if(strcmp(inst, "RESW")==0){
		lc[line+1]= lc[line]+ make_int(operand)*3;
		return 1;
	}
	if(strcmp(inst, "RESB")==0){
		lc[line+1]=lc[line]+make_int(operand);
		return 1;
	}
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
	//
	//
	//
	//해당 명령어 없는 경우
	return 1;
}
//Loc가 ard인 'name' symbol을 symbol table 에 저장
int insert_symbol(char* name, int adr){
	sym* node;
	//이미 있는 symbol인 경우 에러처리
	if(read_symbol(name)!=-1){
		print_error(ASSEMBLE_FAIL);
		return 0;
	}
	
	//새로운 노드 생성
	node= (sym*)malloc(sizeof(sym));
	strcpy(node->name,name);
	node->adr=adr;
	
	//LIFO
	node->next=symlist;
	symlist=node;
	return 1;
}
//name에 해당하는 sym의 adr return (없으면 -1 return )
int read_symbol(char* name){
	sym* tmp;
	for(tmp=symlist; tmp!=NULL; tmp=tmp->next){
		if(strcmp(tmp->name, name)==0)
			return tmp->adr;
	}
	//해당 name의 symbol 없는 경우
	return -1;
}
int command_sym(){
	sym* tmp;
	if(symlist==NULL){
		print_error(NO_SYMBOL);
		return 0;
	}
	for(tmp=symlist; tmp!=NULL; tmp=tmp->next)
		printf("\t%s\t%X\n", tmp->name, tmp->adr);
	return 1;
}
int create_obj(FILE* fp, char* inst, int line){
	return 1;
}
int create_lst(FILE* fp, char* inst, int line){
	int opcode=code_set[line].num, i, offset=0;
	int row, col,tmp=0;
	int nixbpe[6];
	int obj[8];	//object code
	char* operand;
	if(inst==NULL)
		return 0;

	//엔터 제거
	if(inst[strlen(inst)-1]=='\n')
		inst[strlen(inst)-1]='\0';
	
	//lc 출력 안해도 되는 경우
	if(inst[0]=='.')
		fprintf(fp,"    \t");
	else
		fprintf(fp,"%04X\t",lc[line]);	//Loc
	fprintf(fp,"%s\t", inst);		//Inst	

	//object code 생성할 필요 없는 경우
	if(opcode==-1){
			fprintf(fp,"\n");
		return 1;
	}
	
	//nixbpe 모두 0으로 초기화
	for(i=0; i<6; i++)
		nixbpe[i]=0;

	operand=code_set[line].operand;
	pc= code_set[line].adr;
	row= pc/16;
	col= 2*(pc%16);
	
	//다음 inst로 pc 갱신
	pc= code_set[line+1].adr;
	
	//format 1 or 2
	if(code_set[line].type==1|| code_set[line].type==2){
		memory[row][col]=make_char(opcode/16);
		memory[row][col+1]=make_char(opcode%16);
		fprintf(fp,"%c%c", memory[row][col], memory[row][col+1]);

		//format 2인 경우
		if(code_set[line].type==2){
			memory[row][col+2]='0';
			memory[row][col+3]='0';
			fprintf(fp,"%c%c", memory[row][col+2], memory[row][col+3]);
		}
		fprintf(fp,"\n");
		return 1;
	}

	//immediate mode
	if(operand[0]=='#'){
		//#'숫자' 인 경우
		operand++;
		if(read_symbol(operand)==-1 && make_hex(operand) !=-1){
			nixbpe[1]=1;
			offset=make_hex(operand);
		}
		//#'symbol'인 경우
		else if(read_symbol(operand)!=-1){
			nixbpe[1]=1;
			offset=read_symbol(operand);
		}
	}
	
	//indirect
	else if(operand[0]=='@'){
		operand++;
		if(read_symbol(operand)!=-1){
			nixbpe[0]=1;
			offset=make_hex(operand);
			//memory[offset/16][offset%16];
		}
	}
	
	//simple addressing 
	else{
		nixbpe[0]=1;
		nixbpe[1]=1;
	}
	
	//operand로 받은 symbol이 없는 경우 에러
	/*if(read_symbol(operand)==-1){
		print_error(ASSEMBLE_FAIL);
		return 0;
	}*/
	
	pc= lc[line+1];
	offset = pc - read_symbol(operand);
	
	if(offset<0)
		offset*=-1;

	//base relative 
	if(offset>0x1000){
		nixbpe[3]=1;
		offset= base-read_symbol(operand);
		if(offset<0)
			offset*=-1;
	}
	
	//PC relative
	else
		nixbpe[4]=1;

	//obj code 계산
	obj[0]= opcode/16;
	obj[1]= opcode%16 + nixbpe[0]*2+nixbpe[1];
	obj[2]=0;
	for(i=2; i<6; i++)
		obj[2]= obj[2]*2+nixbpe[i];

	//format 3
	if(nixbpe[5]==0){
		obj[3]= offset/0x100;
		obj[4]= (offset%0x100)/0x10;
		obj[5]=(offset%0x100)%0x10;
		
		for(i=0; i<6; i++){
			memory[row][col+i]= make_char(obj[i]);
			fprintf(fp,"%c", memory[row][col+i]);
		}
	}
	
	//format 4
	else{
		for(i=0; i<3; i++)
			memory[row][col+i]=make_char(obj[i]);
		
		tmp=5-strlen(operand);
		// 5-byte adr 저장(5 digits 보다 작은 경우 앞에 0으로 채워줌)
		for(i=0; i<tmp; i++)
			memory[row][col+3+i]= '0';
		for(i=0; i<strlen(operand); i++)
			memory[row][col+(3+tmp)+i]=operand[i];
		
		for(i=0; i<8; i++)
			fprintf(fp,"%c", memory[row][col+i]);
	}
	
	fprintf(fp,"\n");
	return 1;
}
char make_char(int n){
	char c;
	if(n<0 || n>16)
		return '\0';
	if(n<10)
		c=n+'0';
	else
		c=(n-10)+'A';
	return c;
}
