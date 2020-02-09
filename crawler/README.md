# CS50 Lab 4
## Maria Roodnitsky, CS50 Winter 2020

### crawler 

`crawler` implements the crawling portion of the tiny search engine by crawling from page to page and storing its HTML. It introduces the `crawler` structure, which comes with a `crawler_new` function for initialization. It also contains the function `processURLs` which gets used internally. 

### Internal Functions

```c
/* add the URLs found in a page to the 'to be explored' hashtable or ignore */
void processURLs(webpage_t *currPage, crawler_t *crawler)

/*delete an item (used in this context for freeing empty strings)*/
void itemdelete(void *item)

/*initialize the crawler*/
static crawler_t *crawler_new(int userMaxDepth, char *userDirName)
```

### Usage

This program should be used from the command-line as follows:
`./crawler [startingURL] [dirName] [maxDepth]`
Where `./crawler` refers to the name of the program, `[startingURL]` is the URL from which crawling begins, `[dirName]` is the pre-made directory to which file contents are written, and `[maxDepth]` is the depth to which crawls go. Otherwise, the program terminates as a result of error catching. 

### Assumptions

No assumptions beyond those that are clear from the spec.

The starting URL provided to the function is assumed to be in the format "*http://old-www.cs.dartmouth.edu/*". If not, then the URL is not internal and will not be processed. The directory to which files are written is also assumed to exist. For now, maxDepth is constrained to [0,10] (can be adjusted), and will not accept input beginning with non-numeric values and will otherwise strip out non-numeric values. 

URLs are constrained to 300 characters. 


### Compilation
To compile, simply `make crawler.o'

### Exit Codes
0: No errors\n
1: User did not provide 4 command-line arguments\n
2: User did not provide a valid directory\n
3: User did not provide a depth in the bounds [0, MAXIMUM_DEPTH]\n
4: User did not provide a valid starting URL. \n
5: User provided a depth beginning with a non-numeric character\n
6: An error occurred in writing a file. \n

To test, simply `make test`.
