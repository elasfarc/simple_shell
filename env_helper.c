#include "shell.h"

/**
 * cpy_env - copy enviroment variables records.
 *
 * Return: A new Array of strings contianing enviroment variables records.
 */
char **cpy_env()
{
	char **env = environ, **env_cpy;
	int i, records = 0;


	for (i = 0; env[i] != NULL; i++)
		records++;

	env_cpy = malloc((sizeof(char *) * records) + sizeof(NULL));
	if (!env_cpy)
		return (NULL);

	for (i = 0; env[i] != NULL; i++)
		env_cpy[i] = _strdup(env[i]);

	env_cpy[records] = NULL;
	return (env_cpy);
}


















