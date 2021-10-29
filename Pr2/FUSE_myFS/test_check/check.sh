#!/bin/bash

echo Executing check.sh .....

cd ..

MPOINT="./mount-point/"
temp_path='./temp/'

mkdir $temp_path

file1='./src/fuseLib.c'
file2='./src/myFS.h'

cp $file1 $file2 $temp_path

pwd
ls $temp_path

cp $file1 $file2 $MPOINT

echo diff between copys : 

diff $file1 $MPOINT'fuseLib.c'
diff $file2 $MPOINT'myFS.h'

echo End

rm -r $temp_path


