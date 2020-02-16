/* 
 * word.c - CS50 'word' module
 * 
 * For more information, see 'word.h'
 *
 * In progress, February 2020, Maria Roodnitsky
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "word.h"
#include <ctype.h>

/**************** functions ****************/
void NormalizeWord(char *word){ 
  for(int i = 0; word[i]; i++){
    word[i] = tolower(word[i]);
  }
}
