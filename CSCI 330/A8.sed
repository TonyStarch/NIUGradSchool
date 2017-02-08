#!/bin/sh

######################################
# Assignment 8 - Sed 
# Brandon Tweed
# Section 2
# Instructor: Berezinski
# TA: Muhtaris
######################################

passwd=/home/max/berezin/Data/passwd

#problem 1 - substitute first occurrence of csh on a line with sh
sed 's/csh/sh/' $passwd > sout.1

#problem 2 - substitute 2nd occurrence of string *NP* on a line
# with the string "null field"
sed 's/\*NP\*/null field/2' $passwd > sout.2

#problem 3 - substitute all occurrences of ':' with ';'
sed 's/:/;/g' $passwd > sout.3

#problem 4 - swap the last two fields on a line
sed 's/:\([^:]*\):\([^:]*\)$/:\2:\1/g' $passwd > sout.4

#problem 5  - remove *NP* from the second field
sed 's/^\([^:]*\):\*NP\*:/\1::/' $passwd > sout.5

#problem 6 - remove the contents of the second field (colons stay)
sed 's/^\([^:]*\):[^:]*:/\1::/' $passwd > sout.6

#problem 7 
# if line starts with a t9 then substitute usr2 with usr3
sed '/^t9/s/usr2/usr3/g' $passwd > sout.7

#problem 8
# replace all occurances of /home/mp with /usr/ux
sed 's/\/home\/mp/\/usr\/ux/g' $passwd > sout.8

#problem 9
# remove any periods
sed 's/\.//g' $passwd > sout.9

#problem 10
# indent one tab at all lines that do not begin with t9
sed '/^t9/!s/^/	/g' $passwd > sout.10

#problem 11
# remove the rightmost name or word from 5th field
sed 's/[ ,][^:, ]*:\([^:]*:[^:]*:.*\)$/:\1/' $passwd > sout.11

