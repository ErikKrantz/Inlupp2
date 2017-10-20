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

struct tree                   // cmp_elem_f -> cmp_key_f ??
{  
  element_copy_fun copy_elem_f;
  key_free_fun free_key_f;
  element_free_fun free_elem_f;
  element_comp_fun cmp_elem_f;
  node_t *root;
};

/// Creates a new tree
///
/// \param copy (may be NULL) a function applied to all elements when stored in the tree
/// \param key_free (may be NULL) used to free keys in tree_delete
/// \param elem_free (may be NULL) used to free elements in tree_delete
/// \param compare (may be NULL) used to compare keys
/// \returns: empty tree
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


/// Remove a tree along with all elem_t elements.
///
/// \param tree the tree
/// \param delete_keys if true, run tree's key_free function on all keys
/// \param delete_elements if true, run tree's elem_free function on all elements
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



int subtree_size(node_t *node)
{
  int size = 0;

  if(!node)
    {
      return size;
    }
  ++size;

  return size + subtree_size(node->left) + subtree_size(node->right);
}



/// Get the size of the tree 
///
/// \returns: the number of nodes in the tree
int tree_siz(tree_t *tree)
{
  int size = 0;
  node_t *root = tree->root;
  
  if (!root)
    {
      return size;    
    }
  ++size;
  return size + subtree_size(root->left) + subtree_size(root->right);
}



int node_depth(node_t *node)
{
  int depth = 0;
  
  if(!node)
    {
      return depth;
    }
  int left_depth = node_depth(node->left);
  int right_depth = node_depth(node->right);
  
  ++depth;
  if(left_depth>right_depth)
    {
      return left_depth;
    }
  else
    {
      return right_depth;
    }
}

/// Get the depth of the tree 
///
/// \returns: the depth of the deepest subtree
int tree_depth(tree_t *tree)
{
  int depth = 0;
  node_t *root = tree->root;

  if(!root)
    {
      return depth;
    }
  int left_depth = node_depth(root->left);
  int right_depth = node_depth(root->right);

  ++depth;
  
  if(left_depth>right_depth)
    {
      return left_depth;
    }
  else
    {
      return right_depth;
    }
}


bool subtree_has_key(node_t *node, tree_key_t key, element_comp_fun cmp_f)
{
  if(!node)
    {
      return false;
    }
  tree_key_t node_key = node->key;

  if(cmp_f(key, node_key))
    {
      return true;
    }
  else
    {
      return subtree_has_key(node->left, key, cmp_f) || subtree_has_key(node->right, key, cmp_f);
    }
}


/// Checks whether a key is used in a tree
///
/// Uses the tree's compare function to compare keys.
/// 
/// \param tree pointer to the tree
/// \param key the key to check for inclusion in the tree
/// \returns: true if key is a key in the tree
bool tree_has_key(tree_t *tree, tree_key_t key)
{
  node_t *root = tree->root;
  element_comp_fun cmp_f = tree->cmp_elem_f;
  
  if(!root)
    {
      return false;
    }

  tree_key_t node_key = root->key;

  if(cmp_f(key, node_key))
    {
      return true;
    }
  else
    {
      return subtree_has_key(root->left, key, cmp_f) || subtree_has_key(root->right, key, cmp_f);
    }    
}


bool node_balanced(node_t *node) // everything from here and under is NOT complete.
{
  if(!node)
    {
      return true;      
    }
  int left_depth = node_depth(node->left);
  int right_depth = node_depth(node->right);

  if(abs(left_depth-right_depth)<1)
    {
      return node_balanced(node);
    }
}

// returns true if tree is balanced according to AVL standards (or empty) else false.
bool tree_balanced(tree_t *tree)
{
  node_t *root = tree->root;
  if(!root)
    {
      return true;
    }
  int left_depth = node_depth(root->left);
  int right_depth = node_depth(root->right);

  if(abs(left_depth-right_depth)<1)
    {
      return node_balanced(root);      
    }
}


void left_rotate(int avl_case, node_t *node) 
{
  if(!avl_case)
    {      
      node_t *old_left = node->left;
      node_t *old_left_left = old_left->left;
      node_t *old_left_left_left = old_left_left->left;
      old_left->left = NULL;
      
      node->left = old_left_left;
      node->left->right = old_left;      
      node->left->left = old_left_left_left;      
    }
  else
    {
      node_t *old_left = node->left;
      node_t *old_left_left = old_left->left;
      node_t *old_left_left_right = old_left_left->right;
      old_left_left->right = NULL;
      
      node->left->left = old_left_left_right;
      node->left->left->left = old_left_left;
      
      left_rotate(0, node);
    }
}


void right_rotate()
{
}


