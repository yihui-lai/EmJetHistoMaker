#!/bin/bash

for dirc in `ls /store/user/yofeng/80X/ntuple/Analysis-20170621-v0`; do
    echo $dirc
    find /store/user/yofeng/80X/ntuple/Analysis-20170621-v0/${dirc} -name "ntuple*.root" -exec ls {} \; > ${dirc}.txt 
done
