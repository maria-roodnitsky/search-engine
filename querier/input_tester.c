#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "../common/word.h"


int main(void){
  
  char *line; 
  char buf[200]; 
  char **words;

  while ((line = fgets(buf, 200, stdin)) != NULL){
    words = parseLineIntoWords(line);
    
    if (words == NULL) continue;
    checkWordOrder(words);
    
    for (int i = 0; words[i] != NULL; i++){
      NormalizeWord(words[i]);
      
      if (isAnd(words[i])){
        printf("and found: %s\n", words[i]);
      }
      else if (isOr(words[i])) {
        printf("or found: %s\n", words[i]);
      }
      else {
        printf("word found: %s\n", words[i]);
      }
    }  
  }
  
  return 0;
}