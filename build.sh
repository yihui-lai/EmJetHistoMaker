#!/bin/bash
status=0

STARTINGDIR=${PWD}
echo $STARTINGDIR
# cd cogFiles
PYTHONPATH=${PWD}/cogFiles
echo "WARNING: Replacing EmJetHistos.h"
cog.py -r ${STARTINGDIR}/EmJetHistos.h
make clean; make
status=$?
# if [ $? -ne 0 ] # If previous command was not successful
# then
#     status=1
# fi
cd $STARTINGDIR
exit $status
