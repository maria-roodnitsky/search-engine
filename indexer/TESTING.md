# CS50 Lab 5
## Maria Roodnitsky, CS50 Winter 2020

### TESTING - Indexer

The `indexer` was tested both by the `testing.sh` script, which validates that the program behaves accordingly when it is given invalid input (usually by terminating). It was also tested with `indextest.c` which confirms that an `index_t` object can be constructed, deconstructed, and reconstructed, and yet will always produce files with the same output. 

### Output 
Because a hashtable does not maintain order, when an `index_t` object is reconstructed and again saved, the output may (and probably will) appear in a different order. This is expected and is indicative of a working indexer. 



