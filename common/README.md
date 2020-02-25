# CS50 Lab 5
## Maria Roodnitsky, CS50 Winter 2020

### pageDir [Updated 2/16/2020] 

`pageDir` will store shared page manipulating modules for the tiny search engines. As of right now, it contains two functions, `pageSaver`and `pageLoader`. 

### Usage

The *pageDir* module, defined in `pageDir.h` and implemented in `pageDir.c`, exports the following functions through `pageDir.h`:
```c
/* Writes the current page to a file.*/
int pageSaver(char *fileName, webpage_t *currPage);
/* Given a crawler produced file, recreates a webpage object
webpage_t *pageLoader(char *fileName);
```

### Implementation
`pageSaver` is a function to save crawled pages to a file in a specific format. It returns 0 if the file-writing goes successfully; otherwise, it returns an error exit code of 1.

It writes pages in the following format:

[url]

[depth at which it was discovered]

[html]

`pageLoader` is a function to recreate webpage_t objects from crawler produced files. It takes a 
crawler produced file [fileName] with data in the format listed above. If the file is not openable, it is presumed to be the n+1th pageID of n crawler files and returns NULL to signify the end of crawler files. With a crawler produced file, the function returns a pointer to a webpage_t item.

### Assumptions

No assumptions beyond those that are clear from the spec.

The filename provided to the `pageSaver` is assumed to be in the format "*directory/pageID*", where *directory* refers to the directory where crawled pages are being written, and *pageID* is the crawler-provided page number of the website; for the greater purpose of the tiny search engine. While it will accept other file names, in the greater context of the project, this is a requirement. Likewise, the filename provied to `pageLoader` is assumed to be crawler produced, thus is in the format "*directory/pageID*" and contains data as is expected. The only time this function should encounter a filename that is it unable to process is when it attempts to load the first non-existent crawler file, that is, the n+1th crawler file. 

As a result of the greater context, both the file name and the crawler object are assumed to not be null when given to `pageSaver`.


### Compilation
To compile, simply `make pageDir.o`.

### Return Values
`pageSaver`:
0: No errors
1: Error in opening/writing to file

`pageLoader`: none

### index [2/16/2020]

`index` will store shared modules for the tiny search engines. As of right now, it contains four main functions:`index_new`, `index_save`, `index_free`, and `index_load`.

### Usage

The *index* module, defined in `index.h` and implemented in `index.c`, exports the following functions through `indexr.h`:
```c
/* Creates a pointer to a new index object*/
index_t *index_new(char *userDirName, char *userFileName)
/* Frees all dynamically allocated data within the index */
void index_free(index_t *index)
/* helper function for index_free to free dynamically allocated counter memory */
void counters_del(void *item)
/* takes an index_t item and writes its contents to a file in a given format */
void index_save(index_t *index)
/* helper function for save_item to save keys (words) */
void save_item (void *arg, const char *key, void *item)
/* helper function for save_item to save counters associated to words*/
void save_counts (void *arg, const int key, int count)
/* given a file produced by index_save, back-constructs an index_t object */
index_t *index_load(char *oldIndexFilename, char *newIndexFilename)


```

### Implementation and Assumptions
`index_new` is a function that takes the name of a crawler produced directory [userDirName] and the name of a file to which index data will be written [userFilename], It then dynamically allocates memory for a hashtable, which will hold words as keys and counter objects as items, with pageID as key, the count for that word in that file as count. 

`index_new` assumes that both the directory and file name are not NULL. It also assumes that it has been previously checked that the directory is crawler produced (by finding the .crawler file) and that the file given by the user is writable.

`index_free` is a function that takes an index_t object and frees all of the dynamically allocated memory within it. It uses the help of a helper function `counters_del` to clear out the memory within the counters.

`index_free` makes no assumptions.

`index_save` is a function that takes an index_t object and 'saves' it to the file within the index_t object with the use of helper functions `save_item` and `save_counts` as it iterates through the word hashtable within the index_t structure.  

`index_save` makes the assumption that the index_t object is not NULL, and the file within it should have previously been checked to be writable. 

`index_load` takes a file produced by an indexer [oldIndexFilename] and back-loads it into a new index_t structure with a new file to which to be written [newIndexFilename]. 

`index_load` makes the assumption that the new file has been checked to be writable; the old, readable and in indexer-produced format. 

### word [2/24/2020]
`word` will store shared word processing modules for the tiny search engines.
### Usage

The *word* module, defined in `word.h` and implemented in `word.c`, exports the following functions through `word.h`:
```c
/* Given a word, converts all of its letters to lowercase */
void NormalizeWord(char *word)
/* Takes a string and parses it into words with spaces as deliminators. If there are any non-space or non-alphabetic characters, returns NULL. Otherwise, returns an array of pointers to parsed words. */
char **parseLineIntoWords(char* line);
/* Takes an array of words and confirms that the word order is correct. If the line begins or ends with 'or' or 'and', or the two words are adjacent in any way, returns false. If the word order is correct, returns true. */
bool checkWordOrder(char **words);
/* Takes a word and checks if it is 'and'; if 'and', return true; if not 'and', return false.*/ 
bool isAnd(char *word);
/* Takes a word and checks if it is 'or'; if 'or', return true; if not 'or', return false.*/ 
bool isOr(char *word);
```
### Implementation and Assumptions
`NormalizeWord` is a function that takes a word and converts it to lowercase.`NormalizeWord` assumes that the word is not null. It only converts alphebetic characters; all else is ignored.

`parseLineIntoInput` takes a string and parses it with ' ' as a deliminator. It returns an array of pointers to `char *` objects if there are no invalid characters. Otherwise, it returns NULL. `parseLineIntoInput` assumes the line is not null. 

`checkWordOrder` takes an array of `char *` objects and confirms that there are no 'and' or 'or's in incorrect places. It returns true if there are no issues with word order; false, otherwise. `checkWordOrder` assumes that neither the word array given or the words inside are null.

`isAnd` takes a `char *` and returns *true* if it is "and"; otherwise, it returns false. `isAnd` assumes the `char *` is not null. 

`isOr` takes a `char *` and returns *true* if it is "or"; otherwise, it returns false. `isOr` assumes the `char *` is not null. 

To compile the complete library, simply `make`. To clean up, `make clean`.
--

