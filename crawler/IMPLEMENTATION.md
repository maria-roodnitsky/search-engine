# CS50 Lab 4
## Maria Roodnitsky, CS50 Winter 2020

### crawler IMPLEMENTATION 

## Summary
Crawler is implemented primarily through the use of a helper structure, also aptly named `crawler`. This module also contains a function to initialize this structure, `crawler_new` and a function to process URLs, `processURLs`. The main method checks the commandline arguments and error checking occurs throughout. 

## Data Structures

### crawler struct

```
typedef struct crawler {
  struct bag *webpageBag;
  struct hashtable *webpageHashtable; //hashtable of all seen URLS
  int maxDepth; //maximum depth to which to crawl
  int pageID; //number of page visited (for file writing)
  char *dirName; //name of directory to which to write webpage data 
} crawler_t; 

```

* webpageBag : bag of uncrawled web pages, implemented in bag.c
* webpageHashtable: hashtable of all seen URLS, implemented in hashtable.c
* maxDepth: maximum depth to which to crawl
* pageID: number of page visited (for file writing)
* dirName: name of directory to which to write webpage data 

#### Functions:  
```
static crawler_t *crawler_new(int userMaxDepth, char *userDirName)
```
Allocates space to the crawler, initializes the contents, returns a crawler_t structure.

```
void processURLs(webpage_t *currPage, crawler_t *crawler)
```
While there are still new URLs on a page to be explored, it checks to see if they are internal, and if they have been seen yet. In some cases, it adds these URLs to the hashtable within the crawler and creates webpages to be visited, which are added to the bag within the crawlers. Other times, it ignores the URLs and deals with transient memory accordingly. 
```
int main(const int argc, const char *argv[])
```
Checks to make sure there are four arguments, otherwise exits. Checks to make sure the given directory exists, otherwise exits. Checks to make sure the depth is valid, otherwise exits. Checks to make sure the starting URL is internal and valid, otherwise exits. Creates a crawler, a starting page, and begins to process this starting page. While there are still pages to be processed, calls `processURLs` on them. When all pages within the correct depth have been explored, frees the crawler and its contents. 

### Error Checking
There are six possible exits codes, all of which are documented in the 'README.md'for this module. 

## Helper modules

* hashtable.h (set.h, jhash.h)
* bag.h
* memory.h
* webpage.h (file.h)

## Error Handling
The main method handles most errors dealing with incorrect arguments. 
