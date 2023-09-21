#include "shell.h"

/**
 * charInString - check wheter or not a string @string contains a char @c.
 * @c: char to be looked for in @string.
 * @string: string to be check @c against.
 *
 * Return: (1) if @string has @C
 *		(0) otherwise.
 */
int charInString(char c, char *string)
{
	int i, str_len, is_match = 0;

	str_len = _strlen(string) + 1;

	for (i = 0; (i < str_len && !is_match); i++)
		if (c == string[i])
			is_match = 1;

	return (is_match);
}

/**
 * _strtok - extract tokens from strings.
 * @s: string to be tokenized.
 * @delim: a set of bytes that delimit the tokens in the parsed string.
 *
 * Return: a pointer to the next token, or NULL if there are no more tokens.
 */
char *_strtok(char *s, char *delim)
{
	static char *string; /* set to NULL by the compiler */
	static int start_idx;
	int i, token_start = -1;

	if (s)
	{
		string = s;
		start_idx = 0;
	}

	if (string == NULL || *(string + start_idx) == '\0')
		return (NULL);

	for (i = start_idx; (string[i] != '\0' && token_start == -1); i++)
		if (!charInString(string[i], delim))
			token_start = i;

	if (token_start == -1)
		return (NULL);

	for (i = token_start; string[i]; i++)
	{
		if (charInString(string[i], delim))
		{
			string[i] = '\0';
			start_idx = i + 1;
			break;
		}
		if (string[i + 1] == '\0')
			start_idx = i + 1;
	}

	return (string + token_start);
}

/**
 * _strslice - Slice a portion of a string
 * @str: The input string to be sliced
 * @from: The starting index of the slice (inclusive)
 * @to: The ending index of the slice (inclusive)
 *
 * Description:
 * This function takes an input string `str` and slices a portion of it,
 * starting from the index `from` (inclusive) to the index `to` (inclusive).
 * It allocates memory for the sliced string and returns a pointer to
 * the sliced string.
 * If `str` is NULL or if memory allocation fails, the function returns NULL.
 *
 * Return:
 * On success, a pointer to the sliced string is returned.
 * On failure (if `str` is NULL or if memory allocation fails),
 * NULL is returned.
 */
char *_strslice(const char *str, int from, int to)
{
	char *sliced;
	int i, j;

	if (!str)
		return (NULL);
	/* TODO handle negative values */
	sliced = malloc((sizeof(char) * (to - from)) + 1 + 1);
	if (!sliced)
		return (NULL);

	for ((j = 0, i = from); (i <= to && str[i] != '\0'); (i++, j++))
		sliced[j] = str[i];

	sliced[j] = '\0';
	return (sliced);
}
