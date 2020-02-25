/* 
 * querier.c - CS50
 * 
 * Given a crawler directory, and an index file this program takes a user inputed query and ranks the related pages in the crawler directory
 * February 2020, Maria Roodnitsky
 */

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

/************data types********/
typedef struct array_index {
  int slot; // tracks how many slots are used
  int *array;      // holds an array of keys to a counterset; the keys are stored in decreasing count order
  counters_t *counterset; //holds the counterset to which the keys in the array are attributed
} array_index_t;

typedef struct counter_holder {
  counters_t *copy;		      // holds a merged copy of all of the counters in the and sequence
  counters_t *and_seq_counters;      // holds a specific counter from the and sequence
} counter_holder_t;

/************functions********/
/* prints results of search */
void print_results(counters_t *final_and_seq, array_index_t *ordered_array, char *dirName);

/* creates the counters object that stores the ID: count pairs for every relevant page*/
counters_t *create_and_seq(index_t *index, char **words);

/*copies a counter object into a new counter object*/
counters_t *copy_func(counters_t *original);

/*merges two counter objects, essentially creating a union of the two*/
static void counters_merge(counters_t *setA, counters_t *setB);
/*helper for above function*/
static void counters_merge_helper(void *arg, const int key, const int count);

/*finds the intersection of two counter objects*/
static void counters_intersect(counters_t *setA, counter_holder_t *setB);
/*helper for above function*/
static void counters_intersect_helper(void *arg, const int key, const int item);

/*intersects several counters objects which are all in the same and sequence */
void intersect_related(int start, int end, char **words, counter_holder_t *ch, index_t *index, counters_t *copy);

/*orders the pages for result display using an insertion sort algorithm*/
array_index_t *order(counters_t *counter);
static void array_insertion_sort(void *arg, const int key, const int value);

/*returns the minimum of two numbers*/
int min(int num1, int num2); 

/*counts the number of counternodes in a counter object */
static void nodecount(void *arg, const int key, const int value);

/************main()********/
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
  
  printf("Query? "); //prompt the first query
  
  while ((line = fgets(buf, 200, stdin)) != NULL){
    words = parseLineIntoWords(line); //parse the line into an array holding words
    
    if (words == NULL){ //skip if bad input is given
      printf("Query? "); //prompt for another query
      continue; 
    }//if bad input, skip this query
    else if (!checkWordOrder(words)) { //if the word order is not permissible "and and" 
      free(words);                //or "and/or" at the beginning or end
      printf("Query? ");     //prompt for another query
      continue;              //and skip this query
    }
    printf("Query: "); //print the cleaned up query
    for (int i = 0; words[i] != NULL; i++){ 
    NormalizeWord(words[i]);
    printf("%s ", words[i]);
    } 
    printf("\n");

  counters_t *page_scores = create_and_seq(index, words); //create the counters object that holds the scores of each pageID
  array_index_t *ordered = order(page_scores); //order the counter nodes within the object 
  print_results(page_scores, ordered, dirName); //print the results to the screen
  
  //memory management
  free(words); //free the words array
  counters_del(page_scores); //delete each counter node in the page_scores counters object
  free(ordered->array); //free the array in the ordered array_index structure
  free(ordered); //free the structure itself 
  
  printf("Query? ");  //prompt for another query
  }
  
  printf("\n\nThank you for using the querier.\n");
  free(dirName); //when all done, free the directory name 
  index_free(index); //free the index
  return 0; //exit 0 for success
}

