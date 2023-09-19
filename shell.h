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

/**
 * struct atoi_s - ascii to integer strtuct.
 * @is_vaild: A flag to indicate if the string provided is valid
 *		and could be converted to an integer.
 * @integer: the converted integer from the provided string.
 */
typedef struct atoi_s
{
	int is_vaild, integer;
} atoi_t;


void shell(void);
int execute(char *, char *);
void print_prompt(void);
int should_prompt(short);
void handle_error(char *, int, ...);
void print_env(void);
void handle_exit(const char *, int, ...);

void *_realloc(void *ptr, size_t old_size, size_t new_size);
char *_get_env(char *key);
char *_itoa(int);
atoi_t *_atoi(const char *);

/* env helpers  */
char **cpy_env(void);
void free_env(char **env);
int modify_record_in_env(const char *name, const char *value, char **env);
int add_new_record_to_env(const char *name, const char *value, char ***envptr);
int _setenv(const char *name, const char *value);
int _unsetenv(const char *name);
void handle_env_change(char *cmd_with_args);

void handle_cd(char *cmd_with_args);

int _pow(int, int);

size_t _strlen(const char *);
char *_strdup(const char *);
char *_strcat(char *src, ...);
int _are_strs_eql(const char *, const char *);
void _str_free_all(int n, ...);

char *_strtok(char *s, char *delim);

char **get_argv(char *);
void free_argv(char **argv);
char **get_env_paths(void);
char *get_path(char *);

/* frees  */
void safe_free(void *v);
void free_string_array(char **str_arr, const int *const length);

#endif
