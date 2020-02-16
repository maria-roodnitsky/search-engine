# CS50 Lab 5
## Maria Roodnitsky, CS50 Winter 2020

### DESIGN -- Indexer

The indexer takes a crawler produced directory and iterates through each page, creating a count of how many times each word appears. It relies on a `index_t` structure, which stores the name of the crawler directory, and the name to which the contents of the index will be written. It stores all of the data in a hashtable, with individual words it encounters in the files as keys, and counter objects as the associated items. For each word, there can be multiple counters, each counter being identified with pageID as *key*, and count of times the word has been seen on that page as *count*. 

### Pseudocode
First, the indexer processes commandline arguments, making sure that the first argument is a crawler produced directory and that the second is a valid, writable file. Second, memory is allocated for an `index_t` structure, essentially *new*-ing an index object. Then, the index is filled with the data from the crawler directory, and then, it is saved to a file in a format specified in the `index` module which can be found in the `common` directory. Finally, dynamically allocated memory for the index object is freed. 

