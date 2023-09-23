#include "shell.h"
#include "memory_allocation.h"

#include <signal.h>

/**
 * sig_handler - Signal Handler Function
 * @sig: The signal number received
 *
 * Description:
 * This function serves as a signal handler. It is called when a specific
 * signal, indicated by the `sig` parameter, is received by the program.
 * The main purpose of this handler is to perform cleanup operations,
 * releasing allocated memory, and then exit the program with the given signal.
 *
 * Return: nothing
 */
void sig_handler(int sig)
{
	clean_allocated_memory();
	exit(sig);
}

/**
 * main - entry point for the shell program
 *
 * Return: 0 on success, 1 otherwise.
 */
int main(void)
{
	int last_cmd_return;

	if (signal(SIGINT, sig_handler) == SIG_ERR)
	{
		perror("Failed to set custom signal handler");
		return (1);
	}
	environ = cpy_env();

	last_cmd_return = shell();
	clean_allocated_memory();

	return (last_cmd_return);
}
