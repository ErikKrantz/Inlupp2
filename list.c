#include "list.h"
#include "utils.h"
//#include "tree.h"
#include <stdlib.h>
#include "lager.h"
#include <stdio.h>
#include "common.h"

struct link
{
  L *element;
  link_t *next;
};

struct list
{
  element_comp_fun comp_f;
  element_copy_fun copy_f;
  element_free_fun free_f;
  link_t *first;
  link_t *last;
};

list_t *list_new(element_copy_fun copy, element_free_fun free, element_comp_fun compare)
{
  list_t *new_list = calloc(1, sizeof(list_t));
  if (new_list)
    {
      //  new_list->comp_f = compare(copy);
    }
  return new_list;
}

/*
list_t *list_new(cmp_t *cmp)
{
  list_t *result = calloc(1,sizeof(list_t));
  if (result)
    {
      result->cmp_f = cmp;
    }
  return result;
}
*/

void list_insert(list_t *list, int index, elem_t elem)
{
  return;
}

void list_append(list_t *list, elem_t elem)
{
  return;
}

void list_remove(list_t *list, int index, bool delete)
{
  return;
}

bool list_get(list_t *list, int index, elem_t *result)
{
  return true;
}

bool list_first(list_t *list, elem_t *result)
{
  return true;
}

bool list_last(list_t *list, elem_t *result)
{
  return true;
}

int list_length(list_t *list)
{
  return 0;
}

void list_delete(list_t *list, bool delete)
{
  return;
}

bool list_apply(list_t *list, elem_apply_fun fun, void *data)
{
  return true;
}

int list_contains(list_t *list, elem_t elem)
{
  return 0;
}

int main()
{
  return 0;
}


