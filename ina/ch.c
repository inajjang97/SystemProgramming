




		fgets(com, 40, fp);
		count = 0;
		str = strtok(com, " \t");
		strcpy(optemp, str);//optemp = opcode
		/////////////511
		while(str = strtok(NULL, " \t"))
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
		///////////////
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
	printf("MNTEMP %s\n", mntemp);
	printf("OPTEMP %s\n", optemp);
	printf("TYPETEMP %s\n", typetemp);
	////////HOHOHOHO
	strcpy(nnew->op, optemp);
	strcpy(nnew->m, mntemp);
	strcpy(nnew->type, typetemp);
	///////////// PARSE TYPE
	if (!strcmp(typetemp, "1")) {
		nnew->typenum = TYPE1;
	} else if (!strcmp(typetemp, "2")) {
		nnew->typenum = TYPE2;
	} else if (!strcmp(typetemp, "3/4")) {
		nnew->typenum = TYPE34;
	} else {
		printf("OHNO! LINE%d\n", __LINE__);
	}

	if(table[key].next == NULL) {
		table[key].next = nnew;
	} else {
		nnew->next = table[key].next;
		table[key].next = nnew;
	}
}//hash table에 노드 하나 추가
	






	hnode *temp = (hnode *)malloc(sizeof(hnode));
	for(i = 0; i < 20; i++){
		for(temp = table[i].next; temp; temp = temp->next){
			if(strcmp(m,temp->m)==0){
				//////////// 1258
				printf("MNE: %s\n", temp->type);
				printf("TYPE: %d\n", asmarr[line].type);
				// temp->typenum = TYPE1 or TYPE2 or TYPE34
				if (asmarr[line.type] != 4) {
					asmarr[line].type = temp->typenum;
				}
				asmarr[line].opcode= convert_hex(temp->op);
				Loc[line+1]+= asmarr[line].type;
				printf("store!line %03d %04X %20s type%8d\n",line, asmarr[line].adr, asmarr[line].mn, asmarr[line].type); 
				/////////////////
			}
		}
	}
	
	//주석인경우
	asmarr[line].type =INFO;
	return 1;
}
