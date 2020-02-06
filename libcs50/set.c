/* 
 * sets.c - CS50 'sets' module
 *
 * see sets.h for more information.
 *
 * David Kotz, April 2016, 2017
 * Xia Zhou, July 2017, 2018, January 2019
 * Temi Prioleau, January 2020
 * Implemented by Maria Roodnitsky, January 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "memory.h"

/**************** global types ****************/
typedef struct set{
  struct setnode *head;
} set_t;

typedef struct setnode {
  char *key; // string key value
  void *item;  //pointer to item
  struct setnode *next;	      // link to next node
} setnode_t;

//helper method:
static setnode_t* setnode_new(const char *stringkey, void *item);

/**************** set_new ****************/
set_t *set_new(void){
  set_t *set = count_malloc(sizeof(set_t));
  
  if (set == NULL) {
    return NULL; // an error occurred in allocating space for the the set
  } else {
    // initialize the head of the bag to a null pointer
    set->head = NULL;
    return set;
  }
}

/**************** set_insert ****************/
bool set_insert(set_t *set, const char *key, void *item){
  if (set == NULL || item == NULL || key == NULL){ //if any pointer is null
    return false;     //return false
  } else {
      for (setnode_t *node = set->head; node != NULL; node = node->next) {
        if (strcmp(node->key,key) == 0) { //search through the linked list for the key
          return false; //do nothing if key found
        }
      } 
      setnode_t *newsetnode = setnode_new(key, item);//if the key does not exist, make a new counter
      
      if (newsetnode == NULL) {
        return false; // error allocating for new counter
      } else {     
        newsetnode->next = set->head; //tack this counter to the front 
        set->head = newsetnode;
      return true;
    }
  }
}

/**************** set_find ****************/
void *set_find(set_t *set, const char *key)
{
  if (set == NULL || key == NULL){ //if the pointer to the set or the key is null
    return NULL; //return NULL
  } else {
      for (setnode_t *node = set->head; node != NULL; node = node->next) {
        if (strcmp(node->key,key) == 0) { //search through the linked list for the key
          return node->item;   //if found, return the item
        }
      }
     return NULL; //key not found
  }   
}

/**************** set_print ****************/
void set_print(set_t *set, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item) )
{
  if (fp != NULL) {
    if (set != NULL) {
      fputc('{', fp);
      for (setnode_t *node = set->head; node != NULL; node = node->next) {
        // print this node
        if (itemprint != NULL) {  // print the node's item 
          (*itemprint)(fp, node->key, node->item); 
          fputc(',', fp);
        }
      }
      if (set->head != NULL) {
      fputs("\b}\n", fp); //chop off the final comma, close the bracket
      } else {
      fputs("}\n", fp);
      }
    } else {
      fputs("(null)", fp);
    }
  }
}

/**************** set_iterate ****************/
void set_iterate(set_t *set, void *arg, void (*itemfunc)(void *arg, const char *key, void *item) )
{
  if (set != NULL && itemfunc != NULL) {
    // call itemfunc with arg, on each item
    for (setnode_t *node = set->head; node != NULL; node = node->next) {
      (*itemfunc)(arg, node-> key, node->item); 
    }
  }
}

/**************** set_delete() ****************/
void set_delete(set_t *set, void (*itemdelete)(void *item) )
{
  if (set != NULL) {
    for (setnode_t *node = set->head; node != NULL; ) {
      if (itemdelete != NULL) {		    // if possible...
        (*itemdelete)(node->item);	    // delete node's item
      }
      setnode_t *next = node->next;	    // remember what comes next
      count_free(node->key); //free string
      count_free(node);			    // free the node
      node = next;			    // and move on to next
    }

    count_free(set);
  }
}

//helper method
/**************** setnode_new ****************/
static setnode_t* setnode_new(const char *stringkey, void *item)
{
  setnode_t *newnode = count_malloc(sizeof(setnode_t));

  if (newnode == NULL) {
    // error allocating memory for node; return error
    return NULL;
  } else {
    newnode->key = count_malloc(sizeof(char)*(strlen(stringkey) + 1));
    strcpy(newnode->key, stringkey);
    newnode->item = item;
    newnode->next = NULL;
    return newnode;
  }
}