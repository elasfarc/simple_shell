#include "shell.h"

/**
 * shell - the shell interface
 *      reads from stdin and execute the command given.
 *
 * Return: void
 */
void shell(void)
{
	char *input = NULL;
	size_t n = 0;
	ssize_t rl;
	short is_interactive = isatty(STDIN_FILENO);

		while (should_prompt(is_interactive) &&
			(rl = getline(&input, &n, stdin) > -1))
	{
		execute(input);
	}

	free(input);
}

/**
 * execute - parse input and execute the cmd if any.
 * @input: input line to be parsed.
 *
 * Return: void
 */
void execute(char *input)
{
	char *inputcp, *cmd, *exe_path, **argv, **env = environ;
	pid_t child_pid;
	int exe;

	inputcp = _strdup(input);
	cmd = strtok(input, " \n");
	if (!cmd)
	{
		free(inputcp);
		return;
	}
	exe_path = get_path(cmd);
	if (exe_path == NULL)
	{
		printf("it's NULL\n");
		handle_exe_path_error(cmd);
		free(inputcp);
		return;
	}
	child_pid = fork();
	switch (child_pid)
	{
		case -1:
			_str_free_all(2, inputcp, exe_path);
			perror("Error: fork(..)");
			break;
		case 0:
			argv = get_argv(inputcp);
			exe = execve(exe_path, argv, env);
			if (exe == -1)
			{
				perror("Error: ");
				_str_free_all(3, inputcp, input, exe_path);
				free_argv(argv);
				exit(EXIT_FAILURE);
			}
		default:
			wait(NULL);
			_str_free_all(2, inputcp, exe_path);
	}
}

