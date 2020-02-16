# CS50 Lab 5
## Maria Roodnitsky, CS50 Winter 2020

### IMPLEMENTATION -- Indexer
### The `index_t` Structure
The indexer relies heavily on a `struct index`, which is defined in `index.h` and contains the following parameters: 
```
typedef struct index {
  struct hashtable *wordHashtable; //hashtable of words
  char *dirName; //name of crawler produced directory holding webpage data 
  char *fileName; //name of file to which indices will be written
} index_t; 
```
The `wordHashtable` holds words as *key*, and counter objects as *item*. For further information, see DESIGN.md. `dirName` is presumed to be a directory correctly produced by the crawler, and `fileName` is the name of the file to which indices are written. There is no error checking to make sure the crawler created the directory correctly, the program merely looks for the existence of a '.crawler' file.

### Pseudocode
First, `indexer.c` checks to make sure it has been given three command line arguments: [./indexer] [pageDirectory] [indexFilename]. Second, it checks for the existence of a '.crawler' file within the directory. Third, it makes sure the file given by the user is writable. It then allocates space for the index, calls the `index_build` function (described later), saves the index using `index_save` (see README within common for more information), and then frees the index object. 

### Functions
```
/* takes an index_t object and using the crawler produced directory inside of it, fills it with the data*/
void index_build(index_t *index)
```
`index_build` does the bulk of the job for the indexer. First, it initializes space for a counter_set (to hold word counts), space for temporary file names (to hold 'pageDir/pageID' files), space for a webpage from which words will be read, and space for the individual words as well. It then calls `pageLoader` on each 'pageDir/pageID' file (for more information on `pageLoader` and `NormalizeWord`, see the section on `pageDir` and `word` within the README in the 'common' library). It then iterates through the document, grabbing words using `webpage_getNextWord` and normalizing them using `NormalizeWord`. If the word is less than three characters, it is ignored. If it meets the minimum length, the hashtable within the index is referenced to see if the word has been seen. If it has not, a new counter object is created with count as 1, key as the pageID at which it was found. The counter object is then inserted into the hashtable with the word as key. If the word already exists in the hashtable and has been seen on the page, the count within the counter object is incremented by one. If the word already exists in the hashtable but has not yet been seen on this page, a new counter object is inserted with count as 1, word as key. This is repeated for all words on each crawler page. The process ends once the indexer attempts to read from the n+1th page and encounters a NULL page. Finally, all temporarily-used memory is free'd.  

### Error Management
If the user does not provide 3 arguments, `exit code 1` is returned and the program terminates. If the user does not give a crawler produced directory, `exit code 2` is returned and the program terminated. If the user does not provide a writable file, `exit code 3` is returned and the program terminates. If all executes according, the program exits with `exit code 0`.  

### Output
Upon a successful run, the program creates a file (named by the user in the command line arguments) with index data in the following format: 
[word]: [pageID] [count] [pageID] [count]
[word]: [pageID] [count] [pageID] [count]
...
[word]: [pageID] [count] [pageID] [count]

### Limitations: 
If a directory comes with a '.crawler' file but is *not* crawler produced, the user is sort of out of luck. Error checking will *not* flag this as incorrect usage. The indexer also assumes that the pageIDs begin with 1 (no indexing from 0) and contain no gaps. If a crawler directory is tampered with, and say, pageID = 6 is removed, the indexer will stop reading after the pageID = 5. Once a file is not found, it stops. 

### Testing Plan

The indexer should be able to handle valid and invalid numbers of arguments, writable and unwritable files, and crawler produced and *not* crawler produced directories accordinly. This will be tested by `testing.sh` and output will be found in `testing.out`.
