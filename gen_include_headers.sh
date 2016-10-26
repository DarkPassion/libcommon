#!/bin/bash

# generate output headers

BASE_DIR="$(pwd)/include"
SRC_DIR="$(pwd)/src"

rm -rf $BASE_DIR

if [ ! -r $BASE_DIR ]
then
    echo " create ${BASE_DIR}";
    mkdir $BASE_DIR; 
fi


DIR_LIST=`ls $SRC_DIR/`;

for dir in $DIR_LIST
do

    if [ -d $SRC_DIR/$dir ]
    then
    mkdir $BASE_DIR/$dir
    cp $SRC_DIR/$dir/*.h $BASE_DIR/$dir/
    fi
 
done


echo "Done!"

