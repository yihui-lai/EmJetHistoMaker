#ifndef Fakerate_H
#define Fakerate_H

#include <iostream>

double PnTag(double fr[], int nTag);
double PEmergingnTag(double fr[], int nTag, int ijet);
double frCal(int jet_nTrack, int option);
double frCal(int nTrack, double pt, int option);

#endif
