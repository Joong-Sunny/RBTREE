#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *newNode = (node_t*)calloc(1, sizeof(node_t)); 
  newNode->color = RBTREE_BLACK;
  p -> root = p->nil = newNode;
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

void rotate_right(rbtree *t, node_t *targetNode) {
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
  
  y -> right = targetNode;
  targetNode->parent = y;

}
void rotate_left(rbtree *t, node_t *targetNode) {
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
  
  y -> left = targetNode;
  targetNode->parent = y;
}

void rbtree_insert_fixup(rbtree *t, node_t *targetNode){

while (targetNode->parent->color == RBTREE_RED){


  if(targetNode->parent == targetNode->parent->parent->left){

    node_t *y = targetNode->parent->parent->right;
    if (y->color == RBTREE_RED)
    {
      targetNode->parent->color = RBTREE_BLACK;
      y->color = RBTREE_BLACK;
      targetNode->parent->parent->color = RBTREE_RED;
      targetNode = targetNode->parent->parent;
    }
    else{
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
  else{
     // elif(targetNode->parent == targetNode->parent->parent->right)
    node_t *y = targetNode->parent->parent->left;
    if (y->color == RBTREE_RED)
    {
      targetNode->parent->color = RBTREE_BLACK;
      y->color = RBTREE_BLACK;
      targetNode->parent->parent->color = RBTREE_RED;
      targetNode = targetNode->parent->parent;
    }
    else{
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

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *targetNode = (node_t*)calloc(1, sizeof(node_t)); 
  targetNode->key = key;
  node_t *y = t->nil;
  node_t *x = t->root;
  //search start
  while (x != t->nil)
  {
    y = x;
    if (targetNode->key < x->key)
      x = x->left;
    else
      x = x->right;
  }
  targetNode->parent = y; //finde done... your daddy is y
  
  if (y == t->nil)
    t->root = targetNode;
  else if (targetNode->key < y->key)
    y->left = targetNode;
  else
    y->right = targetNode;
  
  targetNode->left = t->nil;
  targetNode->right = t->nil;
  targetNode->color = RBTREE_RED;
  rbtree_insert_fixup(t, targetNode);
  return t->root;
}

node_t *node_find(const rbtree *t, node_t *root, const key_t key){ //중선 새로 만든 함수

  if(root == t->nil)
      return NULL;

  if(root->key == key) //값이 같을때
      return root;
  else if(root->key > key){  //찾으려는 값이 루트보다 작을 때
      return node_find(t, root->left ,key);
  }
  else{                          //찾으려는 값이 루트보다 클 때
      return node_find(t, root->right ,key);
  }
  return t->root; //코치님이 넣어둔 init코드(사용하지 않음)
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  //원래는 왼쪽 오른쪽?? 보면서 가야됨
  //하지만 이번 테스트의 경우에는, 하나밖에 없으니까 그냥 고고

/////////////////
  node_t *x = t->root;

  //search start
  while (x != t->nil)
  {
    // y = x;
    if (key ==  x->key)
      return x; 
    else if (key < x->key)
      x = x->left;
    else
      x = x->right;
  }
  return NULL;
///////////////////

  // return node_find(t, t->root, key);
}


node_t *node_min(const rbtree *t, node_t *root){
  while(root->left != t->nil)
      root = root->left;
  return root;
}
node_t *rbtree_min(const rbtree *t) {
  node_t *tmp = t->root;
  while(tmp->left != t->nil)
      tmp = tmp->left;
  return tmp;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *tmp = t->root;
  while(tmp->right != t->nil)
      tmp = tmp->right;
  return tmp;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v){
  if (u->parent == t->nil)
    t->root = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;
  v->parent = u->parent;
}
void rbtree_erase_fixup(rbtree *t, node_t *targetNode){
  while ((targetNode != t->root) && (targetNode->color == RBTREE_BLACK)){

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
      if ((w->left->color == RBTREE_BLACK)&&(w->right->color == RBTREE_BLACK))
      {
        w->color = RBTREE_RED;
        targetNode = targetNode->parent;
      }
      else{
        if (w->right->color == RBTREE_BLACK)
        {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          rotate_right(t,w);
          w = targetNode->parent->right;
        }
        w->color = targetNode->parent->color;
        targetNode->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        rotate_left(t,targetNode->parent);
        targetNode = t->root;
      }
      /* code */
    }




    else{
      node_t *w = targetNode->parent->left;
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        targetNode->parent->color = RBTREE_RED;
        rotate_right(t, targetNode->parent);
        w = targetNode->parent->left;
      }
      if ((w->right->color == RBTREE_BLACK)&&(w->left->color == RBTREE_BLACK))
      {
        w->color = RBTREE_RED;
        targetNode = targetNode->parent;
      }
      else{
        if (w->left->color == RBTREE_BLACK)
        {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          rotate_left(t,w);
          w = targetNode->parent->left;
        }
        w->color = targetNode->parent->color;
        targetNode->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        rotate_right(t,targetNode->parent);
        targetNode = t->root;
    }
  }
}
targetNode->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *y = p;
  // node_t *p_original_color;
  color_t p_original_color;
  p_original_color = y->color;
  node_t *x;
  
  if (p->left == t->nil){
    x = p->right;
    rbtree_transplant(t, p, p->right);
  }
  else if (p->right == t->nil){
    x = p->left;
    rbtree_transplant(t, p, p->left);
  }
  else{
    node_t *y = node_min(t, p->right);
    p_original_color = y->color;
    x = y->right;
    if (y->parent == p)
      x->parent = y;
    else {
      rbtree_transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    rbtree_transplant(t, p, y);
    y->left = p ->left;
    y->left->parent = y;
    y->color = p->color;
  }

  if (p_original_color == RBTREE_BLACK){
    rbtree_erase_fixup(t, x);
  }

  // t->root = t->nil;
  return 0;
}

void rbtree_inorder(const rbtree *t, node_t *root, int *cnt, key_t *arr, int n){
  
  if (root->left != t->nil){
    rbtree_inorder(t, root->left, cnt, arr, n);
  }
  
  //출력 arr <- root
  if (*cnt < n){
    arr[*cnt] = root->key;
    (*cnt)++;
    }
  else
    return;

  if (root->right != t->nil){
    rbtree_inorder(t, root->right, cnt, arr, n);
  }
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  int counter = 0;
  int *ptr_counter = &counter;
  rbtree_inorder(t, t->root, ptr_counter, arr, n);

  return 0;
}
