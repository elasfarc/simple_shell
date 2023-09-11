#include "shell.h"

/**
 * _realloc - Reallocates the given area of memory.
 * @ptr: pointer to the area of memory to be reallocated.
 * @old_size: the current size of the data @ptr points to.
 * @new_size: the new memory size after allocation
 *
 * Return: the new allocated pointer with old data if any.
 */
void *_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void *old_ptr, *new_ptr;
	size_t i, stop_point;

	if (new_size == old_size)
		return (ptr);
	if (!ptr)
		return (malloc(new_size));
	if (ptr && new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	old_ptr = ptr;
	new_ptr = malloc(new_size);

	stop_point = new_size > old_size ? old_size : new_size;
	for (i = 0; i < stop_point; i++)
		((char *)new_ptr)[i] = ((char *)old_ptr)[i];

	free(old_ptr);
	return (new_ptr);
}

/**
 * _get_env - get an environment variable.
 * @key: name of the env var
 *
 * Return: the enviroment variable value for the @key.
 */
char *_get_env(char *key)
{
	char **env, *token, *env_record, *value = NULL;
	int i, is_match = 0;

	if (!key)
		return (NULL);

	env = environ;
	for (i = 0; (env[i] != NULL && !is_match); i++)
	{
		env_record = _strdup(env[i]);
		token = strtok(env_record, "=");
		if (_is_str_equal_str(key, token))
		{
			token = strtok(NULL, "=");
			value = _strdup(token);
			is_match = 1;
		}
		free(env_record);
	}
	return (value);
}
