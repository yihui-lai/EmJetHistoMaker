#include "Looper.h"

#include <string>
#include <map>

struct File {
  std::string name;
  double xsec; // in pb
  double efficiency;
  double error_efficiency;
};
// std::string name("/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/QCD_HT700to1000/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/SelectedJetAnalysis/160121_151706/output_merged_QCD_HT700to1000.root"   );
// File file = {name , 1.0};
// std::string label = "QCD_HT700to1000";

std::map<std::string, File> files = {
  {"ModelA",  { "/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/ModelA/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/SelectedJetAnalysis/160107_220503/output_merged_ModelA.root", 0.010, 1.0, 0.0 } },
  {"ModelB",  { "/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/ModelB/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/SelectedJetAnalysis/160107_220526/output_merged_ModelB.root", 0.010, 1.0, 0.0 } },
  // {"WJet",  "/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/WJetSkimMuon/SingleMuon/SelectedJetAnalysis/160120_160809/output_merged_WJetSkimMuon.root"},
  // { "QCD_HT100to200"   ,  { "/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/QCD_HT100to200/QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/SelectedJetAnalysis/160121_151540/output_merged_QCD_HT100to200.root"       , 27540000 , 0.000000e+00 , 0.000000e+00 }  },
  // { "QCD_HT200to300"   ,  { "/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/QCD_HT200to300/QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/SelectedJetAnalysis/160121_151605/output_merged_QCD_HT200to300.root"       , 1735000  , 0.000000e+00 , 0.000000e+00 }  },
  // { "QCD_HT300to500"   ,  { "/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/QCD_HT300to500/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/SelectedJetAnalysis/160121_151642/output_merged_QCD_HT300to500.root"       , 366800   , 0.000000e+00 , 0.000000e+00 }  },
  { "QCD_HT700to1000"  ,  { "/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/QCD_HT700to1000/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/SelectedJetAnalysis/160121_151706/output_merged_QCD_HT700to1000.root"    , 6524     , 4.155094e-03 , 2.008417e-05 }  },
  { "QCD_HT1000to1500" ,  { "/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/QCD_HT1000to1500/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/SelectedJetAnalysis/160121_151740/output_merged_QCD_HT1000to1500.root" , 1064     , 1.706836e-01 , 4.733912e-04 }  },
  { "QCD_HT1500to2000" ,  { "/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/QCD_HT1500to2000/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/SelectedJetAnalysis/160121_151808/output_merged_QCD_HT1500to2000.root" , 121.5    , 4.209039e-01 , 7.314915e-04 }  },
  { "QCD_HT2000toInf"  ,  { "/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/QCD_HT2000toInf/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/SelectedJetAnalysis/160121_151829/output_merged_QCD_HT2000toInf.root"    , 25.45    , 4.695370e-01 , 8.632537e-04 }  },
};

int main(int argc, char *argv[])
{
  Looper t;
  // t.InitFromFileName( "/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/ModelA/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/SelectedJetAnalysis/160107_220503/output_merged_ModelA.root");
  // t.Loop("histo-ModelA.root");
  // t.InitFromFileName( "/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/ModelB/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/SelectedJetAnalysis/160107_220526/output_merged_ModelB.root");
  // t.Loop("histo-ModelB.root");
  // t.InitFromFileName("/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/WJetSkimMuon/SingleMuon/SelectedJetAnalysis/160120_160809/output_merged_WJetSkimMuon.root");
  // t.Loop("histo-WJet.root");
  // t.InitFromFileName("/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/QCD_HT1000to1500/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/SelectedJetAnalysis/160121_151740/output_merged_QCD_HT1000to1500.root" ) ;
  // t.InitFromFileName("/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/QCD_HT100to200/QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/SelectedJetAnalysis/160121_151540/output_merged_QCD_HT100to200.root"       ) ;
  // t.InitFromFileName("/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/QCD_HT1500to2000/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/SelectedJetAnalysis/160121_151808/output_merged_QCD_HT1500to2000.root" ) ;
  // t.InitFromFileName("/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/QCD_HT2000toInf/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/SelectedJetAnalysis/160121_151829/output_merged_QCD_HT2000toInf.root"    ) ;
  // t.InitFromFileName("/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/QCD_HT200to300/QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/SelectedJetAnalysis/160121_151605/output_merged_QCD_HT200to300.root"       ) ;
  // t.InitFromFileName("/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/QCD_HT300to500/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/SelectedJetAnalysis/160121_151642/output_merged_QCD_HT300to500.root"       ) ;
  // t.InitFromFileName("/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/SelectedJetAnalysis-v0/QCD_HT700to1000/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/SelectedJetAnalysis/160121_151706/output_merged_QCD_HT700to1000.root"    ) ;
  std::string prefix = "outputs/histo-";
  std::string postfix = ".root";
  for (auto kv: files) {
    auto label = kv.first;
    auto file = kv.second;
    t.InitFromFileName(file.name, file.xsec, file.efficiency);
    t.Loop(prefix + label + postfix);
  }

}
