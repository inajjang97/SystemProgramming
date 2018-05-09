#ifndef SHELL_H__
#define SHELL_H__

void command_dir();
void command_history(NODE* head, NODE* tail);
void command_quit(NODE** head, OPCODE** ophead);
void command_help();
int command_type(char **command);
#endif
