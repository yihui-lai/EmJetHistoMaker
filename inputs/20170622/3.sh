#!/bin/bash

for file in `ls *.txt`
do
    echo -n $file "  "
    A=`cat $file |wc -l`

    if [[ $file == "SinglePhoton"* ]];then 
      echo "$(($A*5))"
    else
      echo $A
    fi
done
