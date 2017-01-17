#include "HistoMakerBase.h"
#include "EmJetHistos.h"
#include "LumiReWeightingStandAlone.h"

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
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>

using std::string;
using std::vector;
using std::unique_ptr;

class EmJetHistos;
typedef EmJetHistos Histos;
const int nTrackSort=50;
enum class Sample {SIGNAL, QCD, WJET};

class EmJetHistoMaker : public HistoMakerBase
{
 public:
  EmJetHistoMaker();
  ~EmJetHistoMaker() {};
  int TRACKSOURCE;
  int VERTEXSOURCE;
  void InitHistograms();
  void FillHistograms    (long eventnumber);
  void FillEventHistograms  (long eventnumber, string tag);
  void FillJetHistograms    (long eventnumber, int ij, string tag);
  void FillPileupHistograms (long eventnumber, string tag);
  void PrintEvent (long eventnumber, string comment);
  int SetTree(string ifilename);
  void SetOptions(Sample sample=Sample::SIGNAL, bool isData=false, double xsec=1.0, double efficiency=1.0, bool isSignal=false, bool pileupOnly=false);
 private:
  double CalculateEventWeight(long eventnumber);
  bool SelectJet(int jet_index);
  unique_ptr<Histos> histo_;
  unique_ptr<TH1F> histo_nTrueInt_;
  Sample sample_;
  bool isData_;
  double xsec_;
  double efficiency_;
  bool isSignal_;
  bool pileupOnly_;
  unique_ptr<reweight::LumiReWeighting> LumiWeights_;
};

EmJetHistoMaker::EmJetHistoMaker()
{
  TRACKSOURCE = 0;
  // Initialize lumi reweighting utility
  {
    std::string mcFile   = "~/www/2016-03-23/pileup_mc_2015_25ns_Startup_PoissonOOTPU.root";
    std::string dataFile = "~/www/2016-03-21/pileup-DataSkim-20160302.root";
    std::string mcHist   = "nTrueInt";
    std::string dataHist = "pileup";
    LumiWeights_ = unique_ptr<reweight::LumiReWeighting>(new reweight::LumiReWeighting(mcFile, dataFile, mcHist, dataHist));
  }
}

int EmJetHistoMaker::SetTree(std::string filename)
{
  TFile *f = new TFile(filename.c_str());
  // File read error
  if (f->IsZombie()) {
    std::cout << "File read error for file: " << filename << std::endl;
    return 1;
  }
  TDirectory * dir = (TDirectory*)f->Get((filename+":/emJetAnalyzer").c_str());
  dir->GetObject("emJetTree",tree_);
  // tree_->Print();
  Init(tree_);
  return 0; // No error
}

void EmJetHistoMaker::InitHistograms()
{
  TH1::SetDefaultSumw2();
  histo_nTrueInt_ = unique_ptr<TH1F>(new TH1F("nTrueInt", "nTrueInt", 100, 0., 100.));
  if (!pileupOnly_) {
    histo_ = unique_ptr<Histos>(new Histos());
  }
}

