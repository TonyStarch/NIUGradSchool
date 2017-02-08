#!/bin/csh

if($#argv) then
    set UserNum = $1
else
    echo -n "Enter a number:"
    set UserNum = $<
endif

echo UserNum | grep "^[0-9]*" >& /dev/null
if($status) then
    echo "Invalid Number"
    exit 1
else
    echo "Valid Number"
    exit 0
endif
