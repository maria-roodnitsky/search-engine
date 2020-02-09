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
/* Write the current page to a file. 
*  The filename must be provided to the function, and by the time it reaches the function, should be in the format 'dir/pageID'
*  The filename and page are assumed to be not null.
*  Function returns 0 on success, 1 on failed file opening. 
*/
int pageSaver(char *fileName, webpage_t *currPage);

#endif