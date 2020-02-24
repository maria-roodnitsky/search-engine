
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


typedef struct array_index {
  int slot; // holds a copy of all counters in the and sequence
  int *array;      // holds a counter from the and sequence
} array_index_t;


typedef struct counter_holder {
  counters_t *copy;		      // holds a copy of all counters in the and sequence
  counters_t *and_seq_counters;      // holds a counter from the and sequence
} counter_holder_t;

/************ data type ********/
static void set_merge(counters_t *setA, counters_t *setB);
static void set_merge_helper(void *arg, const int key, const int count);

static void set_intersect_helper(void *arg, const int key, const int item);
static void set_intersect(counters_t *setA, counter_holder_t *setB);

counters_t *create_and_seq(index_t *index, char **words);
int min(int num1, int num2); 

void nameprint(FILE *fp, const char *key, void *item);
static void itemcount(void *arg, const int key, const int value);

counters_t *copy_func(counters_t *original);

static void arraystuff(void *arg, const int key, const int value);
static void arraystuff(void *arg, const int key, const int value);
array_index_t *order(counters_t *ctrs);
void print_results(counters_t *final_and_seq, array_index_t *ordered_array, char *dirName);

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
   
  index_t *index = index_load(indexFilename, NULL);  
  
  //exit the program if an error occured in opening the index
  if (index == NULL){
    fprintf(stderr, "Error in loading the intex.\n");
    count_free(dirName); //free the directory name
    count_free(indexFilename); //and the index file name
    return 4;
  }
  
  char *line; //will hold the user input string 
  char buf[200]; //buffer for fgets()
  char **words; //will hold the array of parsed words
  
  printf("Query? \n>");
  while ((line = fgets(buf, 200, stdin)) != NULL){
    words = parseLineIntoWords(line);
    
    if (words == NULL) continue; //if bad input, skip this query
    if (!checkWordOrder(words)) { //if the word order is not permissible "and and" 
      free(words);                //or "and/or" at the beginning or end, skip this iteration
      continue; 
    }
  
    printf("Query: ");
    for (int i = 0; words[i] != NULL; i++){ 
    NormalizeWord(words[i]);
    printf("%s ", words[i]);
    }
    
    printf("\n");

  counters_t *and_seq = create_and_seq(index, words);
  
  array_index_t *ordered = order(and_seq);  
  print_results(and_seq, ordered, dirName);
  free(words);
  counters_del(and_seq);
  }
  free(dirName);
  index_free(index);
}

void print_results(counters_t *final_and_seq, array_index_t *ordered_array, char *dirName){
  
  for (int i = 0; i < ordered_array->slot; i++){
   //if the top result has a score of zero, there must be no matches
   if (counters_get(final_and_seq, ordered_array->array[0]) == 0){
     printf("No results found\n"); //print this to the user
     break;
   }
   else if (counters_get(final_and_seq, ordered_array->array[i]) == 0){ //if the score is zero, 
     continue;
   }
   
   //make a temporary file name with the directory and pageID
   char *tempFile2 = (char *)count_malloc_assert((strlen(dirName) + 10)*sizeof(char), "Not enough memory to make temporary file name.\n");
   
   sprintf (tempFile2, "%s/%d", dirName, ordered_array->array[i]); //print this into the string
   
   FILE *ifile;
   ifile = fopen(tempFile2, "r");
   char *line = malloc(200);
   fgets(line, 200, ifile);
   fclose(ifile);
   
   printf("score: %4d doc: %4d url: %s ", counters_get(final_and_seq, ordered_array->array[i]), ordered_array->array[i], line); 
   free(tempFile2);
   free(line);
   }  
}




