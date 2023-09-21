#include "conditional-commands.h"
#include "shell.h"

/**
 * parse_cond_cmds - Parse Conditional Commands from a String
 * @conditional_cmd: The input string containing conditional commands
 *
 * Description:
 * This function parses conditional commands from the input string and creates
 * a ConditionalCommandList to store them. It recognizes commands separated by
 * '&&' or '||' and creates nodes accordingly.
 *
 * Return:
 * On success, a pointer to the created ConditionalCommandList is returned.
 * On failure (if memory allocation fails), NULL is returned.
 */
ConditionalCommandList *parse_cond_cmds(char *conditional_cmd)
{
	int i, token_start;
	char c, next_c, *token;
	ConditionalCommand *node;
	ConditionalCommandList *list = create_conditional_command_List();

	for (i = 0; (c = conditional_cmd[i]) != '\0'; i++)
		if (c != ' ')
			break;
	if (conditional_cmd[i] == '|' || conditional_cmd[i] == '&')
		return (list);

	token_start = i;
	for (; (c = conditional_cmd[i]) != '\0'; i++)
	{
		next_c = conditional_cmd[i + 1];
		if ((c == '|' && next_c == '|') || (c == '&' && next_c == '&'))
		{
			token = _strslice(conditional_cmd, token_start, i - 1);
			node = create_conditional_command_node(CMD, token);
			link_node(list, node);
			node = create_conditional_command_node(c ==  '&' ? AND : OR, NULL);
			link_node(list, node);
			token_start = i + 2;
			i++;
		}

		if (!conditional_cmd[i + 1] && token_start <= i)
		{
			token = _strslice(conditional_cmd, token_start, i);
			node = create_conditional_command_node(CMD, token);
			link_node(list, node);
		}
	}
	return (list);
}

/**
 * generic_list_free - Free a Generic List
 * @list_ptr: Pointer to the pointer of the list to be freed
 *
 * Description:
 * This function is a generic list freeing function that can handle different
 * list types.
 * It frees the memory associated with the provided list pointer.
 */
void generic_list_free(void *list_ptr) /*   void (*free_list(void)) */
{
	CommenList **common_ptr;
	ListType list_type;
	/*
	 * list_or is a ptr to the list ptr
	 * * -> *list -> *commen -> (inside comment)
	 * CommenList *commen = **((CommenList ***)list_ptr);
	 */
	if (!list_ptr)
		return;

	common_ptr = (CommenList **)list_ptr;
	/* CommentList *common = *common_ptr;*/

	list_type = (*common_ptr)->list_type;

	if (list_type == CONDITIONAL_CMD)
	{
		free_conditional_command_List(list_ptr);
	}
}
