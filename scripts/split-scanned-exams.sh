#!/bin/bash
#By Claudio A. Parra, UCI. 2018.
#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 2 of the License.

#This script is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.

#Description: 
#This script splits the big pdf that results from the scan procces, into
#individual exams, so the upload process in Gradescope is easy and fast.


#pdf with all the exams
BIG_PDF=all.pdf

#how many exams are there in that document?
NUM_EXAMS=197

#how many pages does each exam has?
PAGES_PER_EXAM=10

#how many blank pages are there at the end of each exam?
#it would usually be 0 or 1.
BLANK_PAGES_AT_END=1

#make it 1 to just print the pdftk command on screen but not run them
#make it 0 to run the splitting
DRY_RUN=1;

if [[ $DRY_RUN == 1 ]];
    then
	echo "DRY RUN, JUST PRINTING ON SCREEN."
	echo "Edit the variable DRY_RUN of the script to execute"
	echo "the instructions."
fi;

echo "mkdir split/"

if [[ $DRY_RUN == 0 ]];
    then
	mkdir split/
fi;

for i in $(seq -w 1 $NUM_EXAMS);
do
    let start=$((10#$i-1))*$PAGES_PER_EXAM+1
    let end=$start+$PAGES_PER_EXAM-$BLANK_PAGES_AT_END-1
    com="pdftk $BIG_PDF cat $start-$end output split/$i.pdf;"
    echo $com
    if [[ $DRY_RUN == 0 ]];
    then
	eval $com
    fi;
    
done;
