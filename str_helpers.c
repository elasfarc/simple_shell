#include "shell.h"

/**
 * _strlen - get the length of a string.
 * @s: string to get it's length.
 *
 * Return: size_t -> length of the string.
 */
size_t _strlen(char *s)
{
	if (!s)
	{
		perror("string at _strlen(..) is NULL");
		exit(1);
	}

	return (s[0] == '\0'
			? 0
			: 1 + _strlen(s + 1));
}

/**
 * _strdup - creates a new copy of the string @s
 * @s: string to be copied.
 *
 * Return: the new string.
 */
char *_strdup(char *s)
{
	size_t i, len;
	char *dup;

	if (!s)
	{
		perror("string at _strdup(..) is NULL");
		exit(1);
	}

	len = _strlen(s) + 1;
	dup = malloc(sizeof(char) * len);
	if (!dup)
	{
		perror("Error: malloc at _strdup(..)");
		exit(1);
	}

	for (i = 0; i < len - 1; i++)
		dup[i] = s[i];

	dup[i] = '\0';
	return (dup);
}

/**
 * _strcat - concat a string in src to following string args.
 * @src: source string to be the starting point of the concatenation.
 *
 * Return: the new concated string.
 */
char *_strcat(char *src, ...)
{
	va_list args;
	char *arg;
	size_t arg_len, current_len = 0;
	size_t ch_idx, i, j;

	if (!src)
		return (NULL);

	va_start(args, src);

	current_len = _strlen(src) + 1;
	ch_idx = current_len - 1;

	while ((arg = va_arg(args, char *)))
	{
		arg_len = _strlen(arg);
		src = (char *)_realloc(src, current_len, current_len + arg_len);

		current_len += arg_len;

		for (j = 0, i = ch_idx; i < current_len - 1; i++, j++)
			src[i] = arg[j];

		ch_idx = current_len - 1;
	}

	va_end(args);
	src[current_len - 1] = '\0';
	return (src);
}

