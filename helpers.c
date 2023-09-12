#include "shell.h"

#define SHELL_SIGN ("($) ")

/**
 * should_prompt - depending on @is_interactive;it prints the shell prompt.
 * @is_interactive: indicate wheter or not the stdin refers to the terminal.
 *
 * Return: always 1;
 */
int should_prompt(short is_interactive)
{
	if (is_interactive)
		print_prompt();
	return (1);
}

/**
 * print_prompt - print the shell prompt
 *
 * Return: void
 */
void print_prompt(void)
{
	ssize_t w_bytes;
	char *prompt = "($) ";

	w_bytes = write(1, prompt, 5);

	if (w_bytes == -1)
	{
		perror("Error: print_prompt(..)");
		exit(EXIT_FAILURE);
	}
}

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
 *		,the full path of the command from the first match of PATH
 *		otherwise NULL
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
	printf("not found in current....\n");
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

