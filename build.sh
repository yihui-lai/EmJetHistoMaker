#!/bin/bash

STARTINGDIR=${PWD}
echo $STARTINGDIR
# cd cogFiles
PYTHONPATH=${PWD}/cogFiles
echo "WARNING: Replacing EmJetHistos.h"
cog.py -r ${STARTINGDIR}/EmJetHistos.h
make clean; make
cd $STARTINGDIR
