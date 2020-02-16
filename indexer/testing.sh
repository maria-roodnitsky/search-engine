#!/bin/bash

# Testing script for indexer
# Author: Temi Prioleau
# Added to by: Maria Roodnitsky for both the crawler lab and the indexer lab
# Date: Feb 16, 2020
#
# usage: bash -v testing.sh
# make test 

# Define variables
# test2 and test5 are two crawler produced directories

crawlerDir1=test2
crawlerDir2=test5

echo
echo COMMENCE TESTING FOR INDEXER

#####################################
### These tests should fail ###

# 1 argument (not enough args)
./indexer

# 2 arguments (not enough args)
./indexer $crawlerDir1 

# 3 arguments + non-crawler produced directory
mkdir noncrawler
touch writablefile.txt
./indexer noncrawler writablefile.txt
rmdir noncrawler

# 3 arguments + not writable file
touch notwritfile.txt 
chmod -w notwritfile.txt
./indexer $crawlerDir1 notwritfile.txt
rm notwritfile.txt

######################################
### These tests should pass ####

echo TESTING writing $crawlerDir1 data to 'dir1output.txt'
touch dir1output.txt
./indexer $crawlerDir1 dir1output.txt

echo TESTING writing $crawlerDir2 data to 'dir2output.txt'
touch dir2output.txt
./indexer $crawlerDir2 dir2output.txt

echo
echo TESTING COMPLETE FOR INDEXER

echo 
echo COMMENCE TESTING FOR INDEXTEST

#####################################
### These tests should fail ###

# 1 argument (not enough args)
./indextest

# 2 arguments (not enough args)
./indextest dir2output.txt

# 3 arguments + unreadable input file
chmod -r dir2output.txt 
touch dir2outputtransfer.txt
./indextest dir2output.txt dir2outputtransfer.txt
chmod +r dir2output.txt


# 3 arguments + not writable output file
chmod -w dir2outputtransfer.txt
./indextest dir2output.txt dir2outputtransfer.txt
chmod +w dir2outputtransfer.txt
######################################
### These tests should pass ####

touch dir1outputtransfer.txt
echo TESTING transfering 'dir1output' data to 'dir1outputtransfer'
./indextest dir1output.txt dir1outputtransfer.txt

echo TESTING transfering 'dir2output' data to 'dir2outputtransfer'
./indextest dir2output.txt dir2outputtransfer.txt

echo
echo TESTING COMPLETE FOR INDEXTEST
