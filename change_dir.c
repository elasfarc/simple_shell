#include "shell.h"

#define CHDIR_SUCCESS (0)
#define CHDIR_FAIL (-1)

unsigned int count_args(char **argv);
void handle_error_and_free(char **argv, int *err);
char* get_current_dir(char **argv);
char* get_target_path(char **argv, unsigned int argc);
void handle_cd_result(unsigned int, char **, char *, char *);

void handle_cd(char *cmd_with_args)
{
	char **argv, *current_dir, *target_path = NULL;
	unsigned int argc = 0, cd_result;
	int err_num;
	
	argv = get_argv(cmd_with_args);
	argc = count_args(argv);
	if (argc > 2)
	{
		err_num = E2BIG;
		handle_error_and_free(argv, &err_num);
		return;
	}
	
	current_dir = get_current_dir(argv);
	if(!current_dir)
	{
		return;
	}
	
	target_path = get_target_path(argv, argc);
	
	cd_result = chdir(target_path);
	handle_cd_result(cd_result, argv, current_dir, target_path);
}

unsigned int count_args(char **argv)
{
	unsigned int argc = 0;
	while (argv[argc])
		argc++;
	return argc;
}

void handle_error_and_free(char **argv, int *err)
{
	if (err)
	    errno = *err;
	handle_error(argv[0], 0);
	free_argv(argv);
}

char* get_current_dir(char **argv)
{
	char *current_dir = getcwd(NULL, 0);
	if(!current_dir)
	{
		handle_error_and_free(argv, NULL);
	}
	return current_dir;
}

char* get_target_path(char **argv, unsigned int argc)
{
	return argc == 1 || _are_strs_eql(argv[1], "~")
		? _get_env("HOME")
		: _are_strs_eql(argv[1], "-")
		? _get_env("OLDPWD")
		: _strdup(argv[1]);
}

void handle_cd_result(unsigned int cd_result, char **argv, char *current_dir, char *target_path)
{
	char *err_cmd;
	char **current_env;
	switch (cd_result)
	{
		case CHDIR_FAIL:
			err_cmd = _strcat(_strdup(argv[0]), ": ", target_path ? target_path : "(null)", NULL);
			handle_error(err_cmd, 0);
			free(err_cmd);
			break;
		case CHDIR_SUCCESS:
			current_env = cpy_env();
			if (_setenv("OLDPWD", current_dir) && _setenv("PWD", target_path))
				free_env(current_env);
			else
			{
				free_env(environ);
                		environ = current_env;
                		errno = ECANCELED;
                		handle_error(argv[0], 0);
            		}
	}
	_str_free_all(2, current_dir, target_path);
	free_argv(argv);
}

