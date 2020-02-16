/* 
 * index.h - header file for CS50 'index' module
 * 
 * This module will contain all shared functions for the tiny search engine. 
 * While it is in progress, it currently contains information for 'indexLoad' which loads an index, and 'indexSave' which writes it
 * to a file in a specific format. It also contains the necessary helper functions for these functions, in addition to a function
 * for creating and a function for deleting an index object. 
 *
 * In progress, February 2020, Maria Roodnitsky
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <memory.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"

/************ data type ********/
typedef struct index {
  struct hashtable *wordHashtable; //hashtable of words
  char *dirName; //name of crawler produced directory holding webpage data 
  char *fileName; //name of file to which indices will be written
} index_t; 


/**************** functions ****************/

/************index_new()************/
/* function takes a directory name [userDirName] which is assumed to be a crawler produced directory. 
 * it also takes a file name [userFileName] that is assumed to be a an existing and writable file to which index data  
 * from the files within the crawler directory will be written. Index data will be written in the following format:
 * [word]: [pageID] [count] ... [pageID] [count] */
index_t *index_new(char *userDirName, char *userFileName);

/**************** index_free() ****************/
/* function takes an index item and frees its contents.  
 * First, it frees the directory name from which the index was sorced, then the file name to which the index data (could be) written.
 * Finally, it frees the hashtable within with the help of helper function, counters delete*/
void index_free(index_t *index);

/****************helper functions ****************/
/*****credit: sourced from the save.c file on the cs50 webpage**/
/* helper function to delete the counter objects within the hashtable within an index object */
void counters_del(void *item); //deletes counter objects

/**************** index_save() ****************/
/* takes an index object and writes its contents to the file within the object. Iterates through each counter object in 
 * the hashtable through the help of helper functions*/
void index_save(index_t *index);

/****************helper functions ****************/
/*****credit: sourced from the save.c file on the cs50 webpage**/
void save_item (void *arg, const char *key, void *item); //helper function to save the words (print the word)
void save_counts (void *arg, const int key, int count); //helper function to save the associated counters to each word


/* function takes a file name [oldIndexFilename] that is assumed to be a an existing and readable file from which
 * index data is read. Data is presumed to be in the following format:
 * [word]: [pageID] [count] ... [pageID] [count] 
 * It then back-constructs an index structure, which stores within it the new file to which the data will be written [newIndexFilename]
 * and returns the index_t object. */ 
index_t *index_load(char *oldIndexFilename, char *newIndexFilename);
#endif

