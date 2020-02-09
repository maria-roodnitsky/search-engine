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
