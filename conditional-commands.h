#ifndef _CONDITIONAL_CMDS_H_
#define _CONDITIONAL_CMDS_H_

/**
 * enum list_type - Enumeration for List Types
 * @CONDITIONAL_CMD: used to specifythe type of a list,
 * such as a ConditionalCommandList.
 * Description:
 * This enumeration defines different types of lists. Currently,
 * it includes only the type `CONDITIONAL_CMD`.
 */
typedef enum list_type
{
	CONDITIONAL_CMD
} ListType;
/**
 * struct commen_list_s - Common List Structure
 * @list_type: The type of the list (e.g., CONDITIONAL_CMD)
 *
 * Description:
 * This structure represents a common list that can store various
 * types of lists.
 * It includes a field `list_type` to specify the type of the list, such as
 * CONDITIONAL_CMD. This structure can be used as part of more complex data
 * structures to categorize and manage different types of lists.
 */
typedef struct commen_list_s
{
	ListType list_type;
} CommenList;
/**
 * enum token_type - Enumeration for Token Types
 * @AND: Represents a logical AND token.
 * @OR: Represents a logical OR token.
 * @CMD: Represents a command token.
 */
typedef enum token_type
{
	AND, OR, CMD
} TokenType;

typedef char *StringValue;

/**
 * struct conditional_cmd_s - Structure for Conditional Command Nodes
 * @token_type: The type of the node (e.g., CMD, AND, OR)
 * @value: The value associated with the node (e.g., command string)
 * @next: Pointer to the next ConditionalCommand node in the list
 *
 * Description:
 * This structure represents a node in a list of conditional commands.
 * It includes fields for specifying the type of the node (token_type),
 * storing a value associated with the node (value), and a pointer to the next
 * node in the list (next).
 * The token_type can be one of CMD, AND, or OR, and the value field can store
 * additional information (e.g., command string).
 */
typedef struct conditional_cmd_s
{
	TokenType token_type;
	StringValue value;
	struct conditional_cmd_s *next;
} ConditionalCommand;

/**
 * struct conditional_cmd_list_s - Structure for Conditional Command Lists
 * @common: Pointer to a CommenList structure (common list properties)
 * @head: Pointer to the first ConditionalCommand node in the list
 * @tail: Pointer to the last ConditionalCommand node in the list
 * @size: The number of nodes in the list
 *
 * Description:
 * This structure represents a list of conditional commands.
 * It includes a pointer to a CommenList structure (common) to store common
 * list properties, such as the list type. The head and tail pointers indicate
 * the first and last nodes in the list, and the size field keeps track
 * of the number of nodes in the list.
 */
typedef struct conditional_cmd_list_s
{
	CommenList *common;
	ConditionalCommand *head;
	ConditionalCommand *tail;
	unsigned int size;
} ConditionalCommandList;

void generic_list_free(void *list_ptr);
void free_conditional_command_List(ConditionalCommandList *list);
ConditionalCommandList *parse_cond_cmds(char *conditional_cmd);
ConditionalCommandList *create_conditional_command_List(void);
void link_node(ConditionalCommandList *const list,  ConditionalCommand *node);
ConditionalCommand *create_conditional_command_node(TokenType type, char *val);

#endif
