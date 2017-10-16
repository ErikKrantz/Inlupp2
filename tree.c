#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tree.h"
#include "utils.h"
#include "list.h"
#include "lager.h"

struct element
{
  void *p;
  int i;
  //uint u;
  float f;
};

struct tree_node
{
  tree_node_t *left;
  tree_node_t *right;
  element_t key;
  T value;
};

struct tree_root
{
  cmp_t *cmp_f;
  tree_node_t *root;
};


tree_root_t *tree_new(cmp_t *cmp)
{
  tree_root_t *empty_tree = calloc(1, sizeof(tree_root_t));
  empty_tree->root = calloc(1, sizeof(tree_root_t));
  empty_tree->cmp_f = cmp;

  return empty_tree;
}


void tree_balance(tree_root_t *tree)
{
  
}

void node_new(element_t, tree_node_t *tree)
{

}
