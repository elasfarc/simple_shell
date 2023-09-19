#include "shell.h"

#include <signal.h>

void sig_hadler(int sig)
{
	printf("=== haha =(%d)==\n", sig);
	signal(SIGINT, SIG_DFL);
	raise(SIGINT);	
}

/**
 * main - entry point for the shell program
 *
 * Return: 0 on success, 1 otherwise.
 */
int main(void)
{
	signal(SIGINT, sig_hadler);
	environ = cpy_env();

	shell();

	environ ? free_env(environ) : (void)0;
	return (0);
}

