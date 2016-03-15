from math import *
nBins =20
xMin = -3.
xMax = +2.

delta = xMax - xMin
binWidth = delta/nBins
binLowEdges = [(xMin + binWidth * i) for i in xrange(nBins+1)]
print binLowEdges

bins = [10**x for x in binLowEdges]
print bins


