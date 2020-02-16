/* 
 * indexer.c - CS50
 * 
 * Given a crawler directory, this program produces an index_t object and saves it to a file in the specified format.
 * February 2020, Maria Roodnitsky
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "../libcs50/memory.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../common/word.h"
#include "../libcs50/counters.h"
#include "../common/index.h"
#include "../common/pagedir.h"

/**************** local functions ****************/
/* takes an index_t object and using the crawler produced directory inside of it, fills it */
void index_build(index_t *index);

int main(const int argc, const char *argv[]){
  
  //check to make sure the command line has been given 3 arguments 
  if (argc != 3){
    fprintf(stderr, "error: command line requires 3 arguments in the follow format:[./indexer] [pageDirectory] [indexFilename]\n");
    return 1; //exit 1 if not given 3 arguments
  }
  
  //create a string to hold the name of pageDirectory
  char *dirName = (char *)count_malloc_assert((strlen(argv[1]) + 10)*sizeof(char), "Not enough memory to check existence of pageDirectory.\n");
  strcpy(dirName, argv[1]);
  
  //create a string to hold the name of the file to be used to check the existence of pageDirectory/.crawler
  char *tempFile = (char *)count_malloc_assert((strlen(argv[1]) + 10)*sizeof(char), "Not enough memory to make temporary file name.\n");
  sprintf (tempFile, "%s/.crawler", dirName);
  
  FILE *ifile;
  //check the existence of the '.crawler' file to show that this is a crawler produced directory. 
  if((ifile = fopen(tempFile, "r")) == NULL){
    fprintf(stderr, "The directory '%s' either does not exist or is not a crawler produced directory.\n", dirName);
    count_free(dirName); //free the directory name
    count_free(tempFile); //and temporary file name 
    return 2; //exit 2 if not given a valid crawler produced directory 
  }
  fclose(ifile); //if the '.crawler' file is found, close the file 
  count_free(tempFile); //and free the temporary file crawler file name 
  
  //create a string to hold the name of indexFilename
  char *indexFilename = (char *)count_malloc_assert((strlen(argv[2]) + 3)*sizeof(char), "Not enough memory to check existence of indexFilename.\n");
  strcpy(indexFilename, argv[2]);
  
  //check to make sure the indexFile is a writeable and existing file
  if((ifile = fopen(indexFilename, "w")) == NULL){ //if not
    fprintf(stderr, "The file '%s' either does not exist or is not writable.\n", indexFilename);
    count_free(dirName); //free the directory name
    count_free(indexFilename); //and the index file name
    return 3; //exit 3 if not given a writable or existing index file name 
  }
  fclose(ifile);

  index_t *index = index_new(dirName, indexFilename); //initialize the index 
  index_build(index); //build the index
  index_save(index); //save the internal data to the user given file

  index_free(index);
  return 0;
}


/**************** index_buid() ****************/
void index_build(index_t *index){

  int pageID = 1; //initialize pageID to 1
  counters_t *counter_set; //will hold counters objects associated to pageIDs
  
  char *tempFile = (char *)count_malloc_assert((strlen(index->dirName) + 10)*sizeof(char), "Not enough memory to make temporary file name.\n");

  webpage_t *page; //will hold each file in webpage form for easier parsing 

  char *word; //will hold words

  //create the name of the first file
  sprintf (tempFile, "%s/%d", index->dirName, pageID);
  
  while ((page = pageLoader(tempFile)) != NULL){
    int pos = 0;
    
    while ((word = webpage_getNextWord(page, &pos)) != NULL) { //grab words from HTML
      NormalizeWord(word); //normalize the words
      if (strlen(word) < 3) { //if the word is not longer than 3 characters, ignore it 
        free(word);
        continue; 
      }
    
      counter_set = hashtable_find(index->wordHashtable, word); //check to see what counter_set is associated with this word
      
      if (counter_set == NULL){ //if there is none
          counter_set = counters_new(); //create one
          counters_add(counter_set, pageID); //add the page to it
          hashtable_insert(index->wordHashtable, word, counter_set); //put the set into the word hashtable
      }
      else { //if the word already exists in the hashtable
        int count;
        if ((count = counters_get(counter_set, pageID)) != 0){ //and it has already been seen on this page
          count += 1; //increment the number of times it has been seen
          counters_set(counter_set, pageID, count);
        }
        else { //if it has not been seen on this page, add a counter with this page ID as key
          counters_set(counter_set, pageID, 1);
        }
      }        
      free(word); //free the word once processing is complete
 } //repeat while there are still words to be processed in this webpage
  
  webpage_delete(page); //once there are no more words, delete the page
  free(tempFile); //free the string holding this file name
  pageID += 1; //increment the pageID to the next page
  tempFile = (char *)count_malloc_assert((strlen(index->dirName) + 10)*sizeof(char), "Not enough memory to make temporary file name.\n");
  sprintf (tempFile, "%s/%d", index->dirName, pageID); //prepare for opening the following file 
}

free(tempFile); //once all files have been processed, free space for the temporary file name  
}
