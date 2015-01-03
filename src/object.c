/*
Copyright (c) 2015 Benderx2, 
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of R3X nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
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
