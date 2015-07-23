#!/bin/bash

# process the output of benchmark.sh
# program input 1: imagelist.txt, which list all tested docker images in acending order by image size
# program input 2: output0, which is a directory contains all output of the benchmark.sh 

# step 1: get the push and pull time from the output of benchmark.sh
# step 1: compute the average push, pull and total time (omit the data of 2 largest and 2 smallest total time)
# step 1: print push, pull and total time and their average for each image  
gcc process1.c
./a.out

# step 2: print average push, pull and total time of all image into a single file (in the order of imagelist, that is, in acending order by image size)
gcc process2.c
./a.out

# step 3: separate push, pull and total time, then the result can be easily copied to excel or world table
gcc process3.c
./a.out

rm a.out