/****************print_results() ****************/
void print_results(counters_t *final_and_seq, array_index_t *ordered_array, char *dirName){ 
  int MAX_URL_LENGTH = 300;
  
  if (ordered_array->slot == 0){ //if none of the words entered exist in the index, the array will not have any filled slots
    printf("No results found\n");
  } //print this to the user
  
  for (int i = 0; i < ordered_array->slot; i++){ //for each key in the counter array
   if (counters_get(final_and_seq, ordered_array->array[0]) == 0){//if the top result has a score of zero, there must be no matches
     printf("No results found\n"); //print this to the user
     break;
   }
   else if (counters_get(final_and_seq, ordered_array->array[i]) == 0){ //if the array has reached the results with a score of zero,
   
     continue; //there are no more results to be printed, so exit the loop
   } 
   
   //make a temporary file name with the directory and pageID, which is the item in the ith slot of the ordered array
   char *tempFile = (char *)count_malloc_assert((strlen(dirName) + 4)*sizeof(char), "Not enough memory to make temporary file name.\n");
   sprintf (tempFile, "%s/%d", dirName, ordered_array->array[i]); 
   
   FILE *ifile;
   ifile = fopen(tempFile, "r"); //open the file, which is presumed to exist
   if (ifile == NULL){
     printf("Problem with crawler produced directory. File not found: %s", tempFile);
     return;
   }
   
   char *url = count_calloc_assert(MAX_URL_LENGTH + 1, sizeof(char), "Error allocating space for  url."); //allocate space for the url
   fgets(url, MAX_URL_LENGTH, ifile); //read the first line in as the url
   fclose(ifile); //close the file
   
   printf("score:%3d doc:%3d url: %s", counters_get(final_and_seq, ordered_array->array[i]), ordered_array->array[i], url); 
   free(tempFile); //free the string holding the file name
   free(url); //free the string holding the url
   }  
}

/****************create_and_seq() ****************/
counters_t *create_and_seq(index_t *index, char **words){
  counters_t *and_seq = counters_new(); //create the first and sequence
  bag_t *or_bag = bag_new(); //create a bag which will hold and sequences that need to be 'or'-ed
  
  int i = 0; //tracks which word is being processed
  int start = 0; //tracks the beginning of the current and sequence
  int end = 0; //tracks the end of the current and sequence
  
  while (words[i] != NULL){ //while there are still words to be processed
    if (isOr(words[i]) || words[i+1] == NULL){ //if this word is or, end the previous and sequence; also end if there are no more words
      if (i == 0 || !isOr(words[i])) { 
      counters_merge(and_seq, hashtable_find(index->wordHashtable, words[i])); 
      end += 1;}
      counter_holder_t *ch = count_malloc_assert(sizeof(counter_holder_t), "Error allocating memory for counter holder."); //create a counter holder
      counters_t *copy = copy_func(and_seq); //copy the current and sequence into the copy 
      ch->copy = copy; //insert this into the counter holder
      counters_del(and_seq); //free the and sequence
      intersect_related(start, end, words, ch, index, ch->copy); //intersect every word with the and sequence copy
      bag_insert(or_bag, copy); //once the and sequence has been processed, 
      free(ch); //free the counter holder
      if (isOr(words[i])) {
        and_seq = counters_new(); //allocate for a new and sequence if this is not the end of the words
        start = i + 1;
      }
    } else if (isAnd(words[i])){ //if the word is and, there isn't much to do
      i++; 
      end += 1;
      continue;
    } else {
      counters_merge(and_seq, hashtable_find(index->wordHashtable, words[i])); //otherwise, merge the counters for the word with the 
    }
    i++;
    end += 1;
  }    
  
  counters_t *query_res = counters_new(); //allocate space for the final counter object created by this query
 
  counters_t *set;   
  while ((set = bag_extract(or_bag)) != NULL){ //extract counters holding and sequences 
    counters_merge(query_res, set); //merge them with the final result 
    counters_del(set); //free the individual and sequences
  }
  bag_delete(or_bag, NULL); //delete the bag when it is empty
  return query_res; //return the final counters object
}

/****************intersect_related() ****************/
void intersect_related(int start, int end, char **words, counter_holder_t *ch, index_t *index, counters_t *copy){
  for (int j = start; j < end; j++){ //for all of the words in the and sequence
    if(!isAnd(words[j])){ //if they are NOT 'and' (which is a useless word)
      ch->and_seq_counters = hashtable_find(index->wordHashtable, words[j]); //intersect the counters with the and sequence
      counters_intersect(copy, ch);}
      }
}

