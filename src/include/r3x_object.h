#ifndef __R3X_OBJECT_H
#define __R3X_OBJECT_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <r3x_stack.h>
typedef enum ObjectType {
	R3X_OBJECT_INT,
	R3X_OBJECT_STRING,
	R3X_OBJECT_POINTER,
	R3X_OBJECT_STACK,
	R3X_OBJECT_BUFFER
} ObjectType;
typedef struct object {
/** What the object is **/
	ObjectType Type;
/** The value, for a int it's a a normal integer, for string it's a char*, for pointer it's a void*, for stack it's a stack_t* and for buffer it's a buffer_t* **/
	int Value;
/** Object Identification Number, use by GC and CPU to keep track of objects and access them.. */
	int ObjectID;
} object_t; 
typedef struct ObjectList {
	int* Objects;
	unsigned int NumberOfObjects;
	unsigned int Index;
} ObjectList_t;
object_t* pushObj(vstack_t* stack, ObjectList_t* List, ObjectType Type);
void popObj(vstack_t* stack, ObjectList_t* List);
object_t* accessObj(ObjectList_t* List, unsigned int Index);
#endif
