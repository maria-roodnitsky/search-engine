/* 
 * pagedir.c - CS50 'pagedir' module
 * 
 * For more information, see 'pagedir.h'
 *
 * In progress, February 2020, Maria Roodnitsky
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <memory.h>
#include "../libcs50/webpage.h"
#include "pagedir.h"

/**************** functions ****************/
int pageSaver(char *fileName, webpage_t *currPage){
  
FILE *ifile = fopen(fileName, "w");
if (ifile == NULL) { return 1;}

fprintf(ifile, "%s\n%d\n%s", webpage_getURL(currPage), webpage_getDepth(currPage), webpage_getHTML(currPage));
fclose(ifile);

return 0;
}

webpage_t *pageLoader(char *fileName){
  FILE *ifile = fopen(fileName, "r");
  
  if (ifile == NULL) {return NULL;}
  
  char *buf; //will hold a buffer as the HTML is parsed out
  
  char *url; //will hold page urls
  int depth; //will hold page depths
  char *html; //will hold webpage html
  
  webpage_t *page; //will hold each file in webpage form for easier parsing
  
  url = (char *)malloc(300); //allocate for the url
  buf = (char *)malloc(10000); //and for the HTML buffer
  html = (char *)malloc(1000000); //and for the HTML itself
  strcpy(html, ""); //initialize the HTML to an empty string (to be added to)
  
  fscanf(ifile, "%s", url); //scan the first line into the URL 
  fscanf(ifile, "%d", &depth); //scan the second line into the depth
  
  fgets(buf, 10000000, ifile); //read in the first line
  do {
    strcat(html, buf); //add it to the overall html
    free(buf); //free the buffer
    buf = (char *)malloc(1000000); //create space for a new one
  } while (fgets(buf, 10000000, ifile) != NULL); //do this until the file reader returns NULL
  
  free(buf); //free what is left of the buffer
  
  page = webpage_new(url, depth, html); //create a page with this URL, depth, and HTML
  fclose(ifile); //close the file from which the page was opened
  
  return page;
  }
  
