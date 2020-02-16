/* 
 * indextest.c - CS50
 * 
 * Given an index file, this program reconstructs an index_t object and prints it to the second file name.
 * The input and output files should contain the same information.
 *
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


int main(const int argc, const char *argv[]){
  
  //check to make sure the command line has been given 3 arguments 
  if (argc != 3){
    fprintf(stderr, "error: command line requires 3 arguments in the follow format:[./indexer] [oldIndexFilename] [newIndexFilename]\n");
    return 1; //exit 1 if not given 3 arguments
  }
  
  //copy the user-given "old index" file name
  char *oldIndexFilename = (char *)count_malloc_assert((strlen(argv[1]) + 10)*sizeof(char), "Not enough memory to check if oldIndexFilename is readable.\n");
  strcpy(oldIndexFilename, argv[1]);
  
  //check to make sure the "old index" file exists and is valid
  FILE *ifile;
  
  if((ifile = fopen(oldIndexFilename, "r")) == NULL){
    fprintf(stderr, "The file '%s' either does not exist or is not readable.\n", oldIndexFilename);
    free(oldIndexFilename); //if not, free the space
    return 2; //exit 2 for not readable/existing file
  }
  fclose(ifile);
  
  //copy the user-given "new index" file name
  char *newIndexFilename = (char *)count_malloc_assert((strlen(argv[1]) + 10)*sizeof(char), "Not enough memory to check if newIndexFilename is writable.\n");
  strcpy(newIndexFilename, argv[2]);
  
  //check to make sure the "new index" file exists and is valid
  if((ifile = fopen(newIndexFilename, "w")) == NULL){
    fprintf(stderr, "The file '%s' either does not exist or is not writable.\n", newIndexFilename);
    free(newIndexFilename); //if not, free the space for it
    free(oldIndexFilename); //and for the old index name
    return 3; //exit 3 for not writable/existing file
  } 
  fclose(ifile);
  
  //do index processing
  index_t *index = index_load(oldIndexFilename, newIndexFilename); 
  index_save(index); 
  index_free(index); 
  
  return 0;
}

