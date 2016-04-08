import ROOT as rt
import math
from collections import OrderedDict
from rootutils import getHistos

files = OrderedDict()
# files["QCD_HT100to200"]   = rt.TFile("histo_merged_QCD_HT100to200.root")
# files["QCD_HT200to300"]   = rt.TFile("histo_merged_QCD_HT200to300.root")
# files["QCD_HT300to500"]   = rt.TFile("histo_merged_QCD_HT300to500.root")
# files["QCD_HT500to700"]   = rt.TFile("histo_merged_QCD_HT500to700.root")
# files["QCD_HT700to1000"]  = rt.TFile("histo_merged_QCD_HT700to1000.root")
# files["QCD_HT1000to1500"] = rt.TFile("histo_merged_QCD_HT1000to1500.root")
# files["QCD_HT1500to2000"] = rt.TFile("histo_merged_QCD_HT1500to2000.root")
# files["QCD_HT2000toInf"]  = rt.TFile("histo_merged_QCD_HT2000toInf.root")
files["WJet"]  = rt.TFile("/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/Analysis-20160301-v0/WJetsToLNuInclusive/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/Analysis-20160301/160302_022221/ntuple_merged_WJetsToLNuInclusive.root")

print "%s, %s, %s" % ("name", "efficiency", "uncertainty_efficiency")
for name, f in files.iteritems():
    hist = f.Get("eventCountPreFilter/eventCountPreFilter")
    # f.Get("eventCountPreFilter/eventCountPreFilter").Print()
    eventCountPreFilter = f.Get("eventCountPreFilter/eventCountPreFilter").GetEntries()
    # f.Get("eventCountPostFilter/eventCountPostFilter").Print()
    eventCountPostFilter = f.Get("eventCountPostFilter/eventCountPostFilter").GetEntries()

    efficiency = eventCountPostFilter / eventCountPreFilter
    err_efficiency = math.sqrt(eventCountPostFilter) / eventCountPreFilter
    print "%s, %e, %e" % (name, efficiency, err_efficiency)

