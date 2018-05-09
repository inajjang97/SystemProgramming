





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
