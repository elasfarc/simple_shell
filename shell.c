#include "shell.h"
#include "memory_allocation.h"
#include "conditional-commands.h"

#define EXE_SUCCESS (0)
#define EXE_ERR (-1)
#define FORK_ERR (-2)
#define EXEPATH_ERR (-3)

#define IS_EXIT(cmd) (_are_strs_eql(cmd, "exit"))
#define IS_CD(cmd) (_are_strs_eql(cmd, "cd"))
#define IS_ENV(cmd) (_are_strs_eql(cmd, "env"))
#define IS_ENV_CHANGE(cmd)  \
	(_are_strs_eql(cmd, "setenv") || _are_strs_eql(cmd, "unsetenv"))

int parse_command(char *command_with_args);


/**
 * executeConditionalCommands - Execute Conditional Commands
 * @conditional_cmd: The input string containing conditional commands
 *
 * Description:
 * This function takes an input string `conditional_cmd` that contains
 * conditional commands and executes them based on the defined
 * logic (AND and OR operators).
 * It parses the input into a ConditionalCommandList, processes each command
 * in the list, and handles conditional logic as specified by the tokens.
 *
 * Return: the last executed command result within a compound command.
 *
 * Behavior:
 * - The function first parses the input string into a ConditionalCommandList.
 * - It processes each command in the list one by one, considering the command
 * type (CMD) and the logic operators (AND, OR).
 * - If a command's return value is zero (success) and the logic is OR,
 * the function continues to the next command.
 * - If a command's return value is non-zero (failure) and the logic is AND,
 * the execution stops.
 * - The function handles syntax errors and reports them to STDERR if necessary
 *
 * Note:
 * - The function makes use of external functions such as `parse_command` and
 *   `_strcat` for parsing and string manipulation.
 */
int executeConditionalCommands(char *conditional_cmd)
{
	int last_cmd_return;
	long id;
	ConditionalCommandList *list;
	ConditionalCommand *node;

	list = parse_cond_cmds(conditional_cmd);
	id = push_allocated_memory(create_allocated_memory(LINKED_LIST_PTR, list));

	if (!list->head)
	{
		char *err = _strcat(_strdup("syntax error near unexpected token "),
		 (conditional_cmd[0] == '|' ? "|\n" : "&\n"), NULL);
		write(STDERR_FILENO, err, _strlen(err));
		safe_free(err);
		/*generic_list_free(&list); */
		/* deallocate_memory(id); */
		return (2);
	}
	node = list->head;
	while (node)
	{
		if (node->token_type == CMD)
			last_cmd_return = parse_command(node->value);

		else if ((last_cmd_return == 0 && node->token_type == OR) ||
			(last_cmd_return != 0 && node->token_type == AND))
			break;
		node = node->next;
	}
	generic_list_free(list);
	deallocate_memory(id);
	return (last_cmd_return);
}

/**
 * get_content_befor_comment - Extracts content before
 * a comment symbol in a string.
 *
 * @line: The input string from which to extract content (modified in-place).
 *
 * Description: This function modifies the input string @line
 * to remove any content that appears after a `#` character,
 * including the `#` character itself, only if the `#` is preceded by a space
 * or if it's the first character in the string.
 * The modified string is then returned.
 *
 * Return: A pointer to the modified string with content before
 * the comment symbol.
 */
char *get_content_befor_comment(char *const line)
{
	int i;

	for (i = 0; line[i] != '\0'; i++)
		if (line[i] == '#')
			if (i == 0 || line[i - 1] == ' ')
			{
				line[i] = '\0';
				break;
			}
	return (line);
}

/**
 * shell - the shell interface
 *      reads from stdin and execute the command given.
 *
 * Return: void
 */
int shell(void)
{
	char *input_line = NULL, **Input_line_ptr = &input_line, **compound_commands;
	size_t i, n = 0;
	ssize_t rl;
	short is_interactive = isatty(STDIN_FILENO);
	long mem_alloc_id, mem_alloc_id2;
	AllocatedMemory *am1, *am2;
	int last_cmd_result = 0;

	 am1 = create_allocated_memory(STRING_POINTER, Input_line_ptr);
	 mem_alloc_id = push_allocated_memory(am1);

	while (should_prompt(is_interactive) &&
		(rl = getline(Input_line_ptr, &n, stdin) > -1))
	{
		char *content_before_comment = get_content_befor_comment(input_line);

		compound_commands = get_custom_delim_argv(content_before_comment, ";\n");
		am2 = create_allocated_memory(STRING_ARRAY, compound_commands);
		mem_alloc_id2 = push_allocated_memory(am2);

		for (i = 0; compound_commands[i]; i++)
		{
			char *trimmed = trim(compound_commands[i]);
			short is_empty_string = _are_strs_eql(trimmed, "");

			if (!is_empty_string)
				last_cmd_result = executeConditionalCommands(trimmed);
		}

		free_string_array(compound_commands, NULL);
		deallocate_memory(mem_alloc_id2);
	}
	safe_free(input_line);
	deallocate_memory(mem_alloc_id);
	return (last_cmd_result);
}

/**
* parse_command - Parse and execute a command with arguments.
* @command_with_args: The command with arguments to parse and execute.
*
* Return: void
*/
int parse_command(char *command_with_args)
{
	char  *command_with_args_cp, *command, *exe_path;
	ssize_t execute_result;
	long id, id2;

	command_with_args_cp = _strdup(command_with_args);
	id = push_allocated_memory(
			create_allocated_memory(STRING, command_with_args_cp));
	command = _strtok(command_with_args, " \n");

	if (IS_ENV(command))
		execute_result = print_env();
	else if (IS_ENV_CHANGE(command))
		execute_result = handle_env_change(command_with_args_cp);
	else if (IS_CD(command))
		execute_result = handle_cd(command_with_args_cp);
	else if (IS_EXIT(command))
		execute_result = handle_exit(_strtok(NULL, " \n"));
	else if (command)
	{
		exe_path = get_path(command);
		if (exe_path)
		{
			id2 = push_allocated_memory(create_allocated_memory(STRING, exe_path));
			execute_result = execute(exe_path, command_with_args_cp);
			if (execute_result != EXE_SUCCESS)
				handle_error(command, 0);

			if (execute_result == EXE_ERR) /*child process fail executing */
			{
				clean_allocated_memory();
				exit(errno);
			}
			safe_free(exe_path);
			deallocate_memory(id2);
		}
		execute_result = errno;
	}
	safe_free(command_with_args_cp);
	deallocate_memory(id);
	return (execute_result);
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
	int exe, status, child_exit_code;
	long mem_alloc_id;
	AllocatedMemory *am1;


	child_pid = fork();
	switch (child_pid)
	{
		case -1:
			return (errno);
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
			wait(&status);
			if (WIFEXITED(status))
			{
				child_exit_code = WEXITSTATUS(status);
				errno = child_exit_code;
			}
	}
	return (EXE_SUCCESS);
}
