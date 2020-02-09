#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "../libcs50/memory.h"
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "pagedir.h"

#define MAX_URL_LENGTH 300
#define DEPTH_MAXIMUM 10

/****************local type****************/
typedef struct crawler {
  struct bag *webpageBag; //bag of uncrawled web pages
  struct hashtable *webpageHashtable; //hashtable of all seen URLS
  int maxDepth; //maximum depth to which to crawl
  int pageID; //number of page visited (for file writing)
  char *dirName; //name of directory to which to write webpage data 
} crawler_t; 

/**************** local functions ****************/
static crawler_t *crawler_new(int userMaxDepth, char *userDirName);
void itemdelete(void *item);
void processURLs(webpage_t *currPage, crawler_t *crawler);

int main(const int argc, const char *argv[]){
   
  //check to make sure the command line has been given 4 arguments 
  if (argc != 4){
    fprintf(stderr, "error: command line requires 4 arguments in the follow format:[./crawler] [seedURL] [pageDirectory] [maxDepth]\n");
    return 1; //exit 1 if not given 4 arguments
  }
  
  //create a string to hold the name of pageDirectory
  char *dirName = (char *)count_malloc_assert((strlen(argv[2]) + 10)*sizeof(char), "Not enough memory to check existence of pageDirectory.\n");
  strcpy(dirName, argv[2]);
  
  //create a string to hold the name of the file to be used to check the existence of pageDirectory
  char *tempFile = (char *)count_malloc_assert((strlen(argv[2]) + 10)*sizeof(char), "Not enough memory to make temporary file name.\n");
  sprintf (tempFile, "%s/.crawler", dirName);
  
  FILE *ifile;
  //check the existence of the given directory by opening a '.crawler' file within it
  if((ifile = fopen(tempFile, "w")) == NULL){
    fprintf(stderr, "The directory '%s' does not exist.\n", dirName);
    count_free(dirName); //free the directory name
    count_free(tempFile); //and temporary file name 
    return 2; //exit 2 if not given a valid directory
  }
  fclose(ifile); //if the '.crawler' is written successfully, close the file 
  count_free(tempFile); //and free the temporary file name
  
  if (!isdigit(argv[3][0])) { //check for numeric input as a max depth
    fprintf(stderr, "Your maxDepth value '%s' is non-numeric.\n", argv[3]);
    count_free(dirName); //free the directory
    return 5;//exit 5 if non-numeric depth
  }
  
  int maxDepth = atoi(argv[3]); //convert the argument into an integer and store it in 'maxDepth'
  
  //check to make the depth is in bounds
  if (maxDepth < 0 || maxDepth > DEPTH_MAXIMUM){
    fprintf(stderr, "Invalid depth. Depth must be in the range [0,%d].\n", DEPTH_MAXIMUM);
    count_free(dirName);
    return 3; //exit 3 if it is not in bounds
  }
  
  //store the url given by the user as 'startingURL'
  char *startingURL = (char *)count_malloc_assert((strlen(argv[1]) + 1)*sizeof(char), "Not enough memory to check starting URL validity.\n");
  strcpy(startingURL, argv[1]);
  
  #ifdef MEMTEST
  printf("%s has been taken as the starting URL\n\n", startingURL);
  #endif
  
  if (!IsInternalURL(startingURL)){ //if the URL is not internal, exit and free the directory and URL
    fprintf(stderr, "The given URL is invalid.\n");
    count_free(dirName);
    count_free(startingURL);
    return 4; //exit 4 if given a bad URL
  }
  //if all is well, initialize the crawler and starting page
  char *empty = malloc(3); //create an empty string to be stored with the URL
  strcpy(empty, "");
    
  crawler_t *crawler = crawler_new(maxDepth, dirName); //initialize the crawler   
  webpage_t *startingPage = webpage_new(startingURL, 0, NULL); //initialize the starting page
 
  bag_insert(crawler->webpageBag, startingPage); //insert the starting page into the crawler bag
  hashtable_insert(crawler->webpageHashtable, startingURL, empty); //insert the URL into the hash table   
    
  char *fileName;  
  webpage_t *currPage = bag_extract(crawler->webpageBag);
          
  do {   
    webpage_fetch(currPage); //fetch the webpage HTML and store it in the current page
    
    if (webpage_getHTML(currPage) == NULL) { 
      fprintf(stderr, "%s does not contain any HTML. Skipping.\n", webpage_getURL(currPage));
      webpage_delete(currPage); //delete the current page (count_free its contents)
      currPage = bag_extract(crawler->webpageBag); //extract a new page for processing
      continue;
    }
    
    #ifdef MEMTEST
    printf("%s has been fetched\n\n", webpage_getURL(currPage));
    #endif
        
    fileName = (char *)count_malloc_assert(MAX_URL_LENGTH + 5, "Not enough memory to store file name.\n");
    sprintf (fileName, "%s/%d", dirName, crawler->pageID);

    if (pageSaver(fileName, currPage) != 0) { return 6;} //create the file, exit 6 if failure in doing so 
    crawler->pageID += 1; //increment the page number within the crawler
      
    count_free(fileName); //count_free the filename
           
    if (webpage_getDepth(currPage) < maxDepth){ //if not too deep, process the URLs
      processURLs(currPage, crawler);
    }
    
    webpage_delete(currPage); //delete the current page (count_free its contents)
    currPage = bag_extract(crawler->webpageBag); //extract a new page for processing
    } while (currPage != NULL);
  
  
  hashtable_delete(crawler->webpageHashtable, itemdelete); //count_free the hashtable within the crawler
  bag_delete(crawler->webpageBag, NULL); //count_free the bag within the crawler
  count_free(crawler); //count_free the crawler
  count_free(dirName); //count_free the directory
  return 0;
}


/**************** crawler_new() ****************/
static crawler_t *crawler_new(int userMaxDepth, char *userDirName)
{
  //allocate space for crawler
  crawler_t *crawler = count_malloc_assert(sizeof(crawler_t), "Error in allocating space for crawler.");
  
  //initialize crawler
  crawler->webpageBag = bag_new(); 
  crawler->webpageHashtable = hashtable_new(30);
  crawler->maxDepth = userMaxDepth;
  crawler->pageID = 1;
  crawler->dirName = userDirName;
  
  return crawler;
}

/**************** processURLs() ****************/
void processURLs(webpage_t *currPage, crawler_t *crawler){
  int pos = 0;
  char *tempURL;
  char *empty;
         
  while((tempURL = webpage_getNextURL(currPage, &pos)) != NULL) { //scan URLs until there are none left
    if (IsInternalURL(tempURL)){ //check to make sure the URL is internal
      if (hashtable_find(crawler->webpageHashtable, tempURL) == NULL){ //check to make sure the URL has not yet been seen
        empty = malloc(3); //allocate memory for the placeholder string
        strcpy(empty, "");
        hashtable_insert(crawler->webpageHashtable, tempURL, empty); //insert the URL into the hash table in the crawler
        webpage_t *newPage = webpage_new(tempURL, webpage_getDepth(currPage) + 1, NULL);
        bag_insert(crawler->webpageBag, newPage); //insert the URL into the crawler bag
        
          #ifdef MEMTEST
          printf("%s has been found and stored from the page: %s\n\n", tempURL, webpage_getURL(currPage));
          #endif
        
      }
      else { count_free(tempURL); } //if the URL has already been seen, count_free it
    }
    else { count_free(tempURL); } //if the URL is not internal, count_free it 
    }  
}


// delete an item (used in this context for freeing empty strings)
void itemdelete(void *item)
{
  if (item != NULL) {
    count_free(item);   
  }
}




