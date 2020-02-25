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
#include <stdbool.h>

/**************** functions ****************/
/**************** NormalizeWord() ****************/
void NormalizeWord(char *word){ 
  for(int i = 0; word[i]; i++){
    word[i] = tolower(word[i]);
  }
}

/**************** paseLineIntoWords() ****************/
/**************** source: inserver-split-solution.c****************/
char **parseLineIntoWords(char* line){
  char **words; //array of pointers to words
  words = count_calloc_assert((strlen(line)/2) + 1, sizeof(char*), "Not enough memory for words.");
  
  int word_n = 0; //number of words
  
  for (char *rest = line; *rest != '\0';){
    char *start;   // address of first letter of word
    // seek the start of a word
    for (start = rest; !isalpha(*start) && *start != '\0'; start++) {
      if (!isalpha(*start) && *start != ' ' && *start != '\n'){ //if a non alphabetic character is present
          printf("Error: bad character '%c' in query.\n", start[0]); //check that it is not a space or \n
          free(words);
          return NULL; //return a NULL pointer to show invalid input
      } else if (*start == '\n') {return NULL;} //a new line will only happen if there are no words
      
    }
    // seek the end of the word
    for (rest = start; isalpha(*rest) && *rest != '\0'; rest++) {
    }

    if (*rest != '\0') { //if the word was finished for a reason other than '\0', it should be a space or '\n'
      if (*rest != ' ' && *rest != '\n'){ //if it is neither, it means the query has bad input
          printf("Error: bad character '%c' in query.\n", rest[0]);
          free(words);
          return NULL;
      }
      *rest++ = '\0'; // if the word was ended fairly, use '\0' to signal the end
    }
    if (*start != '\0') { //if start has a beginning to a valid word, add it to the array of words
      words[word_n++] = start;
    }
  }
  return words; 
}

/**************** isAnd() ****************/
bool isAnd(char *word){
  char and[] = "and";
  if (strlen(word) != 3){return false;} //if the string length is not three, the word cannot be 'and' 
  for (int i = 0; i < strlen(word); i++){ //if any letter does not match, the word cannot be 'and'
    if (word[i] != and[i]) {return false;}
  }  
  return true; //otherwise, the word is 'and'
}
/**************** isOr() ****************/
bool isOr(char *word){
  char or[] = "or";
  if (strlen(word) != 2){return false;} //if the string length is not two, the word cannot be 'or' 
  for (int i = 0; i < strlen(word); i++){ //if any letter does not match, the word cannot be 'or'
    if (word[i] != or[i]) {return false;}
  }  
  return true; //otherwise, the word is 'or'
}

/**************** checkWordOrder() ****************/
bool checkWordOrder(char **words){
  if (isAnd(words[0]) || isOr(words[0])){ //if the first word is 'or' or 'and' return false; print error
    printf("Error: '%s' cannot be first word in a query.\n", words[0]);
    return false; //return false for bad word order
  }
  
  for (int i = 0; words[i+1] != NULL; i++){
    if ((isAnd(words[i]) || isOr(words[i])) && (isAnd(words[i + 1]) || isOr(words[i + 1]))){
      printf("Error: '%s' and '%s' cannot be adjacent in a query.\n", words[i], words[i+1]);
      return false; //if any combination of 'and' and 'or' are adjacent, return false; print error
    }
    
    if (words[i+2] == NULL && (isAnd(words[i+1]) || isOr(words[i+1]))){
      printf("Error: '%s' cannot be last word in a query.\n", words[i+1]);
      return false; //if the final word is 'and' or 'or', return false; print error
    }   
  }
  
  return true; //if no catch has been found, the word order has no errors
}
  