counters_t *create_and_seq(index_t *index, char **words){
  counters_t *and_seq = counters_new();
  bag_t *or_bag = bag_new();
  
  int i = 0;
  int start = 0;
  int end = 0;
  
  
  while (words[i] != NULL){
    if (isOr(words[i])){
      counter_holder_t *ch = count_malloc(sizeof(counter_holder_t));
      counters_t *copy = copy_func(and_seq); 
      //counters_t *copy = copy_func(hashtable_find(index->wordHashtable, words[i-1]));
      ch->copy = copy;
      for (int j = start; j < end; j++){
        if(!isAnd(words[j])){
        ch->and_seq_counters = hashtable_find(index->wordHashtable, words[j]);
        printf("intersecting\n"); 
        
        set_intersect(copy, ch);}
      }
      bag_insert(or_bag, and_seq);
      and_seq = counters_new();
      start = i + 1;
    } else if(isAnd(words[i])){
      
      i++; 
      end += 1;
      continue;
    } else {
      set_merge(and_seq, hashtable_find(index->wordHashtable, words[i]));
      counters_print(and_seq, stdout);
    }
    end += 1;
    i++;
  }    
  
  counter_holder_t *ch = count_malloc(sizeof(counter_holder_t));
      printf("inside copy create\n");
      counters_t *copy = copy_func(and_seq); 
      //counters_t *copy = copy_func(hashtable_find(index->wordHashtable, words[0]));
      ch->copy = and_seq;
      
  for (int j = start; j < end; j++){
        if(!isAnd(words[j])){
        ch->and_seq_counters = hashtable_find(index->wordHashtable, words[j]);
        printf("intersecting\n"); 
        
        set_intersect(copy, ch);}
  }
  bag_insert(or_bag, and_seq);
  counters_t *set;  
  counters_t *query_res = counters_new();
  
  
  while ((set = bag_extract(or_bag)) != NULL){
    counters_print(set, stdout);
    set_merge(query_res, set);  
  }
  
  counters_print(query_res, stdout);
  return query_res;
}

counters_t *copy_func(counters_t *original){
  
  counters_t *copy = counters_new();
  set_merge(copy, original);
  
  return copy;
}


/* Merge the second set into the first set;
 * the second set is unchanged.
 */
static void 
set_merge(counters_t *setA, counters_t *setB)
{
  counters_iterate(setB, setA, set_merge_helper);
}

/* Consider one item for insertion into the other set.
 * If the other set does not contain the item, insert it;
 * otherwise, update the other set's item with sum of item values.
 */
static void 
set_merge_helper(void *arg, const int key, const int item)
{
  counters_t *setA = arg;
  int itemB = item;
  
  // find the same key in setA
  int itemA = counters_get(setA, key);
  if (itemA == 0) {
    // not found: insert it
    counters_set(setA, key, itemB);
    //printf("set A did not have an item for %d, so it was merged in\n", key);
  } else {
    // add to the existing value
    itemA += itemB;
    counters_set(setA, key, itemA);
    //printf("set A did have the item for %d, so the count was added in\n", key);
  }
}

/* Take the intersection of two sets and store it in the first set.
 * the second set is unchanged.
 */
static void 
set_intersect(counters_t *setA, counter_holder_t *setB)
{
  counters_iterate(setA, setB, set_intersect_helper);
}


static void 
set_intersect_helper(void *arg, const int key, const int count)
{
  counter_holder_t *ch = arg; 
  counters_t *copy = ch->copy;
  counters_t *counter = ch->and_seq_counters;
  
  int copycount = counters_get(ch->copy, key);
  int countercount = counters_get(counter, key); 
  int min_n;
  
  printf("big set A: %d; small set B: %d; key: %d\n", copycount, countercount, key);
  
  if (countercount == 0) {
    counters_set(ch->copy, key, 0); // not found this key in set B, set the int to zero
  } else {
    min_n = min(copycount, countercount);
    counters_set(ch->copy, key, min_n);
  }
  counters_print(copy, stdout);
}

array_index_t *order(counters_t *ctrs){
  int ntotal = 0;
  
  counters_iterate(ctrs, &ntotal, itemcount);
  
  printf("total is %d\n", ntotal);
  
  array_index_t *arraybig = malloc(sizeof(int)); 
  arraybig->array = malloc(50 * ntotal);
  arraybig->slot = 0;
  
  counters_iterate(ctrs, arraybig, arraystuff); 
  
  return arraybig;
}

//helper function to count items; tests validity of iterate function
static void arraystuff(void *arg, const int key, const int value)
{
  printf("inside stuff\n");
  array_index_t *arraybig = arg;
  int nslot = arraybig->slot;
  counters_t *counter = counters_new();
  counters_set(counter, key, value);
  
  arraybig->array[nslot] = key;
  arraybig->slot += 1;
}

//helper function to count items; tests validity of iterate function
static void itemcount(void *arg, const int key, const int value)
{
  int *nitems = arg;

  if (nitems != NULL)
    (*nitems) += 1;
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


