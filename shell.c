#include "shell.h"

#define EXE_SUCCESS (0)
#define EXE_ERR (-1)
#define FORK_ERR (-2)

/**
 * shell - the shell interface
 *      reads from stdin and execute the command given.
 *
 * Return: void
 */
void shell(void)
{
	char *input = NULL, *inputcp, *cmd, *exe_path;
	size_t n = 0;
	ssize_t rl, exe;
	short is_exit, is_interactive = isatty(STDIN_FILENO);

		while (should_prompt(is_interactive) &&
			(rl = getline(&input, &n, stdin) > -1))
	{
		inputcp = _strdup(input);
		cmd = strtok(input, " \n");
		is_exit = _are_strs_eql(cmd, "exit");
		if (is_exit)
		{
			_str_free_all(2, input, inputcp);
			exit(0);
		}
		if (cmd)
		{
			exe_path = get_path(cmd);
			if (exe_path)
			{
				exe = execute(exe_path, inputcp);
				if (exe != EXE_SUCCESS)
					handle_error(cmd);
				if (exe == EXE_ERR) /*child process fail executing */
				{
					_str_free_all(3, input, inputcp, exe_path);
					exit(EXIT_FAILURE);
				}
				free(exe_path);
			}
			else
				handle_error(cmd);
		}
		free(inputcp);
	}
	free(input);
}

/**
 * execute - parse input and execute the cmd if any.
 * @exe_path: the found execution path (argv[0]).
 * @input: input line to be parsed.
 *
 * Return: void
 */
int execute(char *exe_path, char *input)
{
	char **argv, **env = environ;
	pid_t child_pid;
	int exe;

	child_pid = fork();
	switch (child_pid)
	{
		case -1:
			return (FORK_ERR);
		case 0:
			argv = get_argv(input);
			exe = execve(exe_path, argv, env);
			if (exe == -1)
			{
				free_argv(argv);
				return (EXE_ERR);
			}
			break;
		default:
			wait(NULL);
	}
	return (0);
}

