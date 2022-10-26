#include "rbtree.h"
#include <stdlib.h>

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *newNode = (node_t *)calloc(1, sizeof(node_t));
  newNode->color = RBTREE_BLACK;
  p->root = p->nil = newNode;
  return p;
}

void postorder_delete_all_node(rbtree *t, node_t *targetNode)
{
  if (targetNode == t->nil)
    return;

  if (targetNode->left != t->nil)
    postorder_delete_all_node(t, targetNode->left);
  if (targetNode->right != t->nil)
    postorder_delete_all_node(t, targetNode->right);
  free(targetNode);
}

void delete_rbtree(rbtree *t)
{
  postorder_delete_all_node(t, t->root);
  /*deleted all node, delete nil*/
  free(t->nil);
  free(t);
}

void rotate_right(rbtree *t, node_t *targetNode)
{
  node_t *y = targetNode->left;
  targetNode->left = y->right;
  if (y->right != t->nil)
    y->right->parent = targetNode;
  y->parent = targetNode->parent;
  if (targetNode->parent == t->nil)
    t->root = y;
  else if (targetNode == targetNode->parent->right)
    targetNode->parent->right = y;
  else
    targetNode->parent->left = y;

  y->right = targetNode;
  targetNode->parent = y;
}

void rotate_left(rbtree *t, node_t *targetNode)
{
  node_t *y = targetNode->right;
  targetNode->right = y->left;
  if (y->left != t->nil)
    y->left->parent = targetNode;
  y->parent = targetNode->parent;
  if (targetNode->parent == t->nil)
    t->root = y;
  else if (targetNode == targetNode->parent->left)
    targetNode->parent->left = y;
  else
    targetNode->parent->right = y;

  y->left = targetNode;
  targetNode->parent = y;
}

