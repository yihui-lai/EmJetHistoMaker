#!/bin/bash
make clean
make && ./main
rm outputs/histo-QCD.root
hadd -f outputs/histo-QCD.root outputs/histo-QCD_*
