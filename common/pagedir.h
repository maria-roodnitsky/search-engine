/* 
 * pagedir.h - header file for CS50 'pagedir' module
 * 
 * This module will contain all shared functions for the tiny search engine. 
 * While it is in progress, it currently contains information for 'pageSaver,' which saves crawled pages 
 * to a file in a specific format. 
 *
 * In progress, February 2020, Maria Roodnitsky
 */


#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdbool.h>
#include <webpage.h>

/**************** functions ****************/
/***********pageSaver()*************/
/* Write the current page to a file. 
*  The filename must be provided to the function, and by the time it reaches the function, should be in the format 'dir/pageID'
*  The filename and page are assumed to be not null.
*  Function returns 0 on success, 1 on failed file opening. 
*/
int pageSaver(char *fileName, webpage_t *currPage);

/* Takes a crawler produced file [fileName] with data in the format: (line 1) url (line 2) depth (remaining lines) html
 * if the file is not openable, it is presumed to be the n+1th pageID of n files and returns NULL to signify the end of crawler files
 * with the crawler produced file, the function creates a pointer to a webpage_t item. */
webpage_t *pageLoader(char *fileName);

#endif

