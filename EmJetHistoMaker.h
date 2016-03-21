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

  TH1F* jet_pt;
  TH1F* vertex_Lxy;
  TH1F* jet_pt0;
  TH1F* jet_pt1;
  TH1F* jet_pt2;
  TH1F* jet_pt3;
};

EmJetHistos::EmJetHistos()
{
  jet_pt = new TH1F("jet_pt", "jet_pt", 100, 0., 100.);
  vertex_Lxy = new TH1F("vertex_Lxy", "vertex_Lxy", nBins_vertex_Lxy, bins_vertex_Lxy);
}
EmJetHistos::~EmJetHistos()
{
  delete jet_pt;
  delete vertex_Lxy;
}
