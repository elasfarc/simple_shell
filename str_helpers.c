#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
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


