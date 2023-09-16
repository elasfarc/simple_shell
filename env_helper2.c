#include "shell.h"

/**
 * _setenv - change or add an environment variable.
 * @name: key of the record tobe changed or added.
 * @value: the new value.
 *
 * Return: (1) on success; (0) otherwise.
 */
int _setenv(const char *name, const char *value)
{
	int is_modify_success, is_add_new_success;
	char **env = environ, **env_cpy;

	env_cpy = cpy_env();
	if (!env_cpy)
		return (0);

	is_modify_success = modify_record_in_env(name, value, env_cpy);

	if (!is_modify_success)
		is_add_new_success = add_new_record_to_env(name, value, &env_cpy);

	if (!is_modify_success && !is_add_new_success)
	{
		free_env(env_cpy);
		return (0);
	}

	free_env(env);
	environ = env_cpy;

	return (1);
}

/**
 * _unsetenv - deletes the variable name from the environment.
 *		If name does not exist in the environment,
 *		then the function succeeds,and the environment is unchanged.
 * @name: name of the enviroment variable to be deleted.
 *
 * Return: (1) on success, (0) otherwise.
 */
int _unsetenv(const char *name)
{
	char **env = environ, **new_env, *env_record, *key;
	int i, is_name_in_env = 0;

	for (i = 0; (env[i] != NULL && !is_name_in_env); i++)
	{
		env_record = _strdup(env[i]);
		key = _strtok(env_record, "=");
		if (_are_strs_eql(key, name))
			is_name_in_env = 1;
		free(env_record);
	}

	if (!is_name_in_env)
		return (1);

	i = 0;
	while (env[i])
		i++;
	new_env = malloc((sizeof(char *) * (i - 1)) + sizeof(NULL));
	if (!new_env)
		return (0);
	for (i = 0; env[i]; i++)
	{
		env_record = _strdup(env[i]);
		key = _strtok(env_record, "=");
		if (!(_are_strs_eql(key, name)))
			new_env[i] = _strdup(env[i]);
		free(env_record);
	}

	new_env[i - 1] = NULL;

	free_env(env);
	environ = new_env;

	return (1);
}
