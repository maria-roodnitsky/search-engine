# CS50 Lab 6
## Maria Roodnitsky, CS50 Winter 2020

### IMPLEMENTATION -- Querier
### The `array_index_t` and `counter_holder` Structure
The querier relies on two structures, the `struct array_index` and `struct counter_holder`, which contains the following parameters: 
```
typedef struct array_index {
  int slot; // tracks how many slots are used
  int *array;      // holds an array of keys to a counterset; the keys are stored in decreasing count order
  counters_t *counterset; //holds the counterset to which the keys in the array are attributed
} array_index_t;
```

```
typedef struct counter_holder {
  counters_t *copy;		      // holds a merged copy of all of the counters in the and sequence
  counters_t *and_seq_counters;      // holds a specific counter from the and sequence
} counter_holder_t;
```

### Pseudocode
First, `querier.c` checks to make sure it has been given three command line arguments: [./indexer] [pageDirectory] [indexFilename]. Second, it checks for the existence of a '.crawler' file within the directory. Third, it makes sure the file given by the user is readable. *The file is assumed to be indexer produced.* It then reads input from the user, error checks the query, then processes the query. 

If an and word is seen, it is treated as a part of an and sequence. Words not seperated by 'or' or 'and' are assumed to be joined with 'and'. All and sequences are processed internally first, then processed against one another as or sequences. 

The final counter object is then stored in an ordered array by key (thanks to insertion sort) then printed to the user. 

This is cycled until the end of the file is reached. 

### Functions
```
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
```
`print_results` prints "no results found" if no words were ever added to the ordered array. It also prints "no results found" if the top result is has a count of zero. It stops printing when counts become zero. It then creates a temporary file for each pageID, grabs the url from the crawler produced file, and prints the score, the id, and the url. It then frees all temporarily used memory. This essentially prints the results in order.

`create_and_sequence` first allocates space for a `counter` object, and_seq, and a `bag` object which will store and sequences separated by 'or' and merge them at the end of processing all and sequences internally. If a word is 'or' or is the last word, the and sequence is processed internally. That is, it intersects itself by every individual word counter, and then adds itself to the bag to be processed by "or". Once all and sequences have been processed internally, they are added to the bag, and the bag merges them all together. In the end, the function returns one counter object that is the result of first internally processing and sequences, and then merging all and sequences together. 


`intersect_related` takes a start, an end, an array of pointers to words, a counter holder, and a counter object "copy", and from start to end, it intersects the copy with the counter set it finds in the index attributed with the iterated key. 

`copy_func` creates a new counter object and merges a counter into it, by proxy, creating a copy of it. 


`counters_merge` takes two counter sets and merges the second one into the first. 

`counters_merge_helper` finds the count for this pageID for both counters and puts the sum of the counts into the first counter object, which in context, is the changing and sequence counter. 

`counter_intersect` intersects a non-changing counter with the dynamic counter copy. 

`counter_intersect_helper` takes a `struct counter_holder` which holds the copy of an and sequence which is being modified, and a counter sequence. If the current counter object does not contain the key, it sets the overall pageID count in the copy to 0. Otherwise, if the current counter does contain this key, it sets it to the minimum between the overall and the counter.  

`order` takes a counter object, and iterates through it to count how many counternodes are within it. It then creates a `struct array_index` and stuffs it using `array_insertion_sort`. 

`array_insertion_sort` is a helper function that takes a `struct array_index` and fills it as a counter set is iterated over. It first places the given counter node into the last index of growing array, then it bubbles it down so that the array remains sorted by count size. The array holds keys, and the value attributed to them is check each time upon comparison. 

`min` takes two integers and returns the smaller of the two. 

`nodecount` is a helper function which, when used by a counter iterator, counters the number of counternodes in the counter structure. 



### Error Management
If the user does not provide 3 arguments, `exit code 1` is returned and the program terminates. If the user does not give a crawler produced directory, `exit code 2` is returned and the program terminated. If the user does not provide a readable file, `exit code 3` is returned and the program terminates. If an error occurs in loading the index, the program terminates with `exit code 4`. If all executes according, the program exits with `exit code 0`.  

### Output
Upon a successful run, the program will list relevant documents in decending order. 
*Sample Output:*
Query? breadth first search 
Query: breadth first search 
score:  1 doc:  3 url:  http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html

### Limitations: 
If a directory comes with a '.crawler' file but is *not* crawler produced, the user is sort of out of luck. Error checking will *not* flag this as incorrect usage. The querier also assumes that the pageIDs begin with 1 (no indexing from 0) and contain no gaps. If a crawler directory is tampered with, and say, pageID = 6 is removed, the querier may try to look into it, and problems may arise. 

Please don't tamper with *anything* produced by the crawler or indexer. There may be undefined behaivor.If nothing is tampered with, behaivor should be fine. 