/****************copy_func() ****************/
counters_t *copy_func(counters_t *original){
  counters_t *copy = counters_new(); //allocate space for a new counters object
  counters_merge(copy, original); //merge the copy with the information from the original 
  return copy;
}

/****************counters_merge() ****************/
static void counters_merge(counters_t *counterA, counters_t *counterB){
  counters_iterate(counterB, counterA, counters_merge_helper);
}

/****************counters_merge_helper() ****************/
static void 
counters_merge_helper(void *arg, const int key, const int countB)
{
  counters_t *counterA = arg;
  int countA = counters_get(counterA, key); //find the count for this key in counter A
 
  if (countA == 0) {
    counters_set(counterA, key, countB); //if the counter A does not have it, add this key with counter B's count
  } else {
    countA += countB; //if both counters have this key, put their sum into counter A
    counters_set(counterA, key, countA); 
  }
}

/****************counters_intersect() ****************/
static void counters_intersect(counters_t *counterA, counter_holder_t *counterholder){
  counters_iterate(counterA, counterholder, counters_intersect_helper); 
}

/****************counters_intersect_helper() ****************/
static void counters_intersect_helper(void *arg, const int key, const int count){
  counter_holder_t *ch = arg; 
  
  int copycount = counters_get(ch->copy, key); //grab the overall count for this pageID from the copy
  int countercount = counters_get(ch->and_seq_counters, key); //grab the count of this pageID for this word 
  
  if (countercount == 0) { //if this word doesn't exist on the page
    counters_set(ch->copy, key, 0); // set the overall count (stored in the copy) to zero
  } else {
    counters_set(ch->copy, key, min(copycount, countercount)); //otherwise, set it to the minimum of the two counts
  }
}

/****************order() ****************/
array_index_t *order(counters_t *counter){ 
  int ntotal = 0; //number of counter_node objects in the counter object
  counters_iterate(counter, &ntotal, nodecount); //iterate through the counter object to calculate the number of nodes
  
  array_index_t *array_index = count_malloc_assert(sizeof(array_index_t), "Error allocating memory for array index."); //allocate space for an array_index struct
  array_index->array = count_malloc_assert(sizeof(int)*ntotal, "Error allocating for interal array."); //allocate space for an array of ntotal integers
  array_index->slot = 0; //set the first open index to 0 
  array_index->counterset = counter; //set the internal counterset as the one passed in as a parameter
  
  counters_iterate(counter, array_index, array_insertion_sort); //iterate through the counter object and insert nodes in order
  return array_index;
}

/**************** array_insertion_sort() ****************/
static void array_insertion_sort(void *arg, const int key, const int value){
  array_index_t *array_index = arg; //the argument passed in should be of type 'array_index_t'
  int nslot = array_index->slot; //count of slots in the array in the array_index struct
  
  array_index->array[nslot] = key; //put the nth counter_node's key into the nth slot of the array
  
  int i, temp; //perform an insertion sort
  i = nslot; //bubble down the element that was just inserted until it is in the sorted place
  
  while (i > 0 && counters_get(array_index->counterset, array_index->array[i-1]) < counters_get(array_index->counterset, array_index->array[i])){ //while the count associated with this pageID is bigger than the one before it, flip them
    temp = array_index->array[i]; //flip the two indices using a temporary holder
    array_index->array[i] = array_index->array[i-1];
    array_index->array[i-1] = temp;
    i--; //keep moving back one until either the elements are ordered or it has reached the front of the array
    }
  
  array_index->slot += 1; //increase the number of slots by one for the next counter_node
}

/**************** item_count() ****************/
static void nodecount(void *arg, const int key, const int value){
  int *nitems = arg; //let the argument passed in be the address of a variable that stores the number of
                     //counter_nodes seen in this counter object so far
  if (nitems != NULL) //if the address is valid, increment the value stored at it by 1 for each counter_node seen
    (*nitems) += 1;
}

/**************** min() ****************/
int min(int num1, int num2){
  if (num1 < num2) //if the first num1 is smaller than num2, return it
    return num1;
  else //otherwise, num2 must be less than or equal to num1, in which case, the value it returns will be the overall min
    return num2;
}
