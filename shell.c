#include "shell.h"

#define EXE_SUCCESS (0)
#define EXE_ERR (-1)
#define FORK_ERR (-2)
#define EXEPATH_ERR (-3)


#define IS_EXIT(cmd) (_are_strs_eql(cmd, "exit"))
#define IS_ENV(cmd) (_are_strs_eql(cmd, "env"))
#define IS_ENV_CHANGE(cmd)  \
	(_are_strs_eql(cmd, "setenv") || _are_strs_eql(cmd, "unsetenv"))

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
	short is_interactive = isatty(STDIN_FILENO);

		while (should_prompt(is_interactive) &&
			(rl = getline(&input, &n, stdin) > -1))
	{
		inputcp = _strdup(input);
		cmd = _strtok(input, " \n");

		if (IS_ENV(cmd))
			print_env();
		else if (IS_ENV_CHANGE(cmd))
			handle_env_change(inputcp);
		else if (IS_EXIT(cmd))
			handle_exit(_strtok(NULL, " \n"), 2, input, inputcp);
		else if (cmd)
		{
			exe_path = get_path(cmd);
			execute_result = execute(exe_path, inputcp);
			if (execute_result != EXE_SUCCESS)
				handle_error(cmd, 0);
			if (execute_result == EXE_ERR) /*child process fail executing */
			{
				_str_free_all(3, input, inputcp, exe_path);
				free_env(environ);
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

