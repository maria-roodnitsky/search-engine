#!/bin/bash

# Testing script for crawler.c
# Author: Temi Prioleau
# Added to by: Maria Roodnitsky
# Date: Feb 8, 2020
#
# usage: bash -v testing.sh

#The following directories should exist: data, test0, test1, test1a, test2, test5, test5a, testwikipedia

# Define variables
seedURL=http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
externalURL=www.google.com
nonexistentserver=www.mariaandsabrina.com
nonexistentpage=http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/maria.html
seed2URL=http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html
seedwikipedia=http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/

#####################################
### These tests should fail ###

echo COMMENCE TESTING

# 1 argument
./crawler

# 2 arguments
./crawler $seedURL

# 3 arguments
./crawler $seedURL data

# 4 arguments + externalURL
./crawler $externalURL data 2

# 4 arguments + no directory 
./crawler $seedURL noDir 5

# 4 arguments + out of bounds depth
./crawler $seedURL data 15

# 4 arguments + out of bounds depth
./crawler $seedURL data -2

# 4 arguments + non-numeric depth
./crawler $seedURL data a89

# 4 arguments + nonexistent server
./crawler $nonexistentserver data 8

# 4 arguments + nonexistent page
./crawler $nonexistentpage data 2


######################################
### These tests should pass ####

echo TESTING $seedURL AT DEPTH 0
# at depth 0
./crawler $seedURL test0 0

echo TESTING $seedURL AT DEPTH 1
# at depth 1 with string depth
./crawler $seedURL test1a 1a

echo TESTING $seedURL AT DEPTH 1 WITH NON-NUMERIC
# at depth 1
./crawler $seedURL test1 1

echo TESTING $seedURL AT DEPTH 2
# at depth 2
./crawler $seedURL test2 2

echo TESTING $seedURL AT DEPTH 5
# at depth 5
./crawler $seedURL test5 5

echo TESTING $seed2URL AT DEPTH 5 
# at depth 5, second seed
./crawler $seed2URL test5a 5


echo TESTING $seedwikipedia AT DEPTH 1
# at depth 1, wikipedia seed
./crawler $seedwikipedia testwikipedia 1

echo TESTING COMPLETE
