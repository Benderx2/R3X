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