void EmJetHistoMaker::FillHistograms(long eventnumber)
{
  FillPileupHistograms(eventnumber, "");
  if (pileupOnly_) return;

  FillEventHistograms(eventnumber, "");
  // Event cut 1
  // FillEventHistograms(eventnumber, "/EVTCUT1");
  // Event cut 2
  // FillEventHistograms(eventnumber, "/EVTCUT2");

  // Fill cut flow histogram
  {
    double w = CalculateEventWeight(eventnumber);

    double ht4 = (*jet_pt)[0] + (*jet_pt)[1] + (*jet_pt)[2] + (*jet_pt)[3];
    histo_->hist1d["ht4"]->Fill(ht4, w);
    int nJet = 0;
    for (unsigned ij = 0; ij < jet_pt->size(); ij++) {
      nJet++;
      // if ( TMath::Abs((*jet_eta)[ij]) < 2.0 ) nJet++;
      // if ( (*jet_nhf)[ij] < 0.99 && (*jet_nef)[ij] < 0.99 && (*jet_cef)[ij] < 0.99 ) nJet++;
    }
    int nJetPassingCut = 0;
    for (unsigned ij = 0; ij < jet_pt->size(); ij++) {
      if ( SelectJet(ij) ) nJetPassingCut++;
    }
    // std::cout << event << "\t" << (*jet_pt)[0] << "\t" << (*jet_pt)[1] << "\t" << (*jet_pt)[2] << "\t" << (*jet_pt)[3] << "\t" << ht4 << std::endl;

    const int nCut = 9;
    bool cuts[nCut];
    cuts[0] = true                             ;
    cuts[1] = nJet >= 4                        ;
    cuts[2] = cuts[2-1] && ht4 > 1000          ;
    cuts[3] = cuts[3-1] && (*jet_pt)[0] > 400  ;
    cuts[4] = cuts[4-1] && (*jet_pt)[1] > 200  ;
    cuts[5] = cuts[5-1] && (*jet_pt)[2] > 125  ;
    cuts[6] = cuts[6-1] && (*jet_pt)[3] > 50   ;
    cuts[7] = cuts[7-1] && nJetPassingCut >= 1 ;
    cuts[8] = cuts[8-1] && nJetPassingCut >= 2 ;
    for (int ic = 0; ic < nCut; ic ++) {
      if ( cuts[ic] ) histo_->hist1d["cutflow"]->Fill(ic, w);
    }
  }

}

void EmJetHistoMaker::FillEventHistograms(long eventnumber, string tag)
{
  double w = CalculateEventWeight(eventnumber);

  // Jet loop
  for (unsigned ij = 0; ij < (*jet_pt).size(); ij++) {
    FillJetHistograms(eventnumber, ij, tag+"");
    // Jet cut 1
    // FillJetHistograms(eventnumber, "/JETCUT1");
    // Jet cut 2
    // FillJetHistograms(eventnumber, "/JETCUT2");
  }
}

double EmJetHistoMaker::CalculateEventWeight(long eventnumber)
{
  double weight = 0.0;
  if (isData_) weight = 1.0;
  else {
    weight = 1.0;
    double generator_weight = xsec_*efficiency_/nentries_;
    weight *= generator_weight;
    // double pileup_lumi_weight = LumiWeights_->weight(nTrueInt);
    // weight *= pileup_lumi_weight;
  }
  return weight;
}

bool EmJetHistoMaker::SelectJet(int ij)
{
  if (!(ij < 4)) return false;
  int nTrackPassingCut = 0;
  for (unsigned itk = 0; itk < (*track_pt)[ij].size(); itk++) {
    if( (*track_source)[ij][itk] == 0 ) {
      if( (*track_pt)[ij][itk] > 1.0 ) {
        nTrackPassingCut++;
      }
    }
  }

  bool result = true;
  bool cut_nef = (*jet_nef)[ij] < 0.9             ; result = result && cut_nef      ;
  bool cut_cef = (*jet_cef)[ij] < 0.9             ; result = result && cut_cef      ;
  bool cut_alphaMax = (*jet_alphaMax)[ij] < 0.2   ; result = result && cut_alphaMax ;
  bool cut_tracks = nTrackPassingCut > 0          ; result = result && cut_tracks   ;
  return result;
}

void EmJetHistoMaker::FillJetHistograms(long eventnumber, int ij, string tag)
{
  double w = CalculateEventWeight(eventnumber);
  histo_->hist1d["jet_pt"]->Fill((*jet_pt)[ij], w);
}

void EmJetHistoMaker::FillPileupHistograms(long eventnumber, string tag)
{
  double weight = CalculateEventWeight(eventnumber);
  histo_nTrueInt_->Fill(nTrueInt, weight);
}

void EmJetHistoMaker::PrintEvent (long eventnumber, string comment)
{
  std::cout << "run,lumi,event,comment: " << run << ", " << lumi << ", " << event << ", " << comment << "\n";
}

void
EmJetHistoMaker::SetOptions(Sample sample, bool isData, double xsec, double efficiency, bool isSignal, bool pileupOnly)
{
  sample_ = sample;
  isData_ = isData;
  xsec_ = xsec;
  efficiency_ = efficiency;
  isSignal_ = isSignal;
  pileupOnly_ = pileupOnly;
}