void balance_tree(tree_t *tree)
{
  node_t *root = tree->root;

  node_t *left_node = root->left;
  node_t *right_node = root->right;
  
  int left_depth = node_depth(left_node);
  int right_depth = node_depth(right_node);

  int avl_case; // 0 = LEFT LEFT, 1 = LEFT RIGHT, 2 = RIGHT RIGHT, 3 = RIGHT LEFT
  //no need to check for abs(dif)>1 because balance_tree will only be used when tree_balance == false
  //now we only need to know which subtree to rebalance
  if(left_depth>right_depth)    
    {
      // LEFT LEFT CASE
      if(node_depth(left_node->left)>node_depth(left_node->right))
        {
          avl_case = 0;
          left_rotate(avl_case, root);
        }
      // LEFT RIGHT CASE
      else
        {
          avl_case = 1;
          left_rotate(avl_case, root);
        }     
    }
  else
    {
      // RIGHT LEFT CASE
      if(node_depth(right_node->left)>node_depth(right_node->right))
        {
          avl_case = 2;
          right_rotate(avl_case, root);
        }
      // RIGHT RIGHT CASE
      else
        {
          avl_case = 3;
          right_rotate(avl_case, root);
        }
    }
}



/// Insert element into the tree. Returns false if the key is already used.
///
/// Uses the tree's compare function to compare keys.
///
/// If tree's copy function is non-NULL, it will be applied to
/// elem and its result stored in the tree. Otherwise, elem will
/// be stored in the tree. Note that keys are never copied and are
/// assumed to be immutable. (They may however be freed by the
/// tree.)
///
/// \param tree pointer to the tree
/// \param key the key of element to be appended -- this is assumed to be an immutable value
/// \param elem the element 
/// \returns: true if successful, else false
bool tree_insert(tree_t *tree, tree_key_t key, elem_t elem)
{
  element_copy_fun copy_f = tree->copy_elem_f;

  if(tree_has_key(tree, key))
    {
      return false;
    }
  if(copy_f)
    {
      elem_t elem_copy = (elem_t) copy_f elem;  // <- varför fel?
      
      // store value in tree
    }
  
  else
    {
      
      
    }

  while(!tree_balanced(tree)) //rebalance tree
    {
      balance_tree(tree);      
    }
}


//balance tree before or after actions?



/// Finds the element for a given key in tree.
/// 
/// \param tree pointer to the tree
/// \param key the key of elem to be removed
/// \param result a pointer to where result can be stored (only defined when result is true)
/// \returns: true if key is a key in the tree
bool tree_get(tree_t *tree, tree_key_t key, elem_t *result)
{
  return true;
}
/// Removes the element for a given key in tree.
///
/// \param tree pointer to the tree
/// \param key the key of elem to be removed
/// \param result a pointer to where result can be stored (only defined when result is true)
/// \returns: true if key is a key in the tree
bool tree_remove(tree_t *tree, tree_key_t key, elem_t *result)
{
  return true;
}

/*
/// Swap the element for a given key for another.
///
/// \param t pointer to the tree
/// \param k the key of elem to be changed
/// \param e the updated element 
/// \param result a elem_t where result will be stored (only defined when result is true)
/// \returns true if both operations succeeded
#define tree_update(t, k, e, tmp) \
  (tree_remove(t, k, &tmp) ? tree_insert(t, k, e) : false)

/// Returns an array holding all the keys in the tree
/// in ascending order.
///
/// \param tree pointer to the tree
/// \returns: array of tree_size() keys
tree_key_t *tree_keys(tree_t *tree);

/// Returns an array holding all the elements in the tree
/// in ascending order of their keys (which are not part
/// of the value).
///
/// \param tree pointer to the tree
/// \returns: array of tree_size() elements
elem_t *tree_elements(tree_t *tree);

/// This function is used in tree_apply() to allow applying a function
/// to all elements in a tree. 
typedef bool(*key_elem_apply_fun)(tree_key_t key, elem_t elem, void *data);

enum tree_order { inorder = 0, preorder = -1, postorder = 1 };

/// Applies a function to all elements in the tree in a specified order.
/// Example (using shelf as key):
///
///     tree_t *t = tree_new();
///     tree_insert(t, "A25", some_item);
///     int number = 0;
///     tree_apply(t, inorder, print_item, &number);
///
/// where print_item is a function that prints the number and increments it,
/// and prints the item passed to it. 
///
/// \param tree the tree
/// \param order the order in which the elements will be visited
/// \param fun the function to apply to all elements
/// \param data an extra argument passed to each call to fun (may be NULL)
/// \returns the result of all fun calls, combined with OR (||)
bool tree_apply(tree_t *tree, enum tree_order order, key_elem_apply_fun fun, void *data);
*/
