#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tree.h"
#include "utils.h"
#include "list.h"
#include "common.h"
#include "lager.h"

struct node
{
  node_t *left;
  node_t *right;
  tree_key_t key;
  T value;
};

struct tree
{  
  element_copy_fun copy_elem_f;
  key_free_fun free_key_f;
  element_free_fun free_elem_f;
  element_comp_fun cmp_elem_f;
  node_t *root;
};


tree_t *tree_new(element_copy_fun element_copy, key_free_fun key_free, element_free_fun elem_free, element_comp_fun compare)
{
  tree_t *empty_tree = calloc(1, sizeof(tree_t));
  empty_tree->root = calloc(1, sizeof(tree_t));

  empty_tree->copy_elem_f = element_copy;
  empty_tree->free_key_f = key_free;
  empty_tree->free_elem_f = elem_free;
  empty_tree->cmp_elem_f = compare;

  return empty_tree;
}



void tree_delete_node(node_t *node, bool delete_keys, bool delete_elements, key_free_fun free_key, element_free_fun free_elem)
{
  while(node!=NULL)
    {
      tree_delete_node(node->left, delete_keys, delete_elements, free_key, free_elem);
      tree_delete_node(node->right, delete_keys, delete_elements, free_key, free_elem);

      if(delete_keys)
        {
          free_key(node->key);
        }

      if(delete_elements)
        {
          free_elem((elem_t)node->value);
        }
    }
}


void tree_delete(tree_t *tree, bool delete_keys, bool delete_elements)
{
  key_free_fun free_key = tree->free_key_f;
  element_free_fun free_elem = tree->free_elem_f;
  
  while(tree->root!=NULL)
    {
      tree_delete_node(tree->root->left, delete_keys, delete_elements, free_key, free_elem);
      tree_delete_node(tree->root->right, delete_keys, delete_elements, free_key, free_elem);

      if(delete_keys)
        {
          free_key(tree->root->key);
        }
      
      if(delete_elements)
        {
          free_elem((elem_t)tree->root->value);
        }
    }
}
