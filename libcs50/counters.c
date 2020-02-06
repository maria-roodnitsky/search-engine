/* 
 * counters.c - CS50 'counters' module
 *
 * see counters.h for more information.
 *
 * David Kotz, April 2016, 2017
 * Xia Zhou, July 2017, 2018, January 2019
 * Temi Prioleau, January 2020
 * Implemented by Maria Roodnitsky, January 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counters.h"
#include "memory.h"



/**************** local types ****************/
typedef struct counternode {
  int key; // integer key value
  int count;    //tracks how many times key has been incremented
  struct counternode *next;	      // link to next node
} counternode_t;

/**************** module-local function ****************/
static counternode_t* counternode_new(int key);

/**************** global types ****************/
typedef struct counters {
  struct counternode *head;	      // head of the list of items in counter
} counters_t;

/**************** counters_new() ****************/
counters_t *counters_new(void){
  counters_t *counter = count_malloc(sizeof(counters_t));
  
  if (counter == NULL) {
    return NULL; // an error occured in allocating space for the counters
  } else {
    // initialize the head of the bag to a null pointer
    counter->head = NULL;
    return counter;
  }
}

/**************** counters_add ****************/
int counters_add(counters_t *ctrs, const int key){
  if (ctrs == NULL || key < 0){ //if the pointer to the counterset is null or the key is negative
    return 0;     //return 0
  } else {
      for (counternode_t *node = ctrs->head; node != NULL; node = node->next) {
        if (node->key == key) { //search through the linked list for the key
          node->count += 1;   //if found, increment the count
          return node->count;
        }
      } 
      counternode_t *newcounter = counternode_new(key);//if the key does not exist, make a new counter
      
      if (newcounter == NULL) {
        return 0; // error allocating for new counter
      } else {
        newcounter->count = 1;      
        newcounter->next = ctrs->head; //tack this counter to the front 
        ctrs->head = newcounter;
      return 1;
    }
  }
}

/**************** counters_get ****************/
int counters_get(counters_t *ctrs, const int key){
  if (ctrs == NULL || key < 0){ //if the pointer to the counterset is null or the key is negative
    return 0; //return 0
  } else {
      for (counternode_t *node = ctrs->head; node != NULL; node = node->next) {
        if (node->key == key) { //search through the linked list for the key
          return node->count;   //if found, return the count value
        }
      }
     return 0; //key not found
  }
}

/**************** counters_set ****************/
bool counters_set(counters_t *ctrs, const int key, const int count){
  if (ctrs == NULL || key < 0 || count < 0){ //if the pointer to the counterset is null or the key is negative
     return false; //return false
  } else {
       for (counternode_t *node = ctrs->head; node != NULL; node = node->next){
         if (node->key == key) { //search through the linked list for the key
           node->count = count;   //if found, change the count to the provided count
           return true;
          }
        } 
    counternode_t *newcounter = counternode_new(key); //if the key does not exist, make a new counter
    
    if (newcounter == NULL) {
      return false; // error allocating for new counter
    } else {
      newcounter->count = count;    
      newcounter->next = ctrs->head; //tack this counter to the front 
      ctrs->head = newcounter;
      return true;
    }    
  } 
}

 /**************** counters_print ****************/
void counters_print(counters_t *ctrs, FILE *fp){
  if (fp != NULL) { //do nothing if the file pointer is null
    if (ctrs != NULL){
      fputc('{', fp);
      for (counternode_t *node = ctrs->head; node != NULL; node = node->next){
        fprintf(fp, "(%d=%d),", node->key, node->count);
      }
      if (ctrs->head != NULL) {
      fputs("\b}\n", fp); //chop off the final comma, close the bracket
      } else {
      fputs("}\n", fp);
      }   
    }
    else { //if counters is null, print the required message
      fputs("(null)", fp);
    }
  }
}

 /**************** counters_iterate ****************/
void counters_iterate(counters_t *ctrs, void *arg, void (*itemfunc)(void *arg, const int key, const int count)){
  if (ctrs != NULL && itemfunc != NULL) {
   // call itemfunc with arg, on each item
    for (counternode_t *node = ctrs->head; node != NULL; node = node->next) {
      (*itemfunc)(arg, node->key, node->count); 
    }
  }   
}

/**************** counters_delete ****************/
void counters_delete(counters_t *ctrs){
  if (ctrs != NULL) {
    for (counternode_t *node = ctrs->head; node != NULL;){
      counternode_t *next = node->next; //find the next node
      count_free(node); //free the node
      node = next; //move on to the next node
    }   
    count_free(ctrs); //free the whole counters
  }
  #ifdef MEMTEST
  count_report(stdout, "End of counters delete");
  #endif
}


/**************** module-local function****************/
/**************** counternode_new ****************/
static counternode_t* counternode_new(int key)
{
  counternode_t *newnode = count_malloc(sizeof(counternode_t));

  if (newnode == NULL) {
    // error allocating memory for node; return error
    return NULL;
  } else {
    newnode->key = key;
    newnode->count = 0;
    newnode->next = NULL;
    return newnode;
  }
}

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
