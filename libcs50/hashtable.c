/* 
 * hashtable.c - CS50 'sets' module
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
#include "hashtable.h"
#include "jhash.h"
#include "memory.h"
#include "../set/set.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/

typedef struct hashtable {
  struct set **slot_array; //stores a pointer to an array of sets
  int slot_n;          //number of array slots
} hashtable_t;


/**************** hashtable_new ****************/
hashtable_t *hashtable_new(const int slot_n){
  hashtable_t *hashtable = count_malloc(sizeof(hashtable_t)); 
    
  if (hashtable == NULL) { // an error occurred in allocating space for the hashtable
      return NULL;
  } else { //if allocation successful
      hashtable->slot_array = count_calloc(slot_n, sizeof(set_t*)); //allocate slot_array of sets
        
      if (hashtable->slot_array == NULL) { //error allocating space for array with n slots
        return NULL;
      }
      else {
        hashtable->slot_n = slot_n; //store the size
        return hashtable; //return the new hash table
      } 
  }
}

/**************** hashtable_insert ****************/
bool hashtable_insert(hashtable_t *hashtable, const char *key, void *item) {
//if arguments are valid
  if (hashtable != NULL && key != NULL && item != NULL) { //exit if arguments are not valid
    int hashkey = JenkinsHash(key, hashtable->slot_n); //get hashkey
    if (hashtable->slot_array[hashkey] == NULL) { //check to make sure nothing has already been hashed there
      hashtable->slot_array[hashkey] = set_new(); //create new set at slot
    }
    if (hashtable->slot_array[hashkey] != NULL) { //if there is a set at slot (no allocation errors)
      if (set_find(hashtable->slot_array[hashkey], key) == NULL) { //but the hashkey is not yet there (no duplicate keys)
        set_insert(hashtable->slot_array[hashkey], key, item); //insert the item
        return true;
      }
    }
  }
    return false;
}

/**************** hashtable_find ****************/
void *hashtable_find(hashtable_t *hashtable, const char *key) {
  if (hashtable != NULL && key != NULL) { //if inputs are valid
    int hashkey = JenkinsHash(key, hashtable->slot_n); //use Jenkins 
    if (hashtable->slot_array[hashkey] == NULL) { //if there is no existing set in slot
      return NULL; //going into the set is useless, exit before undefined behavior
    } else { //if there is a set at slot
      void *item = set_find(hashtable->slot_array[hashkey], key); //search for the key
      return item; //return the item
        }
    }
  return NULL; //otherwise, return null
}

/**************** hashtable_print ****************/
void hashtable_print(hashtable_t *hashtable, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item)) { 
  if (fp != NULL) { //make sure the file exists
    if (hashtable != NULL) { //make sure hashtable is not NULL
      for (int i = 0; i < hashtable->slot_n; i++) {
        // print the set at this slot
        fprintf(fp, "%d: ", i);
        if (itemprint != NULL) {  // if the item print function is valid, use it
          if (hashtable->slot_array[i] != NULL) { //if there is a set at index
            set_print(hashtable->slot_array[i], fp, itemprint); //print the entire set
          }
        }
        else fprintf(fp, "\n"); 
      }
    } else {
      fputs("(null)\n", fp); //print a null representation of the hashtable
    }
  }                    
}

/**************** hashtable_iterate****************/
void hashtable_iterate(hashtable_t *hashtable, void *arg, void (*itemfunc)(void *arg, const char *key, void *item) ) {
  if (hashtable != NULL && itemfunc != NULL) { //if a hashtable exists and there is a function to process items
    for (int i = 0; i < hashtable->slot_n; i++) { //check each slot
      if (hashtable->slot_array[i] != NULL) { //if there is a set at index
        set_iterate(hashtable->slot_array[i], arg, itemfunc); //iterate through set
      }
    }
  }
}

/**************** hashtable_delete ****************/
void hashtable_delete(hashtable_t *hashtable, void (*itemdelete)(void *item) ) {
  if (hashtable != NULL) { //if a hashtable exists
    for (int i = 0; i < hashtable->slot_n; i++) { //check each slot
    // remember what comes next
      if (itemdelete != NULL) {	//if there is a valid deleting function
        if (hashtable->slot_array[i] != NULL) { //and if there is a set at index
          set_delete(hashtable->slot_array[i], itemdelete); //delete set at this index
        }
      }
    }
  count_free(hashtable->slot_array); //free the array pointer
  count_free(hashtable); //free pointer to the hashtable
  }
#ifdef MEMTEST
  count_report(stdout, "End of hashtable_delete");
#endif
}