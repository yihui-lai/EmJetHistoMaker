import ROOT as rt
rt.TH1.SetDefaultSumw2()
import math
from collections import OrderedDict
from rootutils import getTrees
files= OrderedDict()
# files['WJet'] = "/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/WJetSkimMuon/SingleMuon/SelectedJetAnalysis/160107_215042/output_merged_WJetSkimMuon.root"
# files['ModelA'] = "/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/ModelA/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/SelectedJetAnalysis/160107_220503/output_merged_ModelA.root"
# files['ModelB'] = "/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/ModelB/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/SelectedJetAnalysis/160107_220526/output_merged_ModelB.root"
# files['MC'] = "/afs/cern.ch/user/y/yoshin/CMSSW_7_4_12/src/EmergingJetAnalysis/ntuple.root"
files['ModelA_Analysis-20160314'] = "/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/Analysis-20160314-v0/ModelA/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/Analysis-20160314/160314_234710/ntuple_merged_ModelA.root"
treename = 'emergingJetsTree'
classname = 'BaseClass'

index = 0
for k,v in files.iteritems():
    name = k
    filename = v
    f = rt.TFile(filename)
    trees = getTrees(f)
    # if len(trees) != 1: print('Warning: More than one TTree in file. Calling MakeClass on the first instance.')
    # tree = trees[0]
    for tree in trees:
        # print tree.GetName()
        if tree.GetName()==treename: break
    tree.MakeClass(classname)
    # raw_input("Press Enter to continue...")

# raw_input("Press Enter to continue...")

import os
os.rename(classname + '.C', classname + '.cc')

