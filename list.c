#include "list.h"
#include "utils.h"
#include "tree.h"
#include <stdlib.h>
#include "lager.h"
#include <stdio.h>

struct element
{
  void *p;
  int   i;
  // uint  u;
  float f;
};


struct link
{
  L *element;
  link_t *next;
};

struct list
{
  cmp_t *cmp_f;
  link_t *first;
  link_t *last;
};


list_t *list_new(cmp_t *cmp)
{
  list_t *result = calloc(1,sizeof(list_t));
  if (result)
    {
      result->cmp_f = cmp;
    }
  return result;
}


void list_insert(list_t *list, element_t elem)
{
  tree_node_t **c = &(list->first);
  while (*c && list->cmp_f((*c)->elem, elem))
    {
      c = &((*c)->next);
    }
  *c = node_new(elem, *c);
}



int main()
{
  return 0;
}


