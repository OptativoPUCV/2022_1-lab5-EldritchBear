#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
  TreeMap* map = (TreeMap *) malloc (sizeof(map));
  map->lower_than = lower_than;
  map->current = NULL;
  map->root = NULL;
  return map;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  TreeNode * newNode = createTreeNode(key, value);
  Pair * p = searchTreeMap(tree, key);
  TreeNode * aux = tree->current;

  if (p != NULL)return;
  if(tree->lower_than(newNode->pair->key, aux->pair->key)){
    tree->current->left = newNode;
    newNode->parent = tree->current;
    tree->current = newNode;
  }
  else{ 
    tree->current->right = newNode;
    newNode->parent = tree->current;
    tree->current = newNode;
  }
}

TreeNode * minimum(TreeNode * x){
  while(x != NULL){
    if(x->left == NULL)return x;
    x = x->left;
  }
  return NULL;
}


void removeNode(TreeMap * tree, TreeNode* node) {
if((node->left == NULL) && (node->right == NULL)){
  if(node->parent == NULL){
      tree->root = NULL;
    }
    if(node->parent->right == node){
      node->parent->right = NULL;
    }
    else{
      node->parent->left = NULL;
    }
  }

  if(((node->left != NULL) && (node->right != NULL))){ //dos hijos
    TreeNode *aux = node;
    /*if(node->parent == NULL){
      aux = aux->right;
      tree->root = minimum(aux);
    }*/
    
    aux = aux->right;
    aux = minimum(aux);

    node->pair->key = aux->pair->key;
    node->pair->value = aux->pair->value;
    removeNode(tree,aux);
    return;
  }
  
  if(((node->right != NULL) && (node->left == NULL)) || ((node->right == NULL) && (node->left != NULL))){ //un solo hijo
    if(node->parent == NULL){
      if(node->left != NULL){
        tree->root = node->left;
      }else{
        tree->root = node->right;
      }
    }
    if(node->parent->right == node){ //nodo->current a la derecha
        if(node->right != NULL){ //buscar hijo a la derecha
            node->parent->right = node->right;
            node->right->parent = node->parent;
        }
        else{ //sino hijo esta a la izquierda
            node->parent->right = node->left;
            node->left->parent = node->parent;
        }
    }
    else{ // el nodo->current esta a la izquierda
        if(node->left != NULL){ //revisar hijo a la izquierda
            node->parent->left = node->left;
            node->left->parent = node->parent;
        }
        else{ //hijo a la derecha
            node->parent->left = node->right;
            node->right->parent = node->parent;
        }
    }
  }
  free(node);
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {
  TreeNode * aux = tree->root;
  while(aux != NULL){
    tree->current = aux;
    if(tree->lower_than(key, aux->pair->key))
      aux = aux->left;
    else if (tree->lower_than(aux->pair->key, key))
      aux = aux->right;
    else{
      tree->current = aux;
      return aux->pair;
    }
  }
  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
  TreeNode *nodo = tree->root;
  TreeNode *aux = tree->root;

  while(nodo != NULL){
    if(tree->lower_than(nodo->pair->key, key) == 1){
      if(tree->lower_than(aux->pair->key,key) == 1){
        aux = nodo;
      }
      nodo = nodo->right;
    }
    else{
      nodo = nodo->right;
      //if(tree->lower_than(key , nodo->pair->key) == 1){
        
      }
    }
  }
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
  TreeNode *aux = tree->root;
  aux = minimum(aux);
    return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
  TreeNode *aux = tree->current;
  if(aux->right != NULL){
    aux = aux->right;
    tree->current = aux;
    return minimum(aux)->pair;
  }

  if(aux->right == NULL){
    while(aux->parent != NULL){
      if(aux->parent == NULL)return NULL;
      if(tree->lower_than(aux->pair->key, aux->parent->pair->key) == 1){
        aux = aux->parent;
        tree->current = aux;
        return aux->pair;
      }
      else{
        aux = aux->parent;
      }
    }
  }
    return NULL;
}
