#include <r3x_object.h>
#include <nt_malloc.h>
object_t* pushObj(vstack_t* stack, ObjectList_t* List, ObjectType Type)
{
	// Create an object and store it in a list...
	if(List->NumberOfObjects == 0){
		List->Objects = nt_malloc(16 * sizeof(int)); // 16 default size...
		List->Index = 0;
	}
	else if(List->Index >= List->NumberOfObjects){
		// Allocate twice as objects as the previous buffer and null out 
		int* new_buf = nt_malloc(List->NumberOfObjects * 2 * sizeof(int));
		memset(new_buf, 0, List->NumberOfObjects * 2 * sizeof(int));
		// Copy from the original to the new one and free the original
		memcpy(new_buf, List->Objects, List->NumberOfObjects * sizeof(int));
		nt_free(List->Objects);
		// Set the buffer and size accordingly 
		List->Objects =  new_buf;
		List->NumberOfObjects = List->NumberOfObjects * 2;
	}
	// Allocate new object and put it to the list.
	object_t* new_obj = nt_malloc(sizeof(object_t));
	new_obj->Type = Type;
	new_obj->Value = 0;
	new_obj->ObjectID = List->Index;
	List->Objects[List->Index] = (intptr_t)new_obj;
	// Increment Index 
	List->Index++;
	// Push to stack and return...
	Stack.Push(stack, new_obj->ObjectID);
	return new_obj;
}
void popObj(vstack_t* stack, ObjectList_t* List)
{
	Stack.Pop(stack); // Pop out the object ID
	List->Objects[List->Index-1] = 0;
}
object_t* accessObj(ObjectList_t* List, unsigned int Index)
{	
	if(Index >= List->Index){
		return NULL; // Return NULL if invalid Index..
	} else {
		return (object_t*)((intptr_t)(List->Objects[Index])); // Since List->Objects contains a pointer stored as integer, hence typecast...
	}
}
