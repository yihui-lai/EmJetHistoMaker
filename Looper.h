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
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <functional>

using std::string;

#define N_TRACKSORT 50
enum class Sample {SIGNAL, QCD, WJET};

class Looper : BaseClass 
{
  public:
    Looper();
    void InitFromFileName(const std::string ifilename, Sample isample=Sample::SIGNAL, bool iData=false, double ixsec=1.0, double iefficiency=1.0, bool isignal=false);
    void Loop(string ofilename);
  private:
    std::unordered_map<std::string, TH1F*> histos1D;
    std::unordered_map<std::string, TH2F*> histos2D;
    Sample sample;
    bool isData;
    double xsec;
    double efficiency;
    bool isSignal;
    void InitHistograms();
};

Looper::Looper() : xsec(1.0), efficiency(1.0), isSignal(false)
{
}

void Looper::InitFromFileName(const std::string ifilename, Sample isample, bool iData, double ixsec, double iefficiency, bool isignal)
{
  TTree *tree = 0;
  std::string filename = ifilename;
  sample = isample;
  isData = iData;
  xsec = ixsec;
  efficiency = iefficiency;
  isSignal = isignal;
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
  fChain->SetBranchStatus("jets_alphaMax",1);  // activate branchname
  if (isSignal) {
    fChain->SetBranchStatus("jets_nDarkPions",1);  // activate branchname
  }
  fChain->SetBranchStatus("tracks_nHits",1);  // activate branchname
  fChain->SetBranchStatus("tracks_nMissInnerHits",1);  // activate branchname
  fChain->SetBranchStatus("tracks_ipXY",1);  // activate branchname
  fChain->SetBranchStatus("tracks_ipXYSig",1);  // activate branchname
  // fChain->SetBranchStatus("tracks_ipZ",1);  // activate branchname

  Long64_t nentries = fChain->GetEntriesFast();
  std::cout << "Scaling by cross section = " << xsec << std::endl;
  std::cout << "Scaling by filter efficiency = " << efficiency << std::endl;
  std::cout << "Scaling by 1/number of events = 1/" << nentries << std::endl;
  if ( nentries == 0 ) {
    std::cout << "No entries!" << std::endl;
  }
  double weight = 0.0;
  if (isData) weight = 1.0;
  else        weight = xsec*efficiency/nentries;

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
        if (jentry!=0) std::cout << "Last chunk took " << timer.RealTime() << std::endl;
        timer.Start();
        std::cout << "Running over global entry: " << jentry << std::endl;
      }
      else {
      }

      // Fill jet related histograms
      {
        int nJet = jets_pt->size();
        // std::cout << "nJet:" << nJet << std::endl;
        histos1D["nJet"]->Fill(nJet, weight);
        double ht = 0;
        double sumMedianLogIpSig = 0;
        vector<double> vec_medianLogIpSig;
        if (sample==Sample::SIGNAL || sample==Sample::QCD) {
          if ( jets_pt->size() < 4  ) {
            std::cout << "Skipping event with nJet<4" << std::endl;
            break;
          }
        }
        // for (unsigned i=0; i!=4; i++) {
        for (unsigned ijet=0; ijet!=jets_pt->size(); ijet++) {
          // Skip jets beyond the fourth
          if (ijet>3) break;
          double pt             = jets_pt             -> at(ijet);
          double eta            = jets_eta            -> at(ijet);
          double medianLogIpSig = jets_medianLogIpSig -> at(ijet);
          double alphaMax       = jets_alphaMax       -> at(ijet);
          auto& vec_nHits     = tracks_nHits          -> at(ijet);
          auto& vec_nMissHits = tracks_nMissInnerHits -> at(ijet);
          auto& vec_ipXY      = tracks_ipXY           -> at(ijet);
          auto& vec_ipXYSig   = tracks_ipXYSig        -> at(ijet);
          int nTracks = vec_ipXY.size();
          // Skip jets with zero tracks
          if (nTracks==0) continue;
          int nDarkPions = 0;
          bool sig = false;
          if (isSignal) {
            nDarkPions = jets_nDarkPions->at(ijet);
            if (nDarkPions > 0) sig = true;
          }

          histos1D["jet_pt"]->Fill(pt, weight);
          if (sig) histos1D["jet_pt_sig"]->Fill(pt, weight);
          if (ijet==0) histos1D["jet_pt0"]->Fill(pt, weight);
          if (ijet==1) histos1D["jet_pt1"]->Fill(pt, weight);
          if (ijet==2) histos1D["jet_pt2"]->Fill(pt, weight);
          if (ijet==3) histos1D["jet_pt3"]->Fill(pt, weight);

          histos1D["jet_eta"]->Fill(eta, weight);
          if (sig) histos1D["jet_eta_sig"]->Fill(eta, weight);
          if (ijet==0) histos1D["jet_eta0"]->Fill(eta, weight);
          if (ijet==1) histos1D["jet_eta1"]->Fill(eta, weight);
          if (ijet==2) histos1D["jet_eta2"]->Fill(eta, weight);
          if (ijet==3) histos1D["jet_eta3"]->Fill(eta, weight);

          histos1D["jet_nTracks"]->Fill(nTracks, weight);
          if (sig) histos1D["jet_nTracks_sig"]->Fill(nTracks, weight);

          histos1D["jet_medianLogIpSig"]->Fill(medianLogIpSig, weight);
          if (sig) histos1D["jet_medianLogIpSig_sig"]->Fill(medianLogIpSig, weight);

          histos1D["jet_alphaMax"]->Fill(alphaMax, weight);
          if (sig) histos1D["jet_alphaMax_sig"]->Fill(alphaMax, weight);

          histos1D["jet_nDarkPions"]->Fill(nDarkPions, weight);
          if (sig) histos1D["jet_nDarkPions_sig"]->Fill(nDarkPions, weight);

          histos2D["jet_medianLogIpSig_pt"]->Fill(pt, medianLogIpSig, weight);

          ht += pt;
          sumMedianLogIpSig += medianLogIpSig;
          vec_medianLogIpSig.push_back(medianLogIpSig);

          // Loop through tracks for given jet
          vector<int> index_ipXYSig_ordered;
          for (unsigned itk=0; itk!=vec_ipXY.size(); itk++) {
            string name; string prefix =""; string postfix="";
            index_ipXYSig_ordered.push_back(itk);
            int nHits = vec_nHits[itk];
            int nMissHits = vec_nMissHits[itk];
            double missHitFrac = double(nMissHits) / double(nHits);
            double ipXY    = vec_ipXY    [itk];
            double ipXYSig = vec_ipXYSig [itk];
            name = prefix + "track_nHits"        + postfix; histos1D[name] ->Fill( nHits               , weight);
            name = prefix + "track_nMissHits"    + postfix; histos1D[name] ->Fill( nMissHits           , weight);
            name = prefix + "track_missHitFrac"  + postfix; histos1D[name] ->Fill( missHitFrac         , weight);
            name = prefix + "track_ipXY"         + postfix; histos1D[name] ->Fill( ipXY                , weight);
            name = prefix + "track_logIpSig"     + postfix; histos1D[name] ->Fill( TMath::Log(ipXYSig) , weight);
            if (sig) {
              postfix = "_sig";
              name = prefix + "track_nHits"        + postfix; histos1D[name] ->Fill( nHits               , weight);
              name = prefix + "track_nMissHits"    + postfix; histos1D[name] ->Fill( nMissHits           , weight);
              name = prefix + "track_missHitFrac"  + postfix; histos1D[name] ->Fill( missHitFrac         , weight);
              name = prefix + "track_ipXY"         + postfix; histos1D[name] ->Fill( ipXY                , weight);
              name = prefix + "track_logIpSig"     + postfix; histos1D[name] ->Fill( TMath::Log(ipXYSig) , weight);
            }
          }
          // Sort tracks in descending order of ipXYSig
          std::sort( index_ipXYSig_ordered.begin(), index_ipXYSig_ordered.end(), [&](int a, int b){ return vec_ipXYSig[a] > vec_ipXYSig[b]; } );
          for (int i=0; i< N_TRACKSORT; i++) {
            if (i>=nTracks) break; // Don't try to run over more than number of tracks in current jet
            string prefix = ""; string postfix = std::to_string(i);
            auto ipXYSig_i = vec_ipXYSig[index_ipXYSig_ordered[i]]; // i-th largest ipXYSig
            string name = prefix + "jet_track_logIpSig" + postfix ; histos1D[name]->Fill( TMath::Log(ipXYSig_i) , weight );
          }
          auto medIpSig = vec_ipXYSig[ index_ipXYSig_ordered[ nTracks/2 ] ];
          auto medLogIpSig = TMath::Log( medIpSig );

          histos1D["jet_medLogIpSig"]->Fill(medLogIpSig, weight);
          if (sig) histos1D["jet_medLogIpSig_sig"]->Fill(medLogIpSig, weight);
        }

        // Event-level quantities

        if (sample==Sample::SIGNAL || sample==Sample::QCD) {
          // nJet >= 4 at this point
          assert(jets_pt->size()>=4);
          double deltaPt = jets_pt->at(0) - jets_pt->at(1);
          double sigmaPt = 0;
          double sigmaPt2 = 0;
          for ( unsigned i=0; i<3; i++ ) {
            float delta = jets_pt->at(i) - jets_pt->at(i+1); 
            sigmaPt += delta*delta;
            sigmaPt2 += TMath::Sqrt(delta);
          }
          sigmaPt = TMath::Sqrt(sigmaPt);
          sigmaPt2 = sigmaPt2*sigmaPt2;
          histos1D["deltaPt"]->Fill(deltaPt, weight);
          histos1D["sigmaPt"]->Fill(sigmaPt, weight);
          histos1D["sigmaPt2"]->Fill(sigmaPt2, weight);

          histos1D["ht"]->Fill(ht, weight);
          histos1D["sumMedianLogIpSig"]->Fill(sumMedianLogIpSig, weight);
          std::sort(vec_medianLogIpSig.begin(), vec_medianLogIpSig.end(), std::greater<double>());
          histos1D["maxMedianLogIpSig0"]->Fill( vec_medianLogIpSig[0], weight ) ;
          histos1D["maxMedianLogIpSig1"]->Fill( vec_medianLogIpSig[1], weight ) ;
          histos1D["maxMedianLogIpSig2"]->Fill( vec_medianLogIpSig[2], weight ) ;
          histos1D["maxMedianLogIpSig3"]->Fill( vec_medianLogIpSig[3], weight ) ;
        }
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
  string prefix = ""; string postfix = "";
  name = prefix + "nJet"               + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() ,  25 , 0  ,  25  ) ;
  name = prefix + "ht"                 + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0  , 2500 ) ;
  name = prefix + "sigmaPt"            + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0  , 1500 ) ;
  name = prefix + "sigmaPt2"           + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0  , 1500 ) ;
  name = prefix + "deltaPt"            + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0  , 1000 ) ;
  name = prefix + "jet_pt"             + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0  , 1000 ) ;
  name = prefix + "jet_eta"            + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , -5 , 5    ) ;
  name = prefix + "jet_nTracks"        + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0. , 100  ) ;
  name = prefix + "jet_medianLogIpSig" + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , -5 , 5    ) ;
  name = prefix + "jet_medLogIpSig"    + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , -5 , 5    ) ;
  name = prefix + "jet_alphaMax"       + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0. , 1.   ) ;
  name = prefix + "jet_nHits"          + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0. , 100  ) ;
  name = prefix + "jet_nMissHits"      + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0. , 100  ) ;
  name = prefix + "jet_missHitFrac"    + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0. ,   1. ) ;
  name = prefix + "jet_nDarkPions"     + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0. , 100  ) ;
  name = prefix + "sumMedianLogIpSig"  + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , -25,  25  ) ;
  name = prefix + "track_ipXY"         + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , -5 ,   5  ) ;
  name = prefix + "track_logIpSig"     + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , -5 ,   5  ) ;
  name = prefix + "track_nHits"        + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0. , 100  ) ;
  name = prefix + "track_nMissHits"    + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0. , 100  ) ;
  name = prefix + "track_missHitFrac"  + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0. ,   1. ) ;
  // Ordered by pt
  for (int i=0; i< 4; i++) {
    prefix = ""; postfix = std::to_string(i);
    name = prefix + "jet_pt"             + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0  , 1000 ) ;
    name = prefix + "jet_eta"            + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , -5 , 5    ) ;
  }
  // Ordered by logIpSig
  for (int i=0; i< N_TRACKSORT; i++) {
    prefix = ""; postfix = std::to_string(i);
    name = prefix + "jet_track_logIpSig" + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , -5 , 20   ) ;
  }
  // Ordered by medianLogIpSig
  for (int i=0; i< 4; i++) {
    prefix = ""; postfix = std::to_string(i);
    name = prefix + "maxMedianLogIpSig"  + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , -5 , 5    ) ;
  }

  {
    prefix = ""; postfix = "_sig";
    name = prefix + "jet_pt"             + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0  , 1000 ) ;
    name = prefix + "jet_eta"            + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , -5 , 5    ) ;
    name = prefix + "jet_nTracks"        + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0. , 100  ) ;
    name = prefix + "jet_medianLogIpSig" + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , -5 , 5    ) ;
    name = prefix + "jet_medLogIpSig"    + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , -5 , 5    ) ;
    name = prefix + "jet_alphaMax"       + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0. , 1.   ) ;
    name = prefix + "jet_nHits"          + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0. , 100  ) ;
    name = prefix + "jet_nMissHits"      + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0. , 100  ) ;
    name = prefix + "jet_missHitFrac"    + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0. ,   1. ) ;
    name = prefix + "jet_nDarkPions"     + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0. , 100  ) ;
    name = prefix + "track_ipXY"         + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , -5 ,   5  ) ;
    name = prefix + "track_logIpSig"     + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , -5 ,   5  ) ;
    name = prefix + "track_nHits"        + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0. , 100  ) ;
    name = prefix + "track_nMissHits"    + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0. , 100  ) ;
    name = prefix + "track_missHitFrac"  + postfix ; histos1D[name] = new TH1F(name.c_str() , name.c_str() , 100 , 0. ,   1. ) ;
  }

  postfix = ""; postfix="";
  name = prefix + "jet_medianLogIpSig_pt" + postfix ; histos2D[name] = new TH2F(name.c_str() , name.c_str() , 100 , 0  , 1000, 100, -5, 5  ) ;
}
