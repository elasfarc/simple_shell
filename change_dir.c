#include "shell.h"

#define CHDIR_SUCCESS (0)
#define CHDIR_FAIL (-1)

unsigned int count_args(char **argv);
void handle_error_and_free(char **argv, int *err);
char *get_current_dir(char **argv);
char *get_target_path(char **argv, unsigned int argc);
void handle_cd_result(unsigned int, char **, char *, char *);
/**
 * handle_cd - parse arguments passed to the CD command
 *		changes the current directory of the calling process.
 *		handle errors if any.
 * @cmd_with_args: the provided command by the user.
 *
 * Return: void
 */
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
	if (!current_dir)
	{
		return;
	}

	target_path = get_target_path(argv, argc);

	cd_result = chdir(target_path);
	handle_cd_result(cd_result, argv, current_dir, target_path);
}

/**
 * count_args - count args in arguments vector (argv)
 * @argv: The arguments vector.
 *
 * Return: number of args (argc).
 */
unsigned int count_args(char **argv)
{
	unsigned int argc = 0;

	while (argv[argc])
		argc++;
	return (argc);
}

/**
 * handle_error_and_free - handle early errors in changing directory
 *			by setting global errno and calling handle_error
 * @argv: The arguments vector to be freed.
 * @err: points to an integer representing
 *		the error number to be assigned to global errno
 *
 * Return: void
 */
void handle_error_and_free(char **argv, int *err)
{
	if (err)
		errno = *err;
	handle_error(argv[0], 0);
	free_argv(argv);
}

/**
 * get_current_dir - returns the current working directory.
 * @argv: The arguments vector to be freed in case if error.
 *
 * Return: string represents the current working directory.
 */
char *get_current_dir(char **argv)
{
	char *current_dir = getcwd(NULL, 0);

	if (!current_dir)
	{
		handle_error_and_free(argv, NULL);
	}
	return (current_dir);
}

/**
 * get_target_path - returns the full path of the path provided by the user
 *			depending on number of arguments.
 * @argv: the arguments vector.
 * @argc: number of args
 *
 * Return: a pointer to the string represents the full target path.
 */
char *get_target_path(char **argv, unsigned int argc)
{
	return (argc == 1 || _are_strs_eql(argv[1], "~")
		 ? _get_env("HOME")
		 : _are_strs_eql(argv[1], "-")
		 ? _get_env("OLDPWD")
		 : _strdup(argv[1]));
}

/**
 * handle_cd_result - depending on @res if success tries to update
 * enviroment variables if updating is not success
 * restore old enviroment variables,
 * if fail handle the error then frees allocated memory.
 *
 * frees allocated memory
 * @res: the result of chdir system call.
 * @argv: the arguments vector.
 * @cwd: the current working directory.
 * @tgt_path: the full target path.
 *
 * Return: void
 */
void handle_cd_result(unsigned int res, char **argv, char *cwd, char *tgt_path)
{
	char *err_cmd;
	char **current_env;

	switch (res)
	{
		case CHDIR_FAIL:
			err_cmd = _strcat(_strdup(argv[0]), ": ",
					tgt_path ? tgt_path : "(null)", NULL);
			handle_error(err_cmd, 0);
			free(err_cmd);
			break;
		case CHDIR_SUCCESS:
			current_env = cpy_env();
			if (_setenv("OLDPWD", cwd) && _setenv("PWD", tgt_path))
				free_env(current_env);
			else
			{
				free_env(environ);
				environ = current_env;
				errno = ECANCELED;
				handle_error(argv[0], 0);
			}
	}
	_str_free_all(2, cwd, tgt_path);
	free_argv(argv);
}

