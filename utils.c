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
		token = _strtok(env_record, "=");
		if (_are_strs_eql(key, token))
		{
			token = _strtok(NULL, "=");
			value = _strdup(token);
			is_match = 1;
		}
		free(env_record);
	}
	return (value);
}

/**
 * _itoa - convert an integer to it's ASCII representation.
 * @n: number to be converted.
 *
 * Return: ASCII representation string.
 */

char *_itoa(int n)
{
	char *ascii;
	int bytes = 0, is_neg, is_zero, i, j, abs;

	is_neg = n < 0 ? 1 : 0;
	is_zero = n == 0 ? 1 : 0;
	abs = is_neg ? (n * -1) : n;

	if (is_zero)
		bytes = 1;
	else
		for (i = abs; i > 0; i /= 10)
			bytes++;

	bytes += (is_neg);
	ascii = malloc((sizeof(char) * bytes) + 1);

	if (!ascii)
		return (NULL);

	if (is_neg)
		ascii[0] = '-';
	for ((i = bytes - 1, j = abs); i >= (is_neg ? 1 : 0); (i--, j /= 10))
		ascii[i] = ((j % 10) + '0');

	ascii[bytes] = '\0';
	return (ascii);
}

atoi_t *_atoi(const char *ASCII)
{
	#define RADIX (10)
	#define ZERO_ASCII (48)
	int i, integer = 0, is_negative = 0, is_valid_number = 1;
	int zero_idx_ascci_len;

	atoi_t *atoi = malloc(sizeof(atoi_t));
	if (!atoi)
		return (NULL);
	if (!ASCII)
	{
		atoi->is_vaild = 0;
		return atoi;
	}
	zero_idx_ascci_len = _strlen(ASCII) - 1;
	if (ASCII[0] == '-')
	{
		is_negative = 1;
		ASCII = ASCII + 1;
		zero_idx_ascci_len -= 1;
	}

	for (i = 0; (ASCII[i] != '\0' && is_valid_number); i++)
	{
		if (ASCII[i] >= '0' && ASCII[i] <= '9')
			integer += (ASCII[i] - ZERO_ASCII) * _pow(RADIX, zero_idx_ascci_len - i);
		else
			is_valid_number = 0;
	}

	atoi->is_vaild = is_valid_number;
	atoi->integer = is_negative ? (integer * -1) : integer;

	return atoi;
}

