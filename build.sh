#!/bin/bash

STARTINGDIR=${PWD}
echo $STARTINGDIR
# cd cogFiles
PYTHONPATH=${PWD}/cogFiles
echo "WARNING: Replacing EmJetHistoMaker.h"
cog.py -r ${STARTINGDIR}/EmJetHistoMaker.h
# make clean; make
cd $STARTINGDIR
