// Base class for program to loop over single file and fill histograms with arbitrary weights
#include "BaseClass.h"
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
#include <algorithm>
#include <functional>

using std::string;

class HistoMakerBase : BaseClass
{
  public:
    HistoMakerBase() {};
    virtual void OpenOutputFile (string ofilename);
    virtual void InitHistograms() = 0;
    virtual void FillHistograms(long eventnumber) = 0;
    virtual void WriteHistograms();
    virtual int SetTree(string ifilename) = 0;
    void LoopOverCurrentTree ();

  private:
    int reportEvery_ = 10000;
    TFile* ofile_;
    TStopwatch timer_total_;
    TStopwatch timer_;
};

// Copied from BaseClass::Loop()
void HistoMakerBase::LoopOverCurrentTree()
{
  if (fChain == 0) {
    std::cout << "Invalid tree!" << std::endl;
    return;
  }
  Long64_t nentries = fChain->GetEntriesFast();
  if ( nentries == 0 ) {
    std::cout << "No entries!" << std::endl;
    return;
  }
  else {
    std::cout << "Number of entries: " << nentries << std::endl;
  }

  timer_total_.Start();
  // Loop over all events in TChain
  for (Long64_t jentry = 0; jentry < nentries; jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;

    // Do stuff with one entry of the TTree
    if ( jentry % reportEvery_ == 0 ) {
      if (jentry!=0) std::cout << "Chunk processing time (s): " << timer_.RealTime() << std::endl;
      timer_.Start();
      std::cout << "Running over global entry: " << jentry << std::endl;
    }
    // Fill histograms
    FillHistograms(jentry);
  }
  double total_time_elapsed = timer_total_.RealTime();
  std::cout << "Total processing time (s): " << total_time_elapsed << std::endl;
  std::cout << std::endl;
}

void HistoMakerBase::OpenOutputFile(string ofilename)
{
  ofile_ = new TFile(ofilename.c_str(), "RECREATE");
}

void HistoMakerBase::WriteHistograms(Long64_t entry)
{
  ofile->Write();
}
