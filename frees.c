#include "shell.h"

/**
 * safe_free - Safely free memory allocated for a pointer
 * @v: Pointer to the memory to be freed
 *
 * Description:
 * This function frees the memory pointed to by the given pointer `v` safely.
 * It checks if the pointer is not NULL before freeing the memory and then sets
 * the pointer to NULL after freeing the memory.
 *
 * Return: None
 */
void safe_free(void *v)
{
	if (v)
	{
		free(v);
		v = NULL;
	}
}

/**
 * free_string_array - Safely free memory allocated for a string array
 * @str_arr: Pointer to the string array
 * @length: Pointer to an integer indicating the length of the string array
 *
 * Description:
 * This function frees the memory allocated for a string array `str_arr`.
 * It also takes an optional `length` parameter, which can be used to specify
 * the length of the array. If `length` is provided, it will be used to iterate
 * through the array. If not, the function will stop when it encounters a NULL
 * pointer in the array.
 *
 * Return: None
 */
void free_string_array(char **str_arr, const int *const length)
{
	int i;

	if (!str_arr)
		return;
	for (i = 0; (length ? (i < *length) : (str_arr[i] != NULL)); i++)
		safe_free(str_arr[i]);
	safe_free(str_arr);
}

