/* 
 * word.h - header file for CS50 'word' module
 * 
 * This module will contain all shared functions for the tiny search engine. 
 * While it is in progress, it currently contains information for 'NormalizeWord,' which converts all 
 * letters in a word to lowercase.
 *
 * In progress, February 2020, Maria Roodnitsky
 */


#ifndef __WORDDIR_H
#define __WORDDIR_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/**************** functions ****************/
/* Converts a word to all lowercase. 
*  The word is assumed not to be null
*  Only letters are promised to be converted.
*/
void NormalizeWord(char *word);

/* Takes a string and parses it into words with spaces as deliminators. If there are any non-space or 
 * non-alphabetic characters, returns NULL. Otherwise, returns an array of pointers to parsed words.
 */
char **parseLineIntoWords(char* line);

/* Takes an array of words and confirms that the word order is correct. 
*  If the line begins or ends with 'or' or 'and', or the two words are adjacent in any way, returns false. 
*  If the word order is correct, returns true. 
*/
bool checkWordOrder(char **words);

/* Takes a word and checks if it is 'and'; if 'and', return true; if not 'and', return false.*/ 
bool isAnd(char *word);

/* Takes a word and checks if it is 'or'; if 'or', return true; if not 'or', return false.*/ 
bool isOr(char *word);

#endif