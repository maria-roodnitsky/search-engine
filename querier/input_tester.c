/* 
 * input_tester.c - CS50
 * 
 * Reads lines from stdin, checks for valid input, then checks for valid word order. 
 * Prints the type for each word found. 
 * 
 * February 2020, Maria Roodnitsky
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "../common/word.h"


int main(void){
  
  char *line; 
  char buf[200]; 
  char **words;

  while ((line = fgets(buf, 200, stdin)) != NULL){ //read a line from stdin
    words = parseLineIntoWords(line); //parse the line into words
    
    if (words == NULL) continue; //if there was invalid input, skip this iteration
    checkWordOrder(words); //check for valid word order; if not, an error message will print
    
    for (int i = 0; words[i] != NULL; i++){
      NormalizeWord(words[i]); //normalize each word
      
      if (isAnd(words[i])){
        printf("and found: %s\n", words[i]); //if and, notify the user that an 'and' was found
      }
      else if (isOr(words[i])) { //if or, notify the user that an 'or' was found
        printf("or found: %s\n", words[i]);
      }
      else { //if just a word, notify the user that a word was found
        printf("word found: %s\n", words[i]);
      }
    }  
  }
  
  return 0;
}