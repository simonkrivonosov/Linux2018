#!/bin/bash
ext_split=$(echo $1 | tr " " "\n")
if [ "$1" = "-info" ]
then echo "[necessary file extensions in  \" \" through space][catalogue name][tar name]"
fi
mkdir $2
for ext in $ext_split
do
    for file in $(find /Users/simon/Desktop/go -name "*$ext" > /dev/null)
    do
    file_name=$(echo `expr "$file" : '.*/\([a-z]*\.[a-z]*\)'`)
    count=0
    while [[ $(find ./$2 -name "$file_name" ) > /dev/null ]]
    do
    count_next=$(($count+1))
    if [ $count -ge 1 ]
    then
    file_name=$(echo ${file_name/$count./$count_next.})
    count=$(($count+1))
    else
    file_name=$(echo ${file_name/./$count_next.})
    count=$(($count+1))
    fi
    done
    cp $file "./$2/$file_name"
    done
done
tar -cf $3.tar ./$2 > /dev/null
echo "done"
