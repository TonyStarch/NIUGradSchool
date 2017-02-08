#!/bin/csh

if($#argv == 0) then
    echo "Enter a time in seconds:"
    set seconds = $<
else
    set seconds = $1
endif
echo $seconds | egrep "^[0-9]+" >& /dev/null

if($status != 0 ) then
    echo "That was an invalid time!"
    exit 1
else
    echo $seconds "seconds are equivalent to:"
endif

@ seconds_remainder = $seconds % 60
@ minutes = $seconds / 60
@ minutes_remainder = $minutes % 60
@ hours = $minutes / 60
@ hours_remainder = $hours % 24
@ days = $hours / 24

echo $seconds_remainder " seconds"
echo $minutes_remainder " minutes"
echo $hours_remainder " hours"
echo $days " days"
