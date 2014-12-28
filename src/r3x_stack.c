#include <r3x_stack.h>
#include <nt_malloc.h>
int PushtoStack(vstack_t* stack, int object);
int PopFromStack(vstack_t* stack);
int GetItemS(vstack_t* stack, int idx);
vstack_t* CreateStack(void);
int Duplicate(vstack_t* stack);
void SetItemS(vstack_t* stack, int idx, int i);
void DestroyStack(vstack_t* Stack);
void init_stack_construct(void)
{
	Stack.Duplicate = &Duplicate;
	Stack.GetItem = &GetItemS;
	Stack.Push = &PushtoStack;
	Stack.Pop = &PopFromStack;
	Stack.Create = &CreateStack;
	Stack.SetItem = &SetItemS;
	Stack.DestroyStack = &DestroyStack;
}
int Duplicate(vstack_t* stack)
{
	PushtoStack(stack, stack->content[stack->stack_count]);
	return 0;
}
int GetItemS(vstack_t* stack, int idx)
{
	if (idx > stack->stack_count){ return -1; }
	else { return (stack->content[idx]); }
}
void SetItemS(vstack_t* stack, int idx, int i)
{
	if (idx > stack->stack_count){ return; }
	else { stack->content[idx] = i; }
}
vstack_t* CreateStack(void)
{
	vstack_t* new_stack = nt_malloc(sizeof(vstack_t)); // Allocate stack structure
	// Set stack and other vars to null
	new_stack->content = NULL;
	new_stack->top_of_stack = 0;
	new_stack->stack_count = 0;
	return new_stack;
}
int PushtoStack(vstack_t* stack, int object)
{
	// Check if we are going to overflow
	if (stack->stack_count == stack->top_of_stack){
		// Check if there exists a buffer
		if (stack->stack_count == 0){
			// Allocate, null out and set.
			int* new_buf = NULL;
			new_buf = nt_malloc(DEFAULT_STACK_SIZE);
			stack->stack_count = 16;
			stack->content = new_buf;
		}
		else {
			// Allocate it double the size to make sure we don't run out too soon
			int* new_buf = nt_malloc(stack->stack_count * 2 * sizeof(int));
			// Copy the old buffer
			memcpy(new_buf, stack->content, stack->stack_count * sizeof(int));
			// Free it, set the new buffer, and the new size
			nt_free(stack->content);
			stack->content = new_buf;
			stack->stack_count = stack->stack_count * 2;
		}
	}
	// finally push it.
	stack->content[stack->top_of_stack] = object;
	stack->top_of_stack++;
	return 0;
}
int PopFromStack(vstack_t* stack)
{
	// if top of stack is 0, return an underflow error else decrement top of stack
	if (stack->top_of_stack == 0){
		return STACK_UFLOW_ERR;
	}
	else {
		stack->top_of_stack--;
		return stack->content[stack->top_of_stack];
	}
}
void DestroyStack(vstack_t* stack)
{
	nt_free(stack->content);
	stack->top_of_stack = 0;
	stack->stack_count = 0;
	nt_free(stack);
}
