#!/bin/bash

set -ex

for f in $(find . -name '*.c' -or -name '*.cpp' -or -name "*.h" -type f)
do
    astyle --style=bsd $f
done


# after formate the code,we need to rm '*.orig' files
for f in $(find . -name '*.orig' -type f)
do
     rm $f
done






