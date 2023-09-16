#include "shell.h"

/**
 * cpy_env - copy enviroment variables records.
 *
 * Return: A new Array of strings contianing enviroment variables records.
 */
char **cpy_env(void)
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

/**
 * free_env - free enviroment variables Array of strings.
 * @env: Array of enviroment variables records (NULL terminated).
 *
 * Return: void
 */
void free_env(char **env)
{
	int i;

	for (i = 0; env[i] != NULL; i++)
		free(env[i]);
	free(env);
}
















