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



