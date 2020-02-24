
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "../libcs50/memory.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../common/word.h"
#include "../libcs50/counters.h"
#include "../common/index.h"
#include "../libcs50/set.h"
#include "../libcs50/bag.h"

/************ data type ********/
typedef struct and_seq {
  struct counters *IDset; //set of IDs
} and_seq_t; 


static void set_merge(counters_t *setA, counters_t *setB);
static void set_merge2(counters_t *setA, counters_t *setB);
static void set_merge3(counters_t *setA, counters_t *setB);
static void set_merge_helper(void *arg, const int key, const int count);
static void set_merge_helper2(void *arg, const int key, const int count);
static void set_merge_helper3(void *arg, const int key, const int count);
static void set_intersect_helper(void *arg, const int key, const int item);
static void set_intersect(counters_t *setA, counters_t *setB);
and_seq_t *create_and_seq(index_t *index, char **words);
and_seq_t *and_seq_new(void);
int min(int num1, int num2); 
void nameprint(FILE *fp, const char *key, void *item);

int main(const int argc, const char *argv[]){
  
  //check to make sure the command line has been given 3 arguments 
  if (argc != 3){
    fprintf(stderr, "error: command line requires 3 arguments in the follow format:[./querier] [pageDirectory] [indexFilename]\n");
    return 1; //exit 1 if not given 3 arguments
  }
  
  //create a string to hold the name of pageDirectory
  char *dirName = (char *)count_malloc_assert((strlen(argv[1]) + 10)*sizeof(char), "Not enough memory to check existence of pageDirectory.\n");
  strcpy(dirName, argv[1]);
  
  //create a string to hold the name of the file to be used to check the existence of pageDirectory/.crawler
  char *tempFile = (char *)count_malloc_assert((strlen(argv[1]) + 10)*sizeof(char), "Not enough memory to make temporary file name.\n");
  sprintf (tempFile, "%s/.crawler", dirName);
  
  FILE *ifile;
  //check the existence of the '.crawler' file to show that this is a crawler produced directory. 
  if((ifile = fopen(tempFile, "r")) == NULL){
    fprintf(stderr, "The directory '%s' either does not exist or is not a crawler produced directory.\n", dirName);
    count_free(dirName); //free the directory name
    count_free(tempFile); //and temporary file name 
    return 2; //exit 2 if not given a valid crawler produced directory 
  }
  fclose(ifile); //if the '.crawler' file is found, close the file 
  count_free(tempFile); //and free the temporary file crawler file name 
  
  //create a string to hold the name of indexFilename
  char *indexFilename = (char *)count_malloc_assert((strlen(argv[2]) + 3)*sizeof(char), "Not enough memory to check existence of indexFilename.\n");
  strcpy(indexFilename, argv[2]);
  
  //check to make sure the indexFile is a writeable and existing file
  if((ifile = fopen(indexFilename, "r")) == NULL){ //if not
    fprintf(stderr, "The file '%s' either does not exist or is not writable.\n", indexFilename);
    count_free(dirName); //free the directory name
    count_free(indexFilename); //and the index file name
    return 3; //exit 3 if not given a writable or existing index file name 
  }
  fclose(ifile);
  
  index_t *index = index_load(indexFilename, NULL); //load the index into a structure
  
  //exit the program if an error occured in opening the index
  if (index == NULL){
    fprintf(stderr, "Error in loading the intex.\n");
    count_free(dirName); //free the directory name
    count_free(indexFilename); //and the index file name
    return 4;
  }
  
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

  and_seq_t *and_seq = create_and_seq(index, words);
  counters_print(and_seq->IDset, stdout);
      
  }

  
  index_free(index);
 
}

and_seq_t *and_seq_new(void)
{
  //allocate space for and_seq
  and_seq_t *and_seq = count_malloc_assert(sizeof(and_seq_t), "Error in allocating space for and sequence.");
  
  //initialize index
  and_seq->IDset = counters_new();
  
  return and_seq;
}

