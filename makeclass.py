import ROOT as rt
rt.TH1.SetDefaultSumw2()
import math
from collections import OrderedDict
from rootutils import getTrees
files= OrderedDict()
# files['WJet'] = "/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/WJetSkimMuon/SingleMuon/SelectedJetAnalysis/160107_215042/output_merged_WJetSkimMuon.root"
# files['ModelA'] = "/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/ModelA/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/SelectedJetAnalysis/160107_220503/output_merged_ModelA.root"
# files['ModelB'] = "/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/ModelB/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/SelectedJetAnalysis/160107_220526/output_merged_ModelB.root"
files['MC'] = "/afs/cern.ch/user/y/yoshin/CMSSW_7_4_12/src/EmergingJetAnalysis/ntuple.root"
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

# Add appropriate 'using' directives
import fileinput
classheader = classname + '.h'
using_vector = 0
for line in fileinput.input(classheader, inplace=1):
    print line,
    if using_vector==0 and line.startswith('#include "vector"'):
        using_vector = 1
        print 'using std::vector;'

