#include "shell.h"

#define SHELL_SIGN ("($) ")

/**
 * should_prompt - depending on @is_interactive;it prints the shell prompt.
 * @is_interactive: indicate wheter or not the stdin refers to the terminal.
 *
 * Return: always 1;
 */
int should_prompt(short is_interactive)
{
	if (is_interactive)
		print_prompt();
	return (1);
}

/**
 * print_prompt - print the shell prompt
 *
 * Return: void
 */
void print_prompt(void)
{
	ssize_t w_bytes;
	char *prompt = "($) ";

	w_bytes = write(1, prompt, 5);

	if (w_bytes == -1)
	{
		perror("Error: print_prompt(..)");
		exit(EXIT_FAILURE);
	}
}


/**
 * handle_error - prints to stderr the associated error message.
 * @cmd: command that encountered an issue at its execution.
 *
 * Return: void
 */
void handle_error(char *cmd)
{
	char *program_path, *error_msg, *e;

	e = _itoa(errno);

	program_path = _get_env("_");
	program_path = (program_path) ? program_path : "hsh";
	error_msg = _strcat(_strdup(program_path), ": ", e, ": ", cmd, ": ", NULL);

	write(STDERR_FILENO, error_msg, _strlen(error_msg));
	fflush(stderr);
	perror("");

	_str_free_all(3, error_msg, program_path, e);
}

