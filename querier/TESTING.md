# CS50 Lab 6
## Maria Roodnitsky, CS50 Winter 2020

### TESTING - Querier

The `querier` was tested by the `testing.sh` script, which validates that the program behaves accordingly when given various combinations of valid and invalid command-line arguments. This program also tests a test file of cases stored in `test_query.txt`

### Cases Tested in 'test_query.txt'

1. Lines with invalid characters
2. Lines with excess spaces
3. Empty Lines
4. Lines with just 'and'
5. Lines with just 'or'
6. Lines beginning with 'and'
7. Lines beginning with 'or'
8. Lines ending with 'and'
9. Lines ending with 'or'
10. Lines with adjacent 'and' and 'or' combinations
11. Line where 'and' or 'or' is a substring of a bigger word like 'andor' which should not be flagged as invalid
12. Lines with no 'and'/'or', multiple words
13. Line with a single word in the index
14. Lines with a single word not in the index
15. Lines with various capitalization
16. Lines with multiple words not in the index
17. Lines with combinations of 'and'/'or', and both words in and not in the index

### Query Parser Testing
The line parsing was tested through a test program called `input_tester.c`. This program validates that there are no improper characters in a line entered by the user (stdin), then it prints back these words and their type: *or*, *and*, or *word*. 

Example line: dartmouth or harvard won the final football game and ivy league championship. 

Output: 
word found: dartmouth

or found: or

word found: harvard

word found: won

word found: the

word found: final
  
word found: football
 
word found: game
 
and found: and

word found: ivy

word found: league

word found: championship