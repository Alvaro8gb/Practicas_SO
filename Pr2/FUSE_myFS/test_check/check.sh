#!/bin/bash

echo Executing check.sh .....

cd ..

temp_path='./mount-pount/temp/'

mkdir $temp_path

cp fuseLib.c $temp_path
cp myFS.h $temp_path



