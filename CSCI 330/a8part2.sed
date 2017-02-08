#!/bin/sh

webpage=/home/max/berezin/Data/sed.data

######################################
# Assignment 8 - Sed 
# Brandon Tweed
# Section 2
# Instructor: Berezinski
# TA: Muhtaris
######################################

# remove any spaces before or after tags
sed -e 's/ *</</g' -e 's/> */>/g' $webpage > p1.out

# replace <p> marker with a blank line
sed -e 's/<p>/\
\
/g' -e 's/<P>/\
\
/g' p1.out > p2.out

# eliminate </p> marker
sed -e 's/<\/p>//g' -e 's/<\/P>//g' p2.out > p3.out

# break at <br> as long as it's not EOL
sed -e s'/<br>$//' -e 's/<br>/\
/g' p3.out > p4.out

# double space before <LI> and replace with asterisk and space
sed -e 's/<li>/\
\
* /g' -e 's/<Li>/\
\
* /g' -e 's/<lI>/\
\
* /g' -e 's/<LI>/\
\
* /g' p4.out > p5.out

# replace <b> and </b> with double quotes
sed -e 's/<b>/ "/g' -e 's/<B>/ "/g' -e 's/<\/b>/" /g' -e 's/<\/B>/" /g' p5.out > p6.out

# break any lines longer than 79 characters into a set of lines
# around 60 characters long
sed -e '/^.\{80,\}/{
s/\(.\{60\}[^ ]* \)/\1\
/
P
D
}' p6.out > p7.out

# Indent all lines between the <ul> and </ul> markers
# put a blank line at each one

# first, make all <ul> tags look like this: <UL>
sed -e 's/ul>/UL>/g' -e 's/Ul>/UL>/g' -e 's/uL>/UL>/g' p7.out > p8a.out

# put <UL> and </UL> tags on lines by themselves
sed -e 's/<UL>/\
<UL>\
/g' -e 's/<\/UL>/\
<\/UL>\
/g' p8a.out > p8b.out 

# for all lines between the <UL> and </UL>
# indent the line

sed -e '/<UL>/,/<\/UL>/{
/<UL>/!s/^/\	/
}' p8b.out > p8c.out

# now remove the <UL> and the </UL>
sed -e 's/<UL>//g' -e 's/<\/UL>//g' p8c.out > p8.out

# echo Removing <hr> marker ..
# Replace the <hr> marker with a line of stars on their own line
# **************************************************************

sed -e 's/<hr>/\
\
****************************************************************\
\
/' -e 's/<HR>/\
\
****************************************************************\
\
/' -e 's/<hR>/\
\
****************************************************************\
\
/' -e 's/<Hr>/\
\
****************************************************************\
\
/' p8.out > p9.out

# Remove any remaining HTML tags
sed -e 's/<[^>]*>//g' p9.out > p10.out

# Replace all occurances of less than and greater than
sed -e 's/\&lt;/</g' -e 's/\&gt;/>/g' p10.out > p11.out

# solution file is sout.12, make it by copying p11.out
cp p11.out sout.12

# clean up temporary files
rm p?.out
rm p??.out

