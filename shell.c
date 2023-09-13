#include "shell.h"

#define EXE_SUCCESS (0)
#define EXE_ERR (-1)
#define FORK_ERR (-2)
#define EXEPATH_ERR (-3)

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
	ssize_t rl, execute_result;
	short isExitCommand, isEnvCommand, is_interactive = isatty(STDIN_FILENO);

		while (should_prompt(is_interactive) &&
			(rl = getline(&input, &n, stdin) > -1))
	{
		inputcp = _strdup(input);
		cmd = strtok(input, " \n");
		isExitCommand = _are_strs_eql(cmd, "exit");
		isEnvCommand = _are_strs_eql(cmd, "env");

		if (isEnvCommand)
			print_env();
		else if (isExitCommand)
		{
			_str_free_all(2, input, inputcp);
			exit(0);
		}
		else if (cmd)
		{
			exe_path = get_path(cmd);
			execute_result = execute(exe_path, inputcp);
			if (execute_result != EXE_SUCCESS)
				handle_error(cmd);
			if (execute_result == EXE_ERR) /*child process fail executing */
			{
				_str_free_all(3, input, inputcp, exe_path);
				exit(EXIT_FAILURE);
			}
			if (exe_path)
				free(exe_path);
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

	if (!exe_path)
		return (EXEPATH_ERR);
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

