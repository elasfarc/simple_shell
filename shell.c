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

void clean(char *input, char **argv);

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
	char **argv = NULL;
	short is_interactive = isatty(STDIN_FILENO);
	char **env = environ;

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
				if (token)
				{
					argv = get_argv(input);
					exe = execve(input, argv, env);
					if (exe == -1)
						perror("");
				}
				clean(input, argv);
				exit(1);
			default:
				wait(NULL);
				/*free(input);*/
		}
		/*print_prompt();*/
	}

	free(input);
}

/**
 * clean - free allocated memory
 * @input: string allocated for the getline function.
 * @argv: array of strings allocated to get args of the command.
 *
 * Return: void
 */
void clean(char *input, char **argv)
{
	int i;

	free(input);
	if (!argv)
		return;
	for (i = 0; argv[i]; i++)
		free(argv[i]);

	free(argv);

}

