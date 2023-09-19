#include "memory_allocation.h"
#include "shell.h"

/**
 * push_allocated_memory - Pushes allocated memory onto the stack.
 * @memory_alloc: memoryAlloc Pointer to the allocated memory.
 *
 * Return: The ID of the pushed memory allocation, or 0 on error.
 */
long int push_allocated_memory(AllocatedMemory *memory_alloc)
{
	AllocatedMemoryStack *stack = get_alloc_mem_stack();
	AllocatedMemoryNode *node = malloc(sizeof(AllocatedMemoryNode));

	node->id = (long int)node;
	if (!node)
		return (0);
	node->data = memory_alloc;
	node->next = NULL;
	node->prev = stack->tail ? stack->tail : NULL;
	stack->tail ? stack->tail->next = node : (void *)0;
	stack->tail = node;
	stack->head = stack->head ? stack->head : node;
	return (node->id);
}

/**
 * deallocate_memory - eallocates memory with the specified ID from the stack.
 *
 * @id: The ID  of the memory allocation to deallocate.
 *
 * Return: 1 if deallocation is successful, 0 otherwise.
 */

int deallocate_memory(long id)
{
	AllocatedMemoryStack *stack = get_alloc_mem_stack();
	AllocatedMemoryNode *current = stack->head;

	while (current)
	{
		if (current->id == id)
		{
			if (current->prev)
				current->prev->next = current->next;
			else
				stack->head = current->next;

			if (current->next)
				current->next->prev = current->prev;
			else
				stack->tail = current->prev;

			safe_free(current->data);
			safe_free(current);
			return (1);

		}
		current = current->next;
	}
	return (0);
}

/**
 * pop_allocated_memory - Pops the top allocated memory from the stack.
 *
 * Return: 1 if a memory allocation is popped successfully,
 * 0 if the stack is empty.
 */
int pop_allocated_memory(void)
{
	AllocatedMemoryNode *to_be_removed;
	AllocatedMemoryStack *stack = get_alloc_mem_stack();

	if (!stack->tail)
		return (0);
	to_be_removed = stack->tail;

	stack->tail = stack->tail->prev;
	if (stack->tail)
		stack->tail->next = NULL;
	else
		stack->head = NULL;
	safe_free(to_be_removed->data);
	safe_free(to_be_removed);
	return (1);
}

/**
 * clean_allocated_memory - Cleans up all allocated memory on the stack
 * and frees resources.
 *
 * Return: void
 */
void clean_allocated_memory(void)
{
	char *str, **str_arr;
	AllocatedMemoryNode *top_node = get_top_allocated_memory();
	alloc_type_t type;

	while (top_node)
	{
		type = top_node->data->type;

		if (type == STRING)
		{
			str = (char *)top_node->data->data;
			safe_free(str);
		}
		else if (type == STRING_ARRAY)
		{
			str_arr = (char **)top_node->data->data;
			free_string_array(str_arr, NULL);
		}
		deallocate_memory(top_node->id);
		top_node = get_top_allocated_memory();
	}
	safe_free(get_alloc_mem_stack());
	free_string_array(environ, NULL);
}
