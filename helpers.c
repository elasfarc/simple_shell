#include "shell.h"
#include "memory_allocation.h"

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
 * @is_custom_error: A boolean flag that serves as an indicator,
 *  allowing the function to discern whether the error number and error message
 *  have been explicitly provided by the user
 *  or if they should be obtained from the global `errno` variable
 *  and the `perror()` function.
 *  When set to `true`, it signifies the error information is user-defined;
 *  when set to `false`, the function will retrieve and utilize
 *  the error details from the system's standard error handling mechanisms.
 *
 * Description: if @is_custom_error variadic variables are as the following:
 *			int error_num, char *error
 *
 * Return: void
 */
void handle_error(char *cmd, int is_custom_error, ...)
{

	char *program_path, *error_msg, *custom_err;
	va_list custom_err_args;

	program_path = _get_env("_");
	program_path = (program_path) ? program_path : _strdup("hsh");

	if (is_custom_error)
	{
		va_start(custom_err_args, is_custom_error);
		custom_err = va_arg(custom_err_args, char *);
		error_msg = _strcat(_strdup(program_path), ": ",
				cmd, ": ", custom_err, "\n", NULL);
		write(STDERR_FILENO, error_msg, _strlen(error_msg));
		va_end(custom_err_args);
	}
	else
	{
		error_msg = _strcat(_strdup(program_path),
				 ": ", cmd, ": ", NULL);
		write(STDERR_FILENO, error_msg, _strlen(error_msg));
		fflush(stderr);
		perror("");
	}
	_str_free_all(2, error_msg, program_path);
}

/**
 * print_env - print enviroment variables.
 *
 * Return: void
 */
int print_env(void)
{
	char **env = environ, *env_records = NULL;
	int i;
	ssize_t  written_bytes;

	for (i = 0; env[i] != NULL; i++)
		env_records = env_records ?
			_strcat(env_records, env[i], "\n", NULL)
			: _strcat(_strdup(env[i]), "\n", NULL);

	written_bytes = write(STDOUT_FILENO, env_records, _strlen(env_records));
	safe_free(env_records);
	return (!(written_bytes != -1));
}

/**
 * handle_exit - handle the exit command.
 * @ascii_int: the status ASCII integer after the exit command.
 *
 * Description: if @ascii_int is NULL or a valid positive ASCII integer
 *			free number of @count_to_free variadic args
 *			exit with the given code or (0) if @ascii_int is NULL
 *		else if it's neither NULL or not a valid integer
 *			write to the stderr and does not exit.
 * Return: void
 */
int handle_exit(const char *ascii_int)
{
	unsigned char exit_code, *exit_code_ptr = &exit_code;
	char *error_msg, *program_path;
	atoi_t *atoi;

	if (ascii_int == NULL)
		*exit_code_ptr = EXIT_SUCCESS;
	else
	{
		atoi = _atoi(ascii_int);
		if (!atoi)
			handle_error("exit", 0);
		if (atoi->is_vaild && atoi->integer >= 0)
			*exit_code_ptr = atoi->integer;
		else
			exit_code_ptr = NULL;
		safe_free(atoi);
	}

	if (exit_code_ptr == NULL)
	{
		program_path = _get_env("_");
		program_path = (program_path) ? program_path : _strdup("hsh");
		error_msg = _strcat(_strdup(program_path),
				": 7: exit: Illegal number: ", ascii_int, "\n", NULL);
		write(STDERR_FILENO, error_msg, _strlen(error_msg));
		_str_free_all(2, program_path, error_msg);
	}
	else
	{
		clean_allocated_memory();
		exit(exit_code);
	}
	return (E2BIG);
}
