/* 
 * index.c - CS50 'index' module
 * 
 * For more information, see 'index.h'
 *
 * In progress, February 2020, Maria Roodnitsky
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <memory.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "index.h"

/**************** main function ****************/
/*****credit: sourced from the save.c file on the cs50 webpage**/
/**************** index_save() ****************/
void index_save(index_t *index){
  //open the user-given file 
  FILE *ifile = fopen(index->fileName, "w");
  
  //iterate through the hashtable to save each element
  hashtable_iterate(index->wordHashtable, ifile, save_item);
  
  fclose(ifile); 
}

/******** help function for save ********/
/*****credit: sourced from the save.c file on the cs50 webpage**/
/******** helper to save item in hashtable ********/
void save_item (void *arg, const char *key, void *item) //save each word in the hashtable
{
	FILE *file = arg; 
	counters_t *counter = item;

	fprintf(file, "%s:", key);
	counters_iterate(counter, file, save_counts);
	fprintf(file, "\n");
}

/******** helper to save items in counters ********/
/*****credit: sourced from the save.c file on the cs50 webpage**/
void save_counts (void *arg, const int key, int count) //save each counter associated to each word
{
	FILE *file = arg; 
	fprintf(file, " %d %d", key, count);
}


/******** main function********/
/**************** index_new() ****************/
index_t *index_new(char *userDirName, char *userFileName)
{
  //allocate space for index
  index_t *index = count_malloc_assert(sizeof(index_t), "Error in allocating space for index.");
  
  //initialize index
  index->wordHashtable = hashtable_new(30);
  index->dirName = userDirName;
  index->fileName = userFileName;
  
  return index;
}

/******** main function********/
/**************** index_free() ****************/
void index_free(index_t *index){
  count_free(index->dirName); //free the interior of the index, starting with the directory name
  count_free(index->fileName); //then the file name
  hashtable_delete(index->wordHashtable, counters_del); //then the hashtable
  free(index); //then the skeleton of the index itself
}

/********helper function for deleting items********/
/******** delete counters item ********/ //frees the counter components
void counters_del(void *item)
{
	counters_t* counter = item;
	if (counter != NULL) {
		counters_delete(counter);
	}
}

/******** main function********/
index_t *index_load(char *oldIndexFilename, char *newIndexFilename){
  
  index_t *index = index_new(NULL, newIndexFilename); //allocate the index 
 
  //open the old index file for processing
  FILE *ifile = fopen(oldIndexFilename, "r");
  
  char *string; //to hold a line of text from the old file, form: [word]: [pageID] [count] ... [pageID] [count] 
  char *word; //to hold the 'word' in the line of text
  int pageID; //to hold the 'pageID' in the (pageID, count) pair
  int count; //to hold the 'count' in the (pageID, count) pair
  
  counters_t *counter_set = counters_new(); //initialize a new counterset
  
  string = (char *)malloc(100000); //allocate space for the string
  word = (char *)malloc(100); //allocate space for the word
  
  while(fgets(string, 10000000, ifile) != NULL){ //while there are lines left in the file
    
    char* parsedString = strtok(string, " "); //parse the line by a space deliminator
    strcpy(word, parsedString); //copy the first part of the parsed string into the 'word' variable
    parsedString = strtok(NULL, " "); //skip to the next part
    
    while (parsedString != NULL) { //while there still string to parse
        pageID = atoi(parsedString); //convert the next piece into an integer, store it as the pageID 
        parsedString = strtok(NULL, " "); 
        count = atoi(parsedString);  //convert the next piece into an integer, store it in counter
        parsedString = strtok(NULL, " "); 
        counters_set(counter_set, pageID, count); //add this pair into the counterset 
        hashtable_insert(index->wordHashtable, word, counter_set); //put the counterset into the word hashtable 
    } 
    free(parsedString); //free the pointer to the parsed string
    free(word); //free word
    free(string); //free the space alloced to the line string
    
    counter_set = counters_new(); //allocate for a new counter set
    word = (char *)malloc(100); //allocate for new word
    string = (char *)malloc(100000); //allocate for new line string
  }
  //free any residual space allocated in the loop
  free(counter_set); 
  free(word);
  free(string);  
  
  fclose(ifile); //close the file
  free(oldIndexFilename); //free its name 
  
  return index;
}