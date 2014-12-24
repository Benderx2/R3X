#ifndef __MACROS_H
#define __MACROS_H
#include <stdlib.h>
#define new(x) (x*)malloc(sizeof(x))
#define delete(x) free(x)
#endif