#ifndef OPCODE_H__
#define OPCODE_H__

//정의되는 함수
int build_opcodelist(OPCODE** ophead);
int hash_function(char *s, int size);
void command_opcodelist(OPCODE** ophead);
int command_opcode_mnemonic(char *command, OPCODE** ophead);
void opcode_insert(OPCODE* node,OPCODE** ophead);
int check_mnemonic(char *mnemonic, char *format, int *opcode, OPCODE **ophead);

#endif
