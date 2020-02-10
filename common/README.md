# CS50 Lab 4
## Maria Roodnitsky, CS50 Winter 2020

### pageDir [2/8/2020]

`pageDir` will store all shared modules for the tiny search engines. As of right now, it contains one function, `pageSaver`. 

### Usage

The *pageDir* module, defined in `pageDir.h` and implemented in `pageDir.c`, exports the following function through `pageDir.h`:
```c
/* Write the current page to a file.*/
int pageSaver(char *fileName, webpage_t *currPage);
```

### Implementation
`pageSaver` is a function to save crawled pages to a file in a specific format. It returns 0 if the file-writing goes successfully; otherwise, it returns an error exit code of 1.

It writes pages in the following format:

[url]

[depth at which it was discovered]

[html]


### Assumptions

No assumptions beyond those that are clear from the spec.

The filename provided to the function is assumed to be in the format "*directory/pageID*", where *directory* refers to the directory where crawled pages are being written, and *pageID* is the crawler-provided page number of the website; for the greater purpose of the tiny search engine. While it will accept other file names, in the greater context of the project, this is a requirement. 

As a result of the greater context, both the file name and the crawler object are assumed to not be null.

### Compilation
To compile, simply `make pageDir.o`.

### Exit Codes
0: No errors
1: Error in opening/writing to file

To test, simply `make test`.
