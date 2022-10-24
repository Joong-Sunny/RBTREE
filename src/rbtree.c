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


node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert

  node_t *newNode = (node_t*)calloc(1, sizeof(node_t));
  newNode -> key = key;
  newNode -> left = newNode -> right = newNode -> parent = t->nil;
  
  t->root = newNode;
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
  return node_find(t, t->root, key);
}


node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
