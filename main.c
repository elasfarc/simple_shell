#include "shell.h"

/**
 * main - entry point for the shell program
 *
 * Return: 0 on success, 1 otherwise.
 */
int main(void)
{
	environ = cpy_env();

	shell();

	free_env(environ);
	return (0);
}

