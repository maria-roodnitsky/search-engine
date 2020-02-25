# CS50 Lab 6
## Maria Roodnitsky, CS50 Winter 2020

### DESIGN -- Querier
The querier takes a crawler produced directory and indexer produced index file, and displays which pages match a user-entered query. It relies on several structures such as a `counter_holder_t` which holds the addresses of multiple counter objects, seeing as most operations are done on counter objects; and will largely contain functions which deal with counter object manipulation. 

### Pseudocode
First, the querier processes commandline arguments, making sure that the first argument is a crawler produced directory and that the second is a valid, readable file, which is assumed to be indexer produced. Second, the data from the index file is loaded into an `index_t` structure. Then, queries are read from standard input until end of file is reached. The queriers are first parsed into normalized words. If there are improper characters (anything other than a space or an alphabetic character), an error message is displayed and processing does not occur. If word order is incorrect, an error message is also printed. If the query is valid, it is reprinted in normalized form and the user is shown a list of documents in ranked order. If no documents match, the user is also informed of that. When the end of file is reached, the program terminates with exit code 0, and dynamically allocated memory is freed. 

### Input
The function takes three commandline arguments:
./querier [crawlerDirectory] [indexerFilename]

Error checking ensures that these parameters are valid. 

### Output
The user is prompted to search for queries. If they enter invalid query syntax, error messages are printed. If they enter valid queries with no results, a "no results" message is printed. If they enter a valid query with results, the documents that match the query are displayed in order of relevance. 

### Testing Plan
First, a function to test the validity of queries will be written. Then, the querier will be tested with a testing.sh script which will pipe in various forms of queries with words that should exist in the index file name and be connected to files in the crawler produced directory. 

