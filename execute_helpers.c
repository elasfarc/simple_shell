#include "shell.h"

/**
 * get_argv - extract args and fill the argv
 * @cmd: string containing the whole command.
 *
 * Return: arguments vector; array of strings NULL terminated.
 */
char **get_argv(char *cmd)
{
	char **argv, *token, *cmd_cp, *sep = " \n";
	size_t i, argc = 0;

	cmd_cp = _strdup(cmd);

	token = strtok(cmd, sep);
	while (token)
	{
		argc++;
		token = strtok(NULL, sep);
	}
	argv = malloc((sizeof(char *) * argc) + sizeof(NULL)); /* +1 for last NULL*/
	if (!argv)
	{
		perror("Error: malloc @get_argv(..)");
		exit(1);
	}

	token = strtok(cmd_cp, sep);
	for (i = 0; i < argc; i++)
	{
		argv[i] = _strdup(token);
		token = strtok(NULL, sep);
	}

	argv[argc] = NULL;

	free(cmd_cp);
	return (argv);
}

/**
 * free_argv - free the args vector.
 * @argv: argv array of strings.
 *
 * Return: void
 */
void free_argv(char **argv)
{
	int i;

	for (i = 0; argv[i]; i++)
		free(argv[i]);
	free(argv);
}

/**
 * get_env_paths - get the enviroment PATH
 *
 * Return: An array of strings containing the PATH env variable.
 */
char **get_env_paths()
{
	char *path, *path_cp, *token, **paths, *sep = ":";
	size_t i, pathes_n = 0;

	path = getenv("PATH");
	path_cp = _strdup(path);

	token = strtok(path_cp, sep);
	while (token)
	{
		pathes_n++;
		token = strtok(NULL, sep);
	}
	free(path_cp);
	paths = malloc((sizeof(char *) * pathes_n) + sizeof(NULL));
	paths[pathes_n] = NULL;

	path_cp = _strdup(path);
	token = strtok(path_cp, sep);
	for (i = 0; i < pathes_n; i++)
	{
		paths[i] = _strdup(token);
		token = strtok(NULL, sep);
	}

	free(path_cp);
	return (paths);
}

/**
 * get_path - get the full path of a @cmd if any
 * @cmd: the command to get it's full path from PATH enviroment variable.
 *
 * Return: the @cmd if it starts with '/' or '.'
 *              ,the full path of the command from the first match of PATH
 *              otherwise NULL
 */
char *get_path(char *cmd)
{
	char **paths, *path;
	int i, is_path = 0;
	struct stat st;

	if (cmd[0] == '/' || cmd[0] == '.')
		return (_strdup(cmd));

	path = _strcat(_get_env("PWD"), "/", cmd, NULL);
	if (stat(path, &st) == 0)
		return (path);

	free(path);
	paths = get_env_paths();
	for (i = 0; (paths[i] != NULL && !is_path); i++)
	{
		paths[i] = _strcat(paths[i], "/", cmd, NULL);
		if (stat(paths[i], &st) == 0)
			is_path = 1;
	}

	if (is_path)
		path = _strdup(paths[i - 1]);
	else
		path = NULL;

	for (i = 0; paths[i] != NULL; i++)
		free(paths[i]);

	free(paths);
	return (path);
}

/**
 * handle_exe_path_error - prints to stderr where no such execute path.
 * @cmd: the command that has no execute path.
 *
 * Return: void
 */

void handle_exe_path_error(char *cmd)
{
	char *program_path, *error_msg;

	program_path = _get_env("_");
	program_path = (program_path) ? program_path : "hsh";

	error_msg = _strcat(_strdup(program_path),
			": 1: ", cmd, ": not found\n", NULL);

	write(STDERR_FILENO, error_msg, _strlen(error_msg));

	free(error_msg);
	free(program_path);
}

