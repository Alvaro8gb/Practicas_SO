#! /bin/bash
echo Executing myTar...

file_name='mytar'
temp_name='temp'
tar_file='filetar.mtar'

if [[ -f $file_name && -x $file_name ]]; then
echo
#file mytar
else
echo "No, existe el fichero $file_name o no es ejecutable"
exit 1
fi

if [ -d $temp_name ]; then
rm -r $temp_name
fi

mkdir $temp_name
cd $temp_name

echo 'Hello world!' > 'file1.txt'
head '/etc/passwd' -n 10 > 'file2.txt'
head '/dev/urandom' -c 1024 > 'file3.dat'

../mytar -c -f $tar_file file1.txt file2.txt file3.dat

if [ $? -eq 1 ]; then
echo Fail to create $tar_file
exit 1
fi

mkdir out 

cp filetar.mtar ./out

cd out 

../../mytar -x -f $tar_file

if [ $? -eq 1 ]; then
echo Fail to extract $tar_file
exit 1
fi

diff ../file1.txt file1.txt > difs.txt
diff ../file2.txt file2.txt >> difs.txt
diff ../file3.dat file3.dat >> difs.txt

output=$(cat difs.txt)


if [ $output =="" ]; then
cd ../../
echo Correct! 
rm -r  $temp_name
exit 0
else 
echo Difs  : $output
exit 1
fi


