#include "memory_allocation.h"
#include "../shell.h"

/**
 * init_alloc_mem_stack - Initializes an AllocatedMemoryStack.
 * @mem_stack_ptr: Pointer to the AllocatedMemoryStack to be initialized.
 *
 * Return: void
 */
void init_alloc_mem_stack(AllocatedMemoryStack **mem_stack_ptr)
{
	(*mem_stack_ptr)->head = NULL;
	(*mem_stack_ptr)->tail = NULL;
	(*mem_stack_ptr)->size = 0;
}

/**
 * get_alloc_mem_stack - Retrieves or creates a global AllocatedMemoryStack.
 *
 * Return: Pointer to the global AllocatedMemoryStack.
 */
AllocatedMemoryStack *get_alloc_mem_stack()
{
	static AllocatedMemoryStack *mem_stack;

	if (!mem_stack)
	{
		mem_stack = malloc(sizeof(AllocatedMemoryStack));
		init_alloc_mem_stack(&mem_stack);
	}
	return (mem_stack);
}

