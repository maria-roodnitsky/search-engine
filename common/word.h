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


char **parseLineIntoWords(char* line);
bool checkWordOrder(char **words);
bool isAnd(char *word);
bool isOr(char *word);

#endif