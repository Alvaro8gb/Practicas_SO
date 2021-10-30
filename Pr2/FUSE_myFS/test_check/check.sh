#!/bin/bash
# /home/usuarioso/Desktop/Practicas_SO/Pr2/FUSE_myFS/test_check

echo Executing check.sh .....

cd ..

MPOINT="./mount-point/"
temp_path='./temp/'
src='./src/'

file1='fuseLib.c'
file2='myFS.h'
file3='file3.dat'

mkdir $temp_path

# A

echo copy $file1 and $file2

cp $src$file1 $src$file2 $temp_path

pwd

ls $temp_path

cp $src$file1 $src$file2 $MPOINT

# B

echo diff between copys : 

diff $temp_path$file1 $MPOINT'fuseLib.c'
diff $temp_path$file2 $MPOINT'myFS.h'

truncate  -o -s -1 $temp_path$file1
truncate  -o -s -1 $MPOINT$file1

# C

echo diff lines between truncates $file1 : 

diff $src$file1 $MPOINT$file1 | wc -l
diff $src$file1 $temp_path$file1 | wc -l

# D

echo generate a random file : $file3 

head '/dev/urandom' -c 4096 > $temp_path$file3

cp $temp_path$file3 $MPOINT

# E

echo dif between file3 copys :

diff $temp_path$file3 $MPOINT$file3 

# F

echo truncate $file2 one more block

truncate  -o -s +1 $temp_path$file2
truncate  -o -s +1 $MPOINT$file2

# G

echo diff lines between truncates $file2 : 

diff $src$file1 $MPOINT$file2 | wc -l
diff $src$file1 $temp_path$file2 | wc -l

echo End
rm -r $temp_path


