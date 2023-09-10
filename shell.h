#ifndef _SHELL_H_
#define _SHELL_H_

#include <stddef.h>

extern char **environ;

void shell(void);
void print_prompt(void);
int should_prompt(short);
size_t _strlen(char *);
char *_strdup(char *);
char **get_argv(char *);

#endif
