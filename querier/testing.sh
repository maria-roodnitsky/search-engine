#!/bin/bash

# Testing script for querier
# Author: Temi Prioleau
# Added to by: Maria Roodnitsky for both the querier lab
# Date: Feb 24, 2020
#
# usage: bash -v testing.sh
# make test 

# Define variables
# test5 is a crawler produced directories, indexfile.txt is index produced

crawlerDir=test5
indexFile=indexfile.txt

echo
echo COMMENCE TESTING FOR QUERIER

#####################################
### These tests should fail ###

# 1 argument (not enough args)
./querier

# 2 arguments (not enough args)
./querier $crawlerDir 

# 3 arguments + non-crawler produced directory
mkdir noncrawler
./querier noncrawler $indexFile
rmdir noncrawler

# 3 arguments + not readable file
touch notreadfile.txt 
chmod -r notreadfile.txt
./querier $crawlerDir notreadfile.txt
rm notreadfile.txt

######################################
### These tests should pass ####

echo TESTING queries in 'indexfile.txt'
./querier $crawlerDir $indexFile < test_query.txt

echo
echo TESTING COMPLETE FOR QUERIER

