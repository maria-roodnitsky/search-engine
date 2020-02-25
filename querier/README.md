# CS50 Lab 6
## Maria Roodnitsky, CS50 Winter 2020

### README -- Querier

### Compiling 
To compile both `querier` and a test program called `input_tester`, which reads queries from stdin and confirms that they are valid; compile using `make all`. 

To compile just the `querier`; compile using `make querier`.

To compile just the `input_tester` program; compile using `make input_tester`.

To clear out excess files; `make clean`. 

To run the testing script `testing.sh` for *both* `indexer` and `indextest`; `make test`. 


### Limitations (from IMPLEMENTATION) 
If a directory comes with a '.crawler' file but is *not* crawler produced, the user is sort of out of luck. Error checking will *not* flag this as incorrect usage. The querier also assumes that the pageIDs begin with 1 (no indexing from 0) and contain no gaps. If a crawler directory is tampered with, and say, pageID = 6 is removed, the querier will not function properly because it assumes that each pageID in the index file exists. 


### Functionality 
All requirements of the spec were implemented. Precedence is given to 'and' over 'or,' and the output is displayed in ranked order. There are also no memory leaks. (Yay!)