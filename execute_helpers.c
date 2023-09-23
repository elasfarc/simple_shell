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

	path = _get_env("PATH");
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
	safe_free(path);
	safe_free(path_cp);
	return (paths);
}
