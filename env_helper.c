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

/**
 * modify_record_in_env - set a varaible name to different value
 *				if @name exisits in @env.
 * @name: key of the record to be changed.
 * @value: the new value to be set.
 * @env: the enviroment variable Array of strings.
 *
 * Return: (1) on Success - @name already exisit in @env, (0) otherwise.
 */
int modify_record_in_env(const char *name, const char *value, char **env)
{
	int i, is_name_in_env = 0;
	char *env_record, *old_env_record, *key;

	for (i = 0; (env[i] != NULL && !is_name_in_env); i++)
	{
		env_record = _strdup(env[i]);
		key = _strtok(env_record, "=");
		if (_are_strs_eql(key, name))
		{
			is_name_in_env = 1;
			old_env_record = env[i];
			env[i] = _strcat(_strdup(key), "=", value, NULL);
			if (!env[i])
			{
				env[i] = old_env_record;
				free(env_record);
				return (0);
			}
			free(old_env_record);
		}
		free(env_record);
	}

	return (is_name_in_env);
}















