#include "HistoMakerBase.h"
#include "EmJetHistos.h"
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
  EmJetHistoMaker() {};
  ~EmJetHistoMaker() {};
  void InitHistograms();
  void FillHistograms(long eventnumber);
  void FillJetHistograms(long eventnumber);
  int SetTree(std::string);
  void SetOptions(Sample isample=Sample::SIGNAL, bool iData=false, double ixsec=1.0, double iefficiency=1.0, bool isignal=false);
 private:
  unique_ptr<Histos> histo_;
  Sample sample_;
  bool isData_;
  double xsec_;
  double efficiency_;
  bool isSignal_;
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
  dir->GetObject("emergingJetsTree",tree_);
  Init(tree_);
  return 0; // No error
}

void EmJetHistoMaker::InitHistograms()
{
  histo_ = unique_ptr<Histos>(new Histos());
}

void EmJetHistoMaker::FillHistograms(long eventnumber)
{
  FillJetHistograms(eventnumber);
}

void EmJetHistoMaker::FillJetHistograms(long eventnumber)
{
  double weight = 0.0;
  if (isData_) weight = 1.0;
  else        weight = xsec_*efficiency_/nentries_;
  // Fill jet histograms
  int nJet = jets_pt->size();
  // std::cout << "nJet:" << nJet << std::endl;
  histo_->nJet->Fill(nJet, weight);
  double ht = 0;
  double sumMedianLogIpSig = 0;
  vector<double> vec_medianLogIpSig;
  if (sample_==Sample::SIGNAL || sample_==Sample::QCD) {
    if ( jets_pt->size() < 4  ) {
      std::cout << "jets_pt->size(): " << jets_pt->size() << std::endl;
      std::cout << "Skipping event with nJet<4" << std::endl;
      return;
    }
  }

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
    // Skip jets with |eta|>2.5
    if (TMath::Abs(eta) > 2.5) continue;
    int nDarkPions = 0;
    bool sig = false;
    if (isSignal_) {
      nDarkPions = jets_nDarkPions->at(ijet);
      if (nDarkPions > 0) sig = true;
    }

    histo_->jet_pt->Fill(pt, weight);
    if (sig) histo_->jet_pt_sig->Fill(pt, weight);
    if (ijet>=0 && ijet<=3) histo_->jet_pt_sorted_by_pt[ijet]->Fill(pt, weight);

    histo_->jet_eta->Fill(eta, weight);
    if (ijet>=0 && ijet<=3) histo_->jet_eta_sorted_by_pt[ijet]->Fill(eta, weight);

    histo_->jet_nTracks->Fill(nTracks, weight);
    if (sig) histo_->jet_nTracks_sig->Fill(nTracks, weight);

    histo_->jet_medianLogIpSig->Fill(medianLogIpSig, weight);
    if (sig) histo_->jet_medianLogIpSig_sig->Fill(medianLogIpSig, weight);

    histo_->jet_alphaMax->Fill(alphaMax, weight);
    if (sig) histo_->jet_alphaMax_sig->Fill(alphaMax, weight);

    histo_->jet_nDarkPions->Fill(nDarkPions, weight);
    if (sig) histo_->jet_nDarkPions_sig->Fill(nDarkPions, weight);

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
      histo_ ->track_nHits       ->Fill( nHits               , weight);
      histo_ ->track_nMissHits   ->Fill( nMissHits           , weight);
      histo_ ->track_missHitFrac ->Fill( missHitFrac         , weight);
      histo_ ->track_ipXY        ->Fill( ipXY                , weight);
      histo_ ->track_logIpSig    ->Fill( TMath::Log(ipXYSig) , weight);
      if (sig) {
        histo_ ->track_nHits_sig       ->Fill( nHits               , weight);
        histo_ ->track_nMissHits_sig   ->Fill( nMissHits           , weight);
        histo_ ->track_missHitFrac_sig ->Fill( missHitFrac         , weight);
        histo_ ->track_ipXY_sig        ->Fill( ipXY                , weight);
        histo_ ->track_logIpSig_sig    ->Fill( TMath::Log(ipXYSig) , weight);
      }
    }
    // Sort tracks in descending order of ipXYSig
    std::sort( index_ipXYSig_ordered.begin(), index_ipXYSig_ordered.end(), [&](int a, int b){ return vec_ipXYSig[a] > vec_ipXYSig[b]; } );
    for (int i=0; i< nTrackSort; i++) {
      if (i>=nTracks) break; // Don't try to run over more than number of tracks in current jet
      auto ipXYSig_i = vec_ipXYSig[index_ipXYSig_ordered[i]]; // i-th largest ipXYSig
      histo_->jet_track_logIpSig_sorted[i]->Fill( TMath::Log(ipXYSig_i) , weight );
    }
    auto medIpSig = vec_ipXYSig[ index_ipXYSig_ordered[ nTracks/2 ] ];
    auto medLogIpSig = TMath::Log( medIpSig );

    histo_->jet_medLogIpSig->Fill(medLogIpSig, weight);
    if (sig) histo_->jet_medLogIpSig_sig->Fill(medLogIpSig, weight);
  }

  // Event-level quantities

  /*
  if (sample_==Sample::SIGNAL || sample_==Sample::QCD) {
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
    histo_->deltaPt->Fill(deltaPt, weight);
    histo_->sigmaPt->Fill(sigmaPt, weight);
    histo_->sigmaPt2->Fill(sigmaPt2, weight);

    histo_->ht->Fill(ht, weight);
    histo_->sumMedianLogIpSig->Fill(sumMedianLogIpSig, weight);
    std::sort(vec_medianLogIpSig.begin(), vec_medianLogIpSig.end(), std::greater<double>());
    histo_->maxMedianLogIpSig0->Fill( vec_medianLogIpSig[0], weight ) ;
    histo_->maxMedianLogIpSig1->Fill( vec_medianLogIpSig[1], weight ) ;
    histo_->maxMedianLogIpSig2->Fill( vec_medianLogIpSig[2], weight ) ;
    histo_->maxMedianLogIpSig3->Fill( vec_medianLogIpSig[3], weight ) ;
  }
  */
}

void
EmJetHistoMaker::SetOptions(Sample sample, bool isData, double xsec, double efficiency, bool isSignal)
{
  sample_ = sample;
  isData_ = isData;
  xsec_ = xsec;
  efficiency_ = efficiency;
  isSignal_ = isSignal;
}
