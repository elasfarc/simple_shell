#include "shell.h"

/**
 * get_argv - Split a string into an array of strings using default delimiters
 * @key: The string to be split
 *
 * Description:
 * This function takes a string `key` and splits it into an array of strings.
 * using default delimiters " \n". It calls the `get_custom_delim_argv`
 * function with these default delimiters.
 *
 * Return:
 * - On success, returns a pointer to an array of strings.
 * - On failure, returns NULL.
 */
char **get_argv(char *key)
{
	char *delim = " \n";

	return (get_custom_delim_argv(key, delim));
}

/**
 * get_custom_delim_argv - Split a string into an array of strings using custom
 * delimiters
 * @cmd: The string to be split
 * @sep: The string containing custom delimiters
 *
 * Description:
 * This function takes a string `cmd` and splits it into an array of strings
 * using custom delimiters specified in the `sep` parameter. It dynamically
 * allocates memory for the resulting array and individual string elements.
 * It also handles freeing the memory when it's no longer needed.
 *
 * Return:
 * - On success, returns a pointer to an array of strings.
 * - On failure, prints an error message and exits the program
 *   with an error code.
 */
char **get_custom_delim_argv(char *cmd, char *sep)
{
	char **argv, *token, *cmd_cp;
	size_t i, argc = 0;

	cmd_cp = _strdup(cmd);

	token = _strtok(cmd, sep);
	while (token)
	{
		argc++;
		token = _strtok(NULL, sep);
	}

	argv = malloc((sizeof(char *) * argc) + sizeof(NULL)); /* +1 for last NULL*/
	if (!argv)
	{
		perror("Error: malloc @get_argv(..)");
		exit(1);
	}

	token = _strtok(cmd_cp, sep);
	for (i = 0; i < argc; i++)
	{
		argv[i] = _strdup(token);
		token = _strtok(NULL, sep);
	}

	argv[argc] = NULL;

	safe_free(cmd_cp);
	return (argv);
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

	token = _strtok(path_cp, sep);
	while (token)
	{
		pathes_n++;
		token = _strtok(NULL, sep);
	}
	safe_free(path_cp);
	paths = malloc((sizeof(char *) * pathes_n) + sizeof(NULL));
	paths[pathes_n] = NULL;

	path_cp = _strdup(path);
	token = _strtok(path_cp, sep);
	for (i = 0; i < pathes_n; i++)
	{
		paths[i] = _strdup(token);
		token = _strtok(NULL, sep);
	}

	safe_free(path_cp);
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
	{
		path = cmd[0] == '/'
			? _strdup(cmd)
			: _strcat(_get_env("PWD"), "/", cmd, NULL);
		if (stat(path, &st) == 0)
			return (path);
		safe_free(path);
		return (NULL);
	}

	path = _strcat(_get_env("PWD"), "/", cmd, NULL);
	if (stat(path, &st) == 0)
		return (path);

	safe_free(path);
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

	free_string_array(paths, NULL);
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

	safe_free(error_msg);
	safe_free(program_path);
}
