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
	program_path = (program_path) ? program_path : _strdup("hsh");
	error_msg = _strcat(_strdup(program_path), ": ", e, ": ", cmd, ": ", NULL);

	write(STDERR_FILENO, error_msg, _strlen(error_msg));
	fflush(stderr);
	perror("");

	_str_free_all(3, error_msg, program_path, e);
}

/**
 * print_env - print enviroment variables.
 *
 * Return: void
 */
void print_env(void)
{
	char **env = environ, *env_records = NULL;
	int i;

	for (i = 0; env[i] != NULL; i++)
		env_records = env_records ?
			_strcat(env_records, env[i], "\n", NULL)
			: _strcat(_strdup(env[i]), "\n", NULL);

	write(STDOUT_FILENO, env_records, _strlen(env_records));
	free(env_records);
}

void handle_exit(const char *ascii_int, int count_to_free, ...)
{
	va_list stringListToFree;
	int i;
	unsigned char exit_code, *exit_code_ptr = &exit_code;
	char *error_msg, *program_path;
	atoi_t *atoi;

	if (ascii_int == NULL)
		*exit_code_ptr = EXIT_SUCCESS;
	else {
		atoi = _atoi(ascii_int);
		if (!atoi)
			handle_error("exit");
		if (atoi->is_vaild && atoi->integer > 0)
			*exit_code_ptr = atoi->integer;
		else
			exit_code_ptr = NULL;
		free(atoi);
	}

	if (exit_code_ptr == NULL)
	{
		program_path = _get_env("_");
		program_path = (program_path) ? program_path : _strdup("hsh");
		error_msg = _strcat(_strdup(program_path), ": 7: exit: Illegal number: ",ascii_int, "\n", NULL);
		write(STDERR_FILENO, error_msg, _strlen(error_msg));
		_str_free_all(2, program_path, error_msg);
	}
	else
	{
		va_start(stringListToFree, count_to_free);
		for (i = 0; i < count_to_free; i++)
			free(va_arg(stringListToFree, char *));
		va_end(stringListToFree);
		exit(exit_code);
	}
}

