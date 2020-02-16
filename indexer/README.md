# CS50 Lab 5
## Maria Roodnitsky, CS50 Winter 2020

### README -- Indexer

### Compiling 
To compile both `indexer` and a test program called `indextest`, which takes an indexer produced file, reloads an `index_t` object from the output, and resaves this into a new file; compile using `make all`. 

To compile just the `indexer`; compile using `make indexer`.

To compile just the `indextest` program; compile using `make indextest`. However, an indexer produced file must already exist for this program to be of much use, seeing as `indextest` takes an 'indexFile' as one of its arguments. 

To clear out excess files; `make clean`. 

To run the testing script `testing.sh` for *both* `indexer` and `indextest`; `make test`. 


### Limitations (from IMPLEMENTATION) 
If a directory comes with a '.crawler' file but is *not* crawler produced, the user is sort of out of luck. Error checking will *not* flag this as incorrect usage. The indexer also assumes that the pageIDs begin with 1 (no indexing from 0) and contain no gaps. If a crawler directory is tampered with, and say, pageID = 6 is removed, the indexer will stop reading after the pageID = 5. Once a file is not found, it stops. 


