#!/bin/bash
DIRNAME=$1
ARCHIVENAME=$2
mkdir $ARCHIVENAME > /dev/null 2>&1
if [[ "$?" = "1" ]]
then
    echo "error"
    exit 1
fi
ext_split=$(echo $3 | tr " " "\n" 2> /dev/null)
if [[ "$?" = "1" ]]
then
    echo "error"
    exit 1
fi
if [[ $1 = "-info" ]]
then
    echo "[necessary file extensions in  \" \" through space][catalogue name][tar name][owner name][group name]"
fi
for ext in $ext_split
do
    maxd=1
    mind=1
while [[ $(find $DIRNAME -name "*$ext" -mindepth $mind 2> /dev/null) != "" ]]
        do
            if [[ "$?" = "1" ]]
            then
                echo "error"
                exit 1
            fi
            for file in $(find $DIRNAME -mindepth $mind -maxdepth $maxd -name "*$ext" 2> /dev/null)
            do
                if [[ "$?" = "1" ]]
                then
                    echo "error"
                    exit 1
                fi
                file_path=${file%/*.*}
                cur_path=$ARCHIVENAME
                for directory in $(echo $file_path | tr "/" " " 2> /dev/null)
                do
                    if [[ "$?" = "1" ]]
                    then
                        echo "error"
                        exit 1
                    fi
                    cur_path="$cur_path/$directory"
                    if [ ! -e $cur_path ]
                    then
                        mkdir $cur_path > /dev/null 2>&1
                        if [[ "$?" = "1" ]]
                        then
                            echo "error"
                            exit 1
                        fi
                    fi
                done
                cp $file "$cur_path/" > /dev/null 2>&1
                if [[ "$?" = "1" ]]
                then
                    echo "error"
                    exit 1
                fi
            done
            ((maxd++))
            ((mind++))
        done
done
tar -cf $ARCHIVENAME.tar $ARCHIVENAME > /dev/null 2>&1
if [[ "$?" = "1" ]]
then
    echo "error"
    exit 1
fi
rm -rf $ARCHIVENAME
if [[ "$?" = "1" ]]
then
    echo "error"
    exit 1
fi
if [ ! -z "$4" ]
then
    chown $4 $ARCHIVENAME.tar > /dev/null 2>&1
    if [[ "$?" = "1" ]]
    then
        echo "error"
        exit 1
    fi
fi
if [ ! -z "$5" ]
then
    chown :$5 $ARCHIVENAME.tar > /dev/null 2>&1
    if [[ "$?" = "1" ]]
    then
        echo "error"
        exit 1
    fi
fi
echo "done"
