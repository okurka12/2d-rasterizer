#! /bin/sh

# This script compares two .bmp files and stores the comparison in cmp.txt

# python executable
PY=py

# files to compare
FILE1=funkcni.bmp
FILE2=nefunkcni.bmp


$PY myhex.py $FILE1 > $FILE1-hex.txt
$PY myhex.py $FILE2 > $FILE2-hex.txt
$PY cmp.py $FILE1-hex.txt $FILE2-hex.txt > cmp.txt
rm $FILE1-hex.txt $FILE2-hex.txt
