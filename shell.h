#ifndef _SHELL_H_
#define _SHELL_H_

#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

extern char **environ;

void shell(void);
void execute(char *);
void print_prompt(void);
int should_prompt(short);

void *_realloc(void *ptr, size_t old_size, size_t new_size);
char *_get_env(char *key);

size_t _strlen(char *);
char *_strdup(char *);
char *_strcat(char *src, ...);
int _are_strs_eql(char *, char *);
void _str_free_all(int n, ...);

char **get_argv(char *);
void free_argv(char **argv);
char **get_env_paths();
char *get_path(char *);
void handle_exe_path_error(char *cmd);

#endif
