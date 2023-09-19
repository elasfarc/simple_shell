#include "shell.h"
#include "memory_allocation.h"

#include <signal.h>


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
	if (signal(SIGINT, sig_handler) == SIG_ERR)
	{
		perror("Failed to set custom signal handler");
		return (1);
	}
	environ = cpy_env();

	shell();
	clean_allocated_memory();

	return (0);
}
