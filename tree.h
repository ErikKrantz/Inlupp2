#ifndef __tree_h__
#define __tree_h__

#include <stdbool.h>

typedef void *T;

typedef struct element element_t;  //typedef void *K;

typedef int(*cmp_t)(element_t, element_t);

typedef struct tree_node tree_node_t;

typedef struct tree_root tree_root_t;

typedef tree_root_t *tree_new(cmp_t *cmp);

void tree_balance(tree_root_t *tree);

#endif