and_seq_t *create_and_seq(index_t *index, char **words){
  and_seq_t *and_seq = and_seq_new();
  bag_t *or_bag = bag_new();
  
  counters_print(hashtable_find(index->wordHashtable, words[0]), stdout);
  set_merge3(and_seq->IDset, hashtable_find(index->wordHashtable, words[0]));
  
  int i = 1;
  while (words[i] != NULL){
    
    if (isOr(words[i - 1])) {
      counters_print(hashtable_find(index->wordHashtable, words[0]), stdout);
      set_merge3(and_seq->IDset, hashtable_find(index->wordHashtable, words[0]));   
    } else if (isOr(words[i])){
      //add to the set of and_seqs, start a new one
      bag_insert(or_bag, and_seq->IDset);
      and_seq = and_seq_new();
      
    } else if (!isAnd(words[i])){     
      printf("made it inside!! \n");
      printf("%s", words[i]);
      counters_t *counters= hashtable_find(index->wordHashtable, words[i]);
      counters_print(counters, stdout);
      set_intersect(and_seq->IDset, hashtable_find(index->wordHashtable, words[i])); 
    }   
      i++;
  }
  
  bag_insert(or_bag, and_seq->IDset);
  counters_t *set;
  
  and_seq_t *query_res = and_seq_new();
  
  
  while ((set = bag_extract(or_bag)) != NULL){
    counters_print(set, stdout);
    set_merge2(query_res->IDset, set);  
  }
  
  counters_print(query_res->IDset, stdout);
  return query_res;
}

/* Merge the second set into the first set;
 * the second set is unchanged.
 */
static void 
set_merge(counters_t *setA, counters_t *setB)
{
  printf("in merge\n");
  counters_iterate(setB, setA, set_merge_helper);
  printf("out of merge\n");
}

static void 
set_merge_helper(void *arg, const int key, const int count)
{
  counters_t *setA = arg;
  printf("in merge helper\n");
  printf("%d is the key of set A", key);
   // find the same key in setA
  int itemA = counters_get(setA, key);
  printf("%d is the item in set A\n", itemA);
  itemA = min(itemA, count);
  printf("min between (count)%d and calculated min %d\n", count, itemA);
  counters_set(setA, key, itemA);
}


static void 
set_merge2(counters_t *setA, counters_t *setB)
{
  counters_iterate(setB, setA, set_merge_helper2);
}

static void 
set_merge_helper2(void *arg, const int key, const int count)
{
  counters_t *setA = arg;
  printf("in merge helper 2\n");
  printf("%d is the key of set A", key);
  
  // find the same key in setA
  int itemA = counters_get(setA, key);
  if (itemA == 0) {
    // not found: insert it
    counters_set(setA, key, count);
    printf("not found: %d is now the count for %d key in set A\n", count, key);
  } else {
    // add to the existing value
    itemA = itemA + count;
    counters_set(setA, key, itemA); 
    printf("%d is now the count for %d key in set A\n", count, key);
  }
}

/* Consider one item for insertion into the other set.
 * If the other set does not contain the item, insert it;
 * otherwise, update the other set's item with sum of item values.
 */
 
static void 
set_merge3(counters_t *setA, counters_t *setB)
{
  counters_iterate(setB, setA, set_merge_helper3);
}

/* Consider one item for insertion into the other set.
 * If the other set does not contain the item, insert it;
 * otherwise, update the other set's item with sum of item values.
 */
 

static void 
set_merge_helper3(void *arg, const int key, const int count)
{
  counters_t *setA = arg;
  printf("in merge helper 3\n");
  printf("%d is now the count for %d key in set A\n", count, key);
  counters_set(setA, key, count);
}

int min(int num1, int num2) 
{
  if (num1 < num2) return num1;
  else return num2;
}

//prints null if the either key or item is null, otherwise, prints the pair both as strings
void nameprint(FILE *fp, const char *key, void *item)
{
  if (key == NULL || item == NULL) {
    fprintf(fp, "(null)");
  }
  else {
    fprintf(fp, "(%s, %s)", key, (char *)item); 
  }
}

/* Take the intersection of two sets and store it in the first set.
 * the second set is unchanged.
 */
/* Take the intersection of two sets and store it in the first set.
 * the second set is unchanged.S
 */
static void 
set_intersect(counters_t *setA, counters_t *setB)
{
  counters_iterate(setB, setA, set_intersect_helper);
}

static void 
set_intersect_helper(void *arg, const int key, const int item)
{
  counters_t *setA = arg; 
  int itemB = item; 

  int itemA = counters_get(setA, key); 
  if (itemA == 0) {
    counters_set(setA, key, itemA);
  } else {
    itemA = itemA < itemB ? itemA : itemB; // found the key, take the minimum
    counters_set(setA, key, itemA);
  }
}