#!/bin/env python
"""Usage: haddsamples.py PARENTDIR
Merges ROOT files contained in PARENTDIR/SUBDIR directories.
Merged ROOT files are saved as PARENTDIR/histo-SUBDIR.root
"""
import sys
import os
import subprocess


use_ahadd=False

def listdir_fullpath(d):
    return [os.path.join(d, f) for f in os.listdir(d)]

import argparse
parser = argparse.ArgumentParser()
parser.add_argument('-f', '--force', action='store_true')
parser.add_argument('directory')
args = parser.parse_args()

dir_to_hadd = args.directory

samplepaths_to_hadd = listdir_fullpath(dir_to_hadd)
samplepaths_to_hadd = [s for s in samplepaths_to_hadd if os.path.isdir(s)] # Only look at directories
samples_to_hadd = [os.path.basename(s.rstrip("/")) for s in samplepaths_to_hadd]
index=0
for s in samples_to_hadd:
    files_to_hadd = listdir_fullpath(os.path.join(dir_to_hadd, s))
    ofilename = 'histo-'+s+'.root'
    ofilepath = os.path.join(dir_to_hadd, ofilename)
    print s
    print ofilename
    print ofilepath
    print files_to_hadd[:5]
    # command = '$EMJETHISTOMAKERDIR/haddnorm/haddnorm eventCountPreTrigger %s %s' % (ofilepath, ' '.join(files_to_hadd[:2]))
    if use_ahadd: haddcommand = 'ahadd.py'
    else        : haddcommand = 'hadd'
    if args.force: haddcommand = haddcommand + ' -f'
    command = '%s %s %s' % (haddcommand, ofilepath, ' '.join(files_to_hadd))
    subprocess.call([command], shell=True)
    # files_to_hadd_concat = " ".join(files_to_hadd)
    index+=1


# subprocess.call()
