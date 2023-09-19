#ifndef _ALLOC_MEM_
#define _ALLOC_MEM_

/**
 * enum alloc_type - Enumeration of allocation types.
 * @STRING: Represents a single string allocation.
 * @STRING_ARRAY: Represents an array of strings allocation.
 *
 */
typedef enum alloc_type
{
	STRING, STRING_ARRAY
} alloc_type_t;

/**
 * struct AllocatedMemory -  hold allocated memory information.
 *
 * @type: The type of allocation.
 * @data: Pointer to the allocated memory data.
 */
typedef struct AllocatedMemory
{
	alloc_type_t type;
	void *data;
} AllocatedMemory;

/**
 * struct AllocatedMemoryNode - representing a node
 * in the allocated memory stack.
 *
 * @id: Unique identifier for the node.
 * @data: Pointer to the allocated memory.
 * @next: Pointer to the next node in the stack.
 * @prev: Pointer to the previous node in the stack.
 */
typedef struct AllocatedMemoryNode
{
	long int id;
	AllocatedMemory *data;
	struct AllocatedMemoryNode *next;
	struct AllocatedMemoryNode *prev;
} AllocatedMemoryNode;

/**
 * struct AllocatedMemoryStack - representing a stack of allocated memory nodes
 *
 * @head: Pointer to the head of the stack.
 * @tail: Pointer to the last added element to the stack.
 * @size: The number of nodes in the stack.
 */
typedef struct AllocatedMemoryStack
{
		AllocatedMemoryNode *head;
		AllocatedMemoryNode *tail;
		unsigned int size;
} AllocatedMemoryStack;



AllocatedMemoryStack *get_alloc_mem_stack();

long int push_allocated_memory(AllocatedMemory *memory_alloc);
int deallocate_memory(long);
int pop_allocated_memory(void);

AllocatedMemoryNode *get_top_allocated_memory(void);

AllocatedMemory *create_allocated_memory(alloc_type_t type, void *data);


#endif
