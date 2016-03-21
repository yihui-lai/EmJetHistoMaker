#include "HistoMakerBase.h"
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TStopwatch.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TMath.h>

#include <cassert>
#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <functional>

using std::string;
using std::vector;

class EmJetHistos;

class EmJetHistoMaker : HistoMakerBase
{
 public:
  EmJetHistoMaker();
  void InitHistograms();
  void FillHistograms(long eventnumber) = 0;
  int SetTree();
 private:
  EmJetHistos histo;
  std::vector<TH1F*> histos1D;
  std::vector<TH2F*> histos2D;
  Sample sample;
  bool isData;
  double xsec;
  double efficiency;
  bool isSignal;
};

int EmJetHistoMaker::SetTree(std::string filename)
{
  TFile *f = new TFile(filename.c_str());
  // File read error
  if (f->IsZombie()) {
    std::cout << "File read error for file: " << filename << std::endl;
    return 1;
  }
  TDirectory * dir = (TDirectory*)f->Get((filename+":/emergingJetAnalyzer").c_str());
  dir->GetObject("emergingJetsTree",tree);
  Init(tree);
  return 0; // No error
}

void EmJetHistoMaker::InitHistograms()
{
  EmJetHistos.Init();
}

const int nBins_vertex_Lxy = 20;
const float bins_vertex_Lxy[nBins_vertex_Lxy+1] = {0.001, 0.0017782794100389228, 0.0031622776601683794, 0.005623413251903491, 0.01, 0.01778279410038923, 0.03162277660168379, 0.05623413251903491, 0.1, 0.1778279410038923, 0.31622776601683794, 0.5623413251903491, 1.0, 1.7782794100389228, 3.1622776601683795, 5.623413251903491, 10.0, 17.78279410038923, 31.622776601683793, 56.23413251903491, 100.0};

class EmJetHistos
{
 public:
  EmJetHistos();
  ~EmJetHistos();
  /*[[[cog
    import emjet_histo_code as mod
    mod.generate_vbin_decl()
    mod.generate_histo_decl()
    mod.generate_histo_vector_decl()
    ]]]*/
  const nBins_vertex_Lxy = 100; bins_vertex_Lxy = { 0.001, 0.0017782794100389228, 0.0031622776601683794, 0.005623413251903491, 0.01, 0.017782794100389229, 0.031622776601683791, 0.056234132519034911, 0.10000000000000001, 0.17782794100389229, 0.31622776601683794, 0.56234132519034907, 1.0, 1.7782794100389228, 3.1622776601683795, 5.6234132519034912, 10.0, 17.782794100389228, 31.622776601683793, 56.234132519034908, 100.0 };
  TH1F* jet_pt;
  TH1F* vertex_Lxy;
  vector<TH1F*> jet_pt_sorted_by_pt;
  //[[[end]]]
};

EmJetHistos::EmJetHistos()
{
  /*[[[cog
    mod.generate_histo_init()
    mod.generate_histo_vector_init()
    ]]]*/
  jet_pt = new TH1F("jet_pt", "jet_pt" , 100, 0, 1000.0);
  vertex_Lxy = new TH1F("vertex_Lxy", "vertex_Lxy" , 100, nBins_vertex_Lxy, bins_vertex_Lxy);
  {
    auto jet_pt_0 = new TH1F("jet_pt_0", "jet_pt_0" , 100, 0, 1000.0);
    jet_pt_sorted_by_pt.push_back(jet_pt_0);
    auto jet_pt_1 = new TH1F("jet_pt_1", "jet_pt_1" , 100, 0, 1000.0);
    jet_pt_sorted_by_pt.push_back(jet_pt_1);
    auto jet_pt_2 = new TH1F("jet_pt_2", "jet_pt_2" , 100, 0, 1000.0);
    jet_pt_sorted_by_pt.push_back(jet_pt_2);
    auto jet_pt_3 = new TH1F("jet_pt_3", "jet_pt_3" , 100, 0, 1000.0);
    jet_pt_sorted_by_pt.push_back(jet_pt_3);
  }
  //[[[end]]]
}
EmJetHistos::~EmJetHistos()
{
  /*[[[cog
    mod.generate_histo_dest()
    mod.generate_histo_vector_dest()
    ]]]*/
  delete jet_pt;
  delete vertex_Lxy;
  for (auto i: jet_pt_sorted_by_pt) { delete i; }
  jet_pt_sorted_by_pt.clear();
  //[[[end]]]
}
