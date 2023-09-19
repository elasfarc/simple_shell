#include "memory_allocation.h"
#include "../shell.h"


/**
 * get_top_allocated_memory - gets the top allocated memory on the stack
 * without removing it.
 *
 * Return: Pointer to the top allocated memory node.
 */
AllocatedMemoryNode *get_top_allocated_memory(void)
{
	AllocatedMemoryStack *stack = get_alloc_mem_stack();

	return (stack->tail);
}
