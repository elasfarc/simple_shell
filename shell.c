#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "shell.h"

/**
 * shell - the shell interface
 *	reads from stdin and execute the command given.
 *
 * Return: void
 */
void shell(void)
{
	char *token, *input = NULL;
	size_t n = 0;
	ssize_t rl, exe;
	pid_t child_pid;
	char *const *argv = {NULL};
	short is_interactive = isatty(STDIN_FILENO);

	while (should_prompt(is_interactive) &&
			(rl = getline(&input, &n, stdin) > -1))
	{
		child_pid = fork();
		switch (child_pid)
		{
			case -1:
				free(input);
				perror("fork error:");
				exit(EXIT_FAILURE);
			case 0:
				token = strtok(input, "\n");
				exe = execve(input, argv, NULL);
				if (!token || (exe == -1))
				{
					if (exe == -1)
						perror("");
					free(input);
					exit(1);
				}
				break;
			default:
				wait(NULL);
				/*free(input);*/
		}
		/*print_prompt();*/
	}

	free(input);
}
