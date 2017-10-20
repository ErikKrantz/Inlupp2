#ifndef __list_h__
#define __list_h__

#include <stdbool.h>

typedef struct link link_t;

/// Change this definition and replace int with the appropriate type
/// in your program.
typedef void *L;

/// This function is used in list_delete() to allow the lists which are
/// the sole owners of their data to free the data on delete.
typedef void(*list_action)(L element);

/// Define struct list in your .c file not here! (why?)
typedef struct list list_t;

/*

typedef struct element elem_t;

typedef int(*cmp_t)(elem_t, elem_t);

*/

#endif
