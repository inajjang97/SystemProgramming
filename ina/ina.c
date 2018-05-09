void add_hash(int key, char *mntemp, char *optemp, char *typetemp)
{
	hnode *nnew = (hnode *)malloc(sizeof(hnode));
	hnode *temp = (hnode *)malloc(sizeof(hnode));
	nnew->next = NULL;
	strcpy(nnew->op, optemp);
	strcpy(nnew->m, mntemp);
	strcpy(nnew->type, typetemp);
	if(table[key].next == NULL)
	{
		table[key].next = nnew;
	}
	else
	{
		nnew->next = table[key].next;
		table[key].next = nnew;
	}
}//hash table에 노드 하나 추가

int store_const(char* value, int line){
	int i,size=1,asc=0;
	char c;
	
	if(asmarr[line].type==WORD)
		size=3;

	//문자
	if(value[0]=='C' || value[0]=='C'){
		value++;
		//글자수 초과
		if(strlen(value)*2!=size)
			return 0;
		for(i=0; i<strlen(value); i++){
			asc= (int)value[i];
			c=convert_hex_char(asc/16);
			if(c=='\0')
				return 0;
			(asmarr[line].obj)[2*i]=c;
		
			c=convert_hex_char(asc%16);
			if(c=='\0')
				return 0;
			(asmarr[line].obj)[2*i+1]= c;
		}
		(asmarr[line].obj)[i]='\0';
		return 1;
	}

	else if(value[0]=='x' || value[0] =='X'){
		value++;
		if(convert_hex(value)==-1)
			return 0;
		for(i=0; i<strlen(value); i++){
			(asmarr[line].obj)[i] = value[i];
		}
	}
	return; 
}
void store_loc(int line, char* m, char* opr, char* opr2){
	int opcode, size=0;
	char* temp;

	asmarr[line].adr= Loc[line];
	Loc[line+1]=Loc[line];
	if(strcmp(m,"START")==0){
		Loc[0]= convert_hex(opr);
		Loc[1]= Loc[0];
		ins[0].type= START;
		return;
