#!/bin/bash

for i in `ls *.txt`;
do
  echo  -n $i "  " ;cat $i|wc -l
done
