#include "BaseClass.h"
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TStopwatch.h>
#include <TH1F.h>
#include <TH2F.h>

#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <functional>

using std::string;

class Looper : BaseClass 
{
  public:
    Looper();
    void InitFromFileName(const std::string ifilename, double iweight=1.0, double iefficiency=1.0);
    void Loop(string ofilename);
  private:
    std::unordered_map<std::string, TH1F*> histos1D;
    std::unordered_map<std::string, TH2F*> histos2D;
    double xsec;
    double efficiency;
    void InitHistograms();
};

Looper::Looper() : xsec(1.0)
{
}

void Looper::InitFromFileName(const std::string ifilename, double ixsec, double iefficiency)
{
  TTree *tree = 0;
  std::string filename = ifilename;
  xsec = ixsec;
  efficiency = iefficiency;
  // Connect to file and read from tree
  TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(filename.c_str());
  if (!f || !f->IsOpen()) {
    f = new TFile(filename.c_str());
  }
  TDirectory * dir = (TDirectory*)f->Get((filename+":/emergingJetAnalyzer").c_str());
  dir->GetObject("emergingJetsTree",tree);
  Init(tree);
}

// Copied from BaseClass::Loop()
void Looper::Loop(string ofilename)
{
  std::cout << "Start processing for output file: " << ofilename << std::endl;
  TH1::SetDefaultSumw2();
  TFile *ofile = new TFile(ofilename.c_str(), "RECREATE");
  InitHistograms();
  if (fChain == 0) return;
  fChain->SetBranchStatus("*",0);  // disable all branches
  fChain->SetBranchStatus("jets_pt",1);  // activate branchname
  fChain->SetBranchStatus("jets_eta",1);  // activate branchname
  fChain->SetBranchStatus("jets_medianLogIpSig",1);  // activate branchname

  Long64_t nentries = fChain->GetEntriesFast();
  std::cout << "Scaling by cross section = " << xsec << std::endl;
  std::cout << "Scaling by filter efficiency = " << efficiency << std::endl;
  std::cout << "Scaling by 1/number of events = 1/" << nentries << std::endl;
  if ( nentries == 0 ) {
    std::cout << "No entries!" << std::endl;
  }
  double weight = xsec*efficiency/nentries;

  Long64_t nbytes = 0, nb = 0;
  TStopwatch timer_total;
  timer_total.Start();
  // Loop over all events in TChain
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    // Get TTree-level entry number from TChain-level entry number
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;

    // Do stuff with one entry of the TTree
    {
      TStopwatch timer;
      if ( jentry % 10000 == 0 ) {
        std::cout << "Running over global entry: " << jentry << std::endl;
        if (jentry!=0) std::cout << "Last chunk took " << timer.RealTime() << std::endl;
        timer.Start();
      }
      else {
      }

      // Fill jet related histograms
      {
        int nJet = jets_pt->size();
        histos1D["nJet"]->Fill(nJet, weight);
        double ht = 0;
        double sumMedianLogIpSig = 0;
        vector<double> vec_medianLogIpSig;
        if ( jets_pt->size() < 4 ) {
          std::cout << "Skipping event with nJet<4" << std::endl;
          break;
        }
        // for (unsigned i=0; i!=4; i++) {
        for (unsigned i=0; i!=jets_pt->size(); i++) {
          // Ignore jets beyond the fourth
          if (i>3) break;
          double pt = jets_pt->at(i);
          double eta = jets_eta->at(i);
          double medianLogIpSig = jets_medianLogIpSig->at(i);

          histos1D["jet_pt"]->Fill(pt, weight);
          if (i==0) histos1D["jet_pt0"]->Fill(pt, weight);
          if (i==1) histos1D["jet_pt1"]->Fill(pt, weight);
          if (i==2) histos1D["jet_pt2"]->Fill(pt, weight);
          if (i==3) histos1D["jet_pt3"]->Fill(pt, weight);

          histos1D["jet_eta"]->Fill(eta, weight);
          if (i==0) histos1D["jet_eta0"]->Fill(eta, weight);
          if (i==1) histos1D["jet_eta1"]->Fill(eta, weight);
          if (i==2) histos1D["jet_eta2"]->Fill(eta, weight);
          if (i==3) histos1D["jet_eta3"]->Fill(eta, weight);

          histos1D["jet_medianLogIpSig"]->Fill(medianLogIpSig, weight);
          if (i==0) histos1D["jet_medianLogIpSig0"]->Fill(medianLogIpSig, weight);
          if (i==1) histos1D["jet_medianLogIpSig1"]->Fill(medianLogIpSig, weight);
          if (i==2) histos1D["jet_medianLogIpSig2"]->Fill(medianLogIpSig, weight);
          if (i==3) histos1D["jet_medianLogIpSig3"]->Fill(medianLogIpSig, weight);

          histos2D["jet_medianLogIpSig_pt"]->Fill(pt, medianLogIpSig, weight);

          ht += pt;
          sumMedianLogIpSig += medianLogIpSig;
          vec_medianLogIpSig.push_back(medianLogIpSig);
        }
        histos1D["ht"]->Fill(ht, weight);
        histos1D["sumMedianLogIpSig"]->Fill(sumMedianLogIpSig, weight);
        std::sort(vec_medianLogIpSig.begin(), vec_medianLogIpSig.end(), std::greater<double>());
        histos1D["maxMedianLogIpSig0"]->Fill( vec_medianLogIpSig[0], weight ) ;
        histos1D["maxMedianLogIpSig1"]->Fill( vec_medianLogIpSig[1], weight ) ;
        histos1D["maxMedianLogIpSig2"]->Fill( vec_medianLogIpSig[2], weight ) ;
        histos1D["maxMedianLogIpSig3"]->Fill( vec_medianLogIpSig[3], weight ) ;
        // for (auto i=0; i!=jets_eta->size(); i++) {
        //   histos1D["jet_eta"]->Fill(jets_eta[i], weight);
        // }
        // for (auto i=0; i!=jets_medianLogIpSig->size(); i++) {
        //   histos1D["jet_medianLogIpSig"]->Fill(jets_medianLogIpSig[i], weight);
        // }

      }

    } // Done processing one TTree entry
  }
  double total_time_elapsed = timer_total.RealTime();
  std::cout << "Total processing time: " << total_time_elapsed << std::endl;
  std::cout << std::endl;
  ofile->Write();

}

void Looper::InitHistograms()
{
  string name = "";
  name = "nJet"               ; histos1D[name] = new TH1F(name.c_str() , name.c_str() ,  25 , 0  ,  25  ) ;
  name = "ht"                 ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0  , 2500 ) ;
  name = "jet_pt"             ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0  , 1000 ) ;
  name = "jet_eta"            ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , -5 , 5    ) ;
  name = "jet_medianLogIpSig" ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , -5 , 5    ) ;
  name = "sumMedianLogIpSig"  ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , -25 , 25  ) ;
  for (int i=0; i< 4; i++) {
    name = "jet_pt"             + std::to_string(i) ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0  , 1000 ) ;
    name = "jet_eta"            + std::to_string(i) ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , -5 , 5    ) ;
    name = "jet_medianLogIpSig" + std::to_string(i) ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , -5 , 5    ) ;
    name = "maxMedianLogIpSig"  + std::to_string(i) ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , -5 , 5    ) ;
  }

  name = "jet_medianLogIpSig_pt"; histos2D[name] = new TH2F(name.c_str() , name.c_str() , 100 , 0  , 1000, 100, -5, 5  ) ;
}
