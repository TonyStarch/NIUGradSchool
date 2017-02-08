#!/bin/csh

if ($#argv) then
   echo "You gave this script an argument"
   echo "The argument was: $1"
else
   echo "You didn't give an argument "
endif