void rbtree_insert_fixup(rbtree *t, node_t *targetNode)
{
  while (targetNode->parent->color == RBTREE_RED)
  {
    if (targetNode->parent == targetNode->parent->parent->left)
    {
      node_t *y = targetNode->parent->parent->right;
      if (y->color == RBTREE_RED)
      {
        targetNode->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        targetNode->parent->parent->color = RBTREE_RED;
        targetNode = targetNode->parent->parent;
      }
      else
      {
        if (targetNode == targetNode->parent->right)
        {
          targetNode = targetNode->parent;
          rotate_left(t, targetNode);
        }
        targetNode->parent->color = RBTREE_BLACK;
        targetNode->parent->parent->color = RBTREE_RED;
        rotate_right(t, targetNode->parent->parent);
      }
    }
    else
    {
      node_t *y = targetNode->parent->parent->left;
      if (y->color == RBTREE_RED)
      {
        targetNode->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        targetNode->parent->parent->color = RBTREE_RED;
        targetNode = targetNode->parent->parent;
      }
      else
      {
        if (targetNode == targetNode->parent->left)
        {
          targetNode = targetNode->parent;
          rotate_right(t, targetNode);
        }
        targetNode->parent->color = RBTREE_BLACK;
        targetNode->parent->parent->color = RBTREE_RED;
        rotate_left(t, targetNode->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  node_t *targetNode = (node_t *)calloc(1, sizeof(node_t));
  targetNode->key = key;
  node_t *parent_of_targetNode = t->nil;
  node_t *x = t->root;
  // search start
  while (x != t->nil)
  {
    parent_of_targetNode = x;
    if (targetNode->key < x->key)
      x = x->left;
    else
      x = x->right;
  }
  targetNode->parent = parent_of_targetNode; 
  /* search done... your daddy is parent_of_targetNode */

  if (parent_of_targetNode == t->nil)
    t->root = targetNode;
  else if (targetNode->key < parent_of_targetNode->key)
    parent_of_targetNode->left = targetNode;
  else
    parent_of_targetNode->right = targetNode;

  targetNode->left = t->nil;
  targetNode->right = t->nil;
  targetNode->color = RBTREE_RED;
  rbtree_insert_fixup(t, targetNode);
  return t->root;
}

node_t *node_find(const rbtree *t, node_t *root, const key_t key)
{
  if (root == t->nil)
    return NULL;
  if (root->key == key)
    return root;
  else if (root->key > key)
    return node_find(t, root->left, key);
  else
    return node_find(t, root->right, key);
  return t->root; /*not use*/ 
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  node_t *x = t->root;
  while (x != t->nil)
  {
    if (key == x->key)
      return x;
    else if (key < x->key)
      x = x->left;
    else
      x = x->right;
  }
  return NULL;
}

node_t *node_min(const rbtree *t, node_t *root)
{
  while (root->left != t->nil)
    root = root->left;
  return root;
}

node_t *rbtree_min(const rbtree *t)
{
  node_t *min = t->root;
  while (min->left != t->nil)
    min = min->left;
  return min;
}
node_t *rbtree_max(const rbtree *t)
{
  node_t *max = t->root;
  while (max->right != t->nil)
    max = max->right;
  return max;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v)
{
  if (u->parent == t->nil)
    t->root = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;
  v->parent = u->parent;
}
void rbtree_erase_fixup(rbtree *t, node_t *targetNode)
{
  while ((targetNode != t->root) && (targetNode->color == RBTREE_BLACK))
  {

    if (targetNode == targetNode->parent->left)
    {
      node_t *w = targetNode->parent->right;
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        targetNode->parent->color = RBTREE_RED;
        rotate_left(t, targetNode->parent);
        w = targetNode->parent->right;
      }
      if ((w->left->color == RBTREE_BLACK)&&(w->right->color == RBTREE_BLACK)){
        w->color = RBTREE_RED;
        targetNode = targetNode->parent;
      }
      else{
        if (w->right->color == RBTREE_BLACK)
        {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          rotate_right(t, w);
          w = targetNode->parent->right;
        }
        w->color = targetNode->parent->color;
        targetNode->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        rotate_left(t, targetNode->parent);
        targetNode = t->root;
      }
    }
    else
    {
      node_t *w = targetNode->parent->left;
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        targetNode->parent->color = RBTREE_RED;
        rotate_right(t, targetNode->parent);
        w = targetNode->parent->left;
      }
      if ((w->right->color == RBTREE_BLACK) && (w->left->color == RBTREE_BLACK))
      {
        w->color = RBTREE_RED;
        targetNode = targetNode->parent;
      }
      else
      {
        if (w->left->color == RBTREE_BLACK)
        {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          rotate_left(t, w);
          w = targetNode->parent->left;
        }
        w->color = targetNode->parent->color;
        targetNode->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        rotate_right(t, targetNode->parent);
        targetNode = t->root;
      }
    }
  }
  targetNode->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p)
{
  /* This function erase specific nodes*/
  node_t *y = p;
  color_t p_original_color;
  p_original_color = y->color;
  node_t *x;

  if (p->left == t->nil)
  {
    x = p->right;
    rbtree_transplant(t, p, p->right);
  }
  else if (p->right == t->nil)
  {
    x = p->left;
    rbtree_transplant(t, p, p->left);
  }
  else
  {
    node_t *y = node_min(t, p->right);
    p_original_color = y->color;
    x = y->right;
    if (y->parent == p)
      x->parent = y;
    else
    {
      rbtree_transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    rbtree_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }

  if (p_original_color == RBTREE_BLACK)
    rbtree_erase_fixup(t, x);
  free(p);
  return 0;
}

void rbtree_inorder(const rbtree *t, node_t *root, int *cnt, key_t *arr, int n)
{
  if (root->left != t->nil)
    rbtree_inorder(t, root->left, cnt, arr, n);
  if (*cnt < n)
  {
    arr[*cnt] = root->key;
    (*cnt)++;
  }
  else
    return;

  if (root->right != t->nil)
    rbtree_inorder(t, root->right, cnt, arr, n);

}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  int counter = 0;
  int *ptr_counter = &counter;
  rbtree_inorder(t, t->root, ptr_counter, arr, n);

  return 0;
}