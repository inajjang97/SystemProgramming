#ifndef ASSEMBLER_H__
#define ASSEMBLER_H__

//정의되는 함수
SYMBOL* allocate(SYMBOL *data);
void command_symbol(SYMBOL **symhead, int symbol_print_flag);
void location_counter(char *format,unsigned int *location, char *mnemonic);
int build_symtab(SYMBOL** symhead, SYMBOL* node, int line_num);
void objectcode_insert(int format, int first, int mid, int end, int location, OBJECT** head, OBJECT** tail, char* var, int enter_flag);
int reg_num(char *reg);
int find_symbol(SYMBOL **symhead, char *symbol);
int command_assemble(char **command, OPCODE **ophead, SYMBOL ***output, int *symbol_print_flag);

#endif
