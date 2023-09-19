#include "shell.h"
#include "memory_allocation.h"

#define EXE_SUCCESS (0)
#define EXE_ERR (-1)
#define FORK_ERR (-2)
#define EXEPATH_ERR (-3)


#define IS_EXIT(cmd) (_are_strs_eql(cmd, "exit"))
#define IS_CD(cmd) (_are_strs_eql(cmd, "cd"))
#define IS_ENV(cmd) (_are_strs_eql(cmd, "env"))
#define IS_ENV_CHANGE(cmd)  \
	(_are_strs_eql(cmd, "setenv") || _are_strs_eql(cmd, "unsetenv"))

void parse_command(char *command_with_args);

/**
 * shell - the shell interface
 *      reads from stdin and execute the command given.
 *
 * Return: void
 */
void shell(void)
{
	char *Input_line = NULL, **commands;
	size_t i, n = 0;
	ssize_t rl;
	short is_interactive = isatty(STDIN_FILENO);
	long mem_alloc_id, mem_alloc_id2;
	AllocatedMemory *am1, *am2;

	while (should_prompt(is_interactive) &&
		(rl = getline(&Input_line, &n, stdin) > -1))
	{
		am1 = create_allocated_memory(STRING, Input_line);
		mem_alloc_id = push_allocated_memory(am1);

		commands = get_custom_delim_argv(Input_line, ";\n");
		am2 = create_allocated_memory(STRING_ARRAY, commands);
		mem_alloc_id2 = push_allocated_memory(am2);

		for (i = 0; commands[i]; i++)
		{
			parse_command(commands[i]);
		}

		free_string_array(commands, NULL);
		deallocate_memory(mem_alloc_id);
		deallocate_memory(mem_alloc_id2);
	}
	safe_free(Input_line);
}

/**
* parse_command - Parse and execute a command with arguments.
* @command_with_args: The command with arguments to parse and execute.
*
* Return: void
*/
void parse_command(char *command_with_args)
{
	char  *command_with_args_cp, *command, *exe_path;
	ssize_t execute_result;
	long id, id2;

	command_with_args_cp = _strdup(command_with_args);
	id = push_allocated_memory(
			create_allocated_memory(STRING, command_with_args_cp));
	command = _strtok(command_with_args, " \n");

	if (IS_ENV(command))
		print_env();
	else if (IS_ENV_CHANGE(command))
		handle_env_change(command_with_args_cp);
	else if (IS_CD(command))
		handle_cd(command_with_args_cp);
	else if (IS_EXIT(command))
		handle_exit(_strtok(NULL, " \n"));
	else if (command)
	{
		exe_path = get_path(command);
		id2 = push_allocated_memory(create_allocated_memory(STRING, exe_path));

		execute_result = execute(exe_path, command_with_args_cp);
		if (execute_result != EXE_SUCCESS)
			handle_error(command, 0);
		if (execute_result == EXE_ERR) /*child process fail executing */
		{
			clean_allocated_memory();
			exit(EXIT_FAILURE);
		}
		if (exe_path)
			safe_free(exe_path);
		deallocate_memory(id2);
	}
	safe_free(command_with_args_cp);

	deallocate_memory(id);
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
	long mem_alloc_id;
	AllocatedMemory *am1;

	if (!exe_path)
		return (EXEPATH_ERR);
	child_pid = fork();
	switch (child_pid)
	{
		case -1:
			return (FORK_ERR);
		case 0:
			argv = get_argv(input);
			am1 = create_allocated_memory(STRING_ARRAY, argv);
			mem_alloc_id = push_allocated_memory(am1);

			exe = execve(exe_path, argv, env);
			if (exe == -1)
			{
				free_string_array(argv, NULL);
				deallocate_memory(mem_alloc_id);
				return (EXE_ERR);
			}
			break;
		default:
			wait(NULL);
	}
	return (0);
}
