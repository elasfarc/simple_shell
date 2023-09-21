#include "conditional-commands.h"
#include "shell.h"

/**
 * create_common_list - Create a CommonList
 * @type: The type of the list (e.g., CONDITIONAL_CMD)
 *
 * Description:
 * This function creates a new CommonList with the specified type and returns a
 * pointer to it.
 * The CommonList is used to store different types of lists in the shell.
 *
 * Return:
 * On success, a pointer to the created CommonList is returned.
 * On failure (if memory allocation fails), NULL is returned.
 */
CommenList *create_common_list(ListType type)
{
	CommenList *common_list = malloc(sizeof(CommenList));

	if (!common_list)
		return (NULL);
	common_list->list_type = type;

	return (common_list);
}

/**
 * create_conditional_command_List - Create a ConditionalCommandList
 *
 * Description:
 * This function creates a new ConditionalCommandList and
 * returns a pointer to it.
 * The ConditionalCommandList is used to store conditional
 * commands in the shell.
 *
 * Return:
 * On success, a pointer to the created ConditionalCommandList is returned.
 * On failure (if memory allocation fails), NULL is returned.
 */
ConditionalCommandList *create_conditional_command_List()
{
	ConditionalCommandList *list;
	CommenList *commen = create_common_list(CONDITIONAL_CMD);

	if (!commen)
		return (NULL);
	list = malloc(sizeof(ConditionalCommandList));
	if (!list)
		return (NULL);
	list->common = commen;
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;

	return (list);
}

/**
 * free_conditional_command_List - Free a ConditionalCommandList
 * @list: Pointer to the ConditionalCommandList to be freed
 *
 * Description:
 * This function frees the memory associated with a ConditionalCommandList,
 * including its nodes and the associated CommonList.
 */
void free_conditional_command_List(ConditionalCommandList *list)
{
	ConditionalCommand *node, *head = !list ? NULL : (list)->head;

	while (head)
	{
		node = head;
		head = node->next;
		if (node->token_type == CMD)
			safe_free(node->value);
		free(node);
	}
	safe_free(list->common);
	safe_free(list);
}

/**
 * create_conditional_command_node - Create a ConditionalCommand Node
 * @type: The token type of the node (e.g., CMD)
 * @val: The value associated with the node
 *
 * Description:
 * This function creates a new ConditionalCommand node with the specified
 * type and value and returns a pointer to it. The node is used to represent
 * commands in the ConditionalCommandList.
 *
 * Return:
 * On success, a pointer to the created ConditionalCommand node is returned.
 * On failure (if memory allocation fails), NULL is returned.
 */
ConditionalCommand *create_conditional_command_node(TokenType type, char *val)
{
	ConditionalCommand *node = malloc(sizeof(ConditionalCommand));

	if (!node)
		return (NULL);
	node->next = NULL;
	node->token_type = type;
	node->value = val;

	return (node);
}


/**
 * link_node - Link a ConditionalCommand Node to a ConditionalCommandList
 * @list: Pointer to the ConditionalCommandList
 * @node: Pointer to the ConditionalCommand Node to be linked
 *
 * Description:
 * This function links a ConditionalCommand node to the ConditionalCommandList.
 * It updates the list's head, tail, and size accordingly.
 */
void link_node(ConditionalCommandList *const list,  ConditionalCommand *node)
{
	list->tail ? (list->tail->next = node) : (void *)0;
	list->head = list->head ? list->head : node;
	list->tail = node;
	if (list->size)
		list->size++;

}
