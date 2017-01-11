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
  void FillHistograms(long eventnumber);
  void FillJetHistograms(long eventnumber);
  void FillPileupHistograms(long eventnumber);
  int SetTree(string ifilename);
  void SetOptions(Sample sample=Sample::SIGNAL, bool isData=false, double xsec=1.0, double efficiency=1.0, bool isSignal=false, bool pileupOnly=false);
 private:
  double CalculateEventWeight(long eventnumber);
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
  FillPileupHistograms(eventnumber);
  if (!pileupOnly_) {
    FillJetHistograms(eventnumber);
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

void EmJetHistoMaker::FillJetHistograms(long eventnumber)
{
  double weight = CalculateEventWeight(eventnumber);
  // Fill jet histograms
  int nJet = jet_pt->size();
  // std::cout << "nJet:" << nJet << std::endl;
  histo_->nJet->Fill(nJet, weight);
  double ht = 0;
  double sumMedianLogIpSig = 0;
  vector<double> vec_medianLogIpSig;
  if (sample_==Sample::SIGNAL || sample_==Sample::QCD) {
    if ( jet_pt->size() < 4  ) {
      std::cout << "jet_pt->size(): " << jet_pt->size() << std::endl;
      std::cout << "Skipping event with nJet<4" << std::endl;
      return; // :CUT: Skip event with nJet<4
    }
  }
  // Count number of jets with alphaMax higher than cut
  {
    float cut_value = 0.05;
    int nJet_high = std::count_if(jet_alphaMax->begin(), jet_alphaMax->end(), [cut_value](float i) {return i >= cut_value;});
    int nJet_low  = nJet - nJet_high;
    histo_->nJet_by_alphaMax[0]->Fill(nJet_low, weight);
    histo_->nJet_by_alphaMax[1]->Fill(nJet_high, weight);
  }

  nJet = 0;
  for (unsigned ijet=0; ijet!=jet_pt->size(); ijet++) {
    // Skip jets beyond the fourth
    // if (ijet>3) break; // :CUT:
    double pt             = jet_pt             -> at(ijet);
    double eta            = jet_eta            -> at(ijet);
    double cef            = jet_cef            -> at(ijet);
    double nef            = jet_nef            -> at(ijet);
    // double medianLogIpSig = jet_medianLogIpSig -> at(ijet);
    double alphaMax       = jet_alphaMax       -> at(ijet);
    // if (TMath::Abs(eta)>2) break; // :CUT: Skips jets with eta>2
    if (cef>0.9) break; // :CUT: Skips jets with charged EM fraction > 0.9
    if (nef>0.9) break; // :CUT: Skips jets with neutral EM fraction > 0.9
    if (alphaMax>0.2) break; // :CUT: Skips jets with alphaMax > 0.05 (cut2)
    auto& vec_source              = track_source              -> at(ijet);
    auto& vec_pt                  = track_pt                  -> at(ijet);
    auto& vec_eta                 = track_eta                 -> at(ijet);
    auto& vec_phi                 = track_phi                 -> at(ijet);
    auto& vec_algo                = track_algo                -> at(ijet);
    auto& vec_originalAlgo        = track_originalAlgo        -> at(ijet);
    auto& vec_nHits               = track_nHits               -> at(ijet);
    auto& vec_nMissHits           = track_nMissInnerHits      -> at(ijet);
    auto& vec_nTrkLayers          = track_nTrkLayers          -> at(ijet);
    auto& vec_nMissInnerTrkLayers = track_nMissInnerTrkLayers -> at(ijet);
    auto& vec_nMissOuterTrkLayers = track_nMissOuterTrkLayers -> at(ijet);
    auto& vec_nMissTrkLayers      = track_nMissTrkLayers      -> at(ijet);
    auto& vec_nPxlLayers          = track_nPxlLayers          -> at(ijet);
    auto& vec_nMissInnerPxlLayers = track_nMissInnerPxlLayers -> at(ijet);
    auto& vec_nMissOuterPxlLayers = track_nMissOuterPxlLayers -> at(ijet);
    auto& vec_nMissPxlLayers      = track_nMissPxlLayers      -> at(ijet);
    auto& vec_dRToJetAxis         = track_dRToJetAxis         -> at(ijet);
    auto& vec_distanceToJet       = track_distanceToJet       -> at(ijet);
    auto& vec_ipXY                = track_ipXY                -> at(ijet);
    auto& vec_ipXYSig             = track_ipXYSig             -> at(ijet);
    int nTracks = vec_ipXY.size();
    if (nTracks==0) continue; // :CUT: Skip jets with zero tracks
    if (TMath::Abs(eta) > 2.5) continue; // :CUT: Skip jets with |eta|>2.5
    int nDarkPions = 0;
    bool sig = false; // Tagged with dark pion?
    int nDarkGluons = 0;
    bool gluond = false; // Tagged with dark gluon?
    if (isSignal_) {
      nDarkPions = jet_nDarkPions->at(ijet);
      if (nDarkPions > 0) sig = true;
      nDarkGluons = jet_nDarkGluons->at(ijet);
      if (nDarkGluons > 0) gluond = true;
    }
    bool lowntrack = false; if (nTracks < 15) lowntrack = true;
    bool highntrack = false; if (nTracks > 35) highntrack = true;

    {
      const float ipSig_cut_values [4] = {2.0, 5.0, 10.0, 20.0};
      for (int icut=0; icut<4; icut++)
        {
          // Count number of tracks with ipXYSig above and below cut value
          float cut_value = ipSig_cut_values[icut];
          int nTrack_high = std::count_if(vec_ipXYSig.begin(), vec_ipXYSig.end(), [cut_value](float i) {return i >= cut_value;});
          int nTrack_low  = nTracks - nTrack_high;
          // histo_->jet_nTrack_by_ipSig_cut_low[icut]->Fill(nTrack_low, weight);
          // histo_->jet_nTrack_by_ipSig_cut_high[icut]->Fill(nTrack_high, weight);
          if (sig) {
            // histo_->jet_nTrack_by_ipSig_cut_low_sig[icut]->Fill(nTrack_low, weight);
            // histo_->jet_nTrack_by_ipSig_cut_high_sig[icut]->Fill(nTrack_high, weight);
          }
        }
    }
    // Calculate prompt track energy fraction
    double total_pt = 0;
    double prompt_pt = 0;
    double prompt_frac_E = 0;


    ht += pt;
    // sumMedianLogIpSig += medianLogIpSig;
    // vec_medianLogIpSig.push_back(medianLogIpSig);

    // Loop through tracks for given jet
    vector<int> index_ipXYSig_ordered;
    for (unsigned itk=0; itk!=vec_ipXY.size(); itk++) {
      string name; string prefix =""; string postfix="";
      index_ipXYSig_ordered.push_back(itk);
      // int nHits = vec_nHits[itk];
      // int nMissHits = vec_nMissHits[itk];
#define GET_TRACK_VAR_FROM_VEC(var) double var = vec_##var[itk]
      GET_TRACK_VAR_FROM_VEC (source              );
      if (source != TRACKSOURCE) continue; // Only run over tracks from the specified source
      GET_TRACK_VAR_FROM_VEC (pt                  );
      GET_TRACK_VAR_FROM_VEC (eta                 );
      GET_TRACK_VAR_FROM_VEC (phi                 );
      GET_TRACK_VAR_FROM_VEC (algo                );
      GET_TRACK_VAR_FROM_VEC (originalAlgo        );
      GET_TRACK_VAR_FROM_VEC (nHits               );
      GET_TRACK_VAR_FROM_VEC (nMissHits           );
      GET_TRACK_VAR_FROM_VEC (ipXY                );
      GET_TRACK_VAR_FROM_VEC (ipXYSig             );
      GET_TRACK_VAR_FROM_VEC (nTrkLayers          );
      GET_TRACK_VAR_FROM_VEC (nMissInnerTrkLayers );
      GET_TRACK_VAR_FROM_VEC (nMissOuterTrkLayers );
      GET_TRACK_VAR_FROM_VEC (nMissTrkLayers      );
      GET_TRACK_VAR_FROM_VEC (nPxlLayers          );
      GET_TRACK_VAR_FROM_VEC (nMissInnerPxlLayers );
      GET_TRACK_VAR_FROM_VEC (nMissOuterPxlLayers );
      GET_TRACK_VAR_FROM_VEC (nMissPxlLayers      );
      GET_TRACK_VAR_FROM_VEC (dRToJetAxis         );
      GET_TRACK_VAR_FROM_VEC (distanceToJet       );
#undef GET_TRACK_VAR_FROM_VEC
      double missHitFrac = double(nMissHits) / double(nHits);
      double ipSig = ipXYSig;
      double logIpSig = TMath::Log(ipXYSig);
      double nNetMissInnerLayers = (nMissInnerPxlLayers+nMissInnerTrkLayers) - (nMissPxlLayers+nMissOuterPxlLayers+nMissTrkLayers+nMissOuterTrkLayers);
      double missLayerFrac = (nNetMissInnerLayers)/(nPxlLayers+nTrkLayers);
      bool regionA = (-1 < eta && eta < 0) && (-2 < phi && phi < -1);
      bool regionB = (-1 < eta && eta < 0) && ( 2 < phi && phi < 3 );
#define FILL_TRACK_HISTO(var, postfix) histo_->track_##var##postfix ->Fill(var, weight)
      FILL_TRACK_HISTO (pt                  , );
      FILL_TRACK_HISTO (eta                 , );
      FILL_TRACK_HISTO (phi                 , );
      FILL_TRACK_HISTO (algo                , );
      FILL_TRACK_HISTO (originalAlgo        , );
      FILL_TRACK_HISTO (nHits               , );
      FILL_TRACK_HISTO (nMissHits           , );
      FILL_TRACK_HISTO (missHitFrac         , );
      FILL_TRACK_HISTO (nTrkLayers          , );
      FILL_TRACK_HISTO (nMissInnerTrkLayers , );
      FILL_TRACK_HISTO (nPxlLayers          , );
      FILL_TRACK_HISTO (nMissInnerPxlLayers , );
      FILL_TRACK_HISTO (nNetMissInnerLayers , );
      FILL_TRACK_HISTO (missLayerFrac       , );
      FILL_TRACK_HISTO (ipXY                , );
      FILL_TRACK_HISTO (ipSig               , );
      FILL_TRACK_HISTO (logIpSig            , );
      FILL_TRACK_HISTO (dRToJetAxis         , );
      FILL_TRACK_HISTO (distanceToJet       , );
      if (sig) {
        FILL_TRACK_HISTO (pt                  , _sig);
        FILL_TRACK_HISTO (eta                 , _sig);
        FILL_TRACK_HISTO (phi                 , _sig);
        FILL_TRACK_HISTO (algo                , _sig);
        FILL_TRACK_HISTO (originalAlgo        , _sig);
        FILL_TRACK_HISTO (nHits               , _sig);
        FILL_TRACK_HISTO (nMissHits           , _sig);
        FILL_TRACK_HISTO (missHitFrac         , _sig);
        FILL_TRACK_HISTO (nTrkLayers          , _sig);
        FILL_TRACK_HISTO (nMissInnerTrkLayers , _sig);
        FILL_TRACK_HISTO (nPxlLayers          , _sig);
        FILL_TRACK_HISTO (nMissInnerPxlLayers , _sig);
        FILL_TRACK_HISTO (nNetMissInnerLayers , _sig);
        FILL_TRACK_HISTO (missLayerFrac       , _sig);
        FILL_TRACK_HISTO (ipXY                , _sig);
        FILL_TRACK_HISTO (ipSig               , _sig);
        FILL_TRACK_HISTO (logIpSig            , _sig);
        FILL_TRACK_HISTO (dRToJetAxis         , _sig);
        FILL_TRACK_HISTO (distanceToJet       , _sig);
      }
      if (gluond) {
        FILL_TRACK_HISTO (pt                  , _gluond);
        FILL_TRACK_HISTO (eta                 , _gluond);
        FILL_TRACK_HISTO (phi                 , _gluond);
        FILL_TRACK_HISTO (algo                , _gluond);
        FILL_TRACK_HISTO (originalAlgo        , _gluond);
        FILL_TRACK_HISTO (nHits               , _gluond);
        FILL_TRACK_HISTO (nMissHits           , _gluond);
        FILL_TRACK_HISTO (missHitFrac         , _gluond);
        FILL_TRACK_HISTO (nTrkLayers          , _gluond);
        FILL_TRACK_HISTO (nMissInnerTrkLayers , _gluond);
        FILL_TRACK_HISTO (nPxlLayers          , _gluond);
        FILL_TRACK_HISTO (nMissInnerPxlLayers , _gluond);
        FILL_TRACK_HISTO (nNetMissInnerLayers , _gluond);
        FILL_TRACK_HISTO (missLayerFrac       , _gluond);
        FILL_TRACK_HISTO (ipXY                , _gluond);
        FILL_TRACK_HISTO (ipSig               , _gluond);
        FILL_TRACK_HISTO (logIpSig            , _gluond);
        FILL_TRACK_HISTO (dRToJetAxis         , _gluond);
        FILL_TRACK_HISTO (distanceToJet       , _gluond);
      }
      if (lowntrack) {
        FILL_TRACK_HISTO (pt                  , _lowntrack);
        FILL_TRACK_HISTO (eta                 , _lowntrack);
        FILL_TRACK_HISTO (phi                 , _lowntrack);
        FILL_TRACK_HISTO (algo                , _lowntrack);
        FILL_TRACK_HISTO (originalAlgo        , _lowntrack);
        FILL_TRACK_HISTO (nHits               , _lowntrack);
        FILL_TRACK_HISTO (nMissHits           , _lowntrack);
        FILL_TRACK_HISTO (missHitFrac         , _lowntrack);
        FILL_TRACK_HISTO (nTrkLayers          , _lowntrack);
        FILL_TRACK_HISTO (nMissInnerTrkLayers , _lowntrack);
        FILL_TRACK_HISTO (nPxlLayers          , _lowntrack);
        FILL_TRACK_HISTO (nMissInnerPxlLayers , _lowntrack);
        FILL_TRACK_HISTO (nNetMissInnerLayers , _lowntrack);
        FILL_TRACK_HISTO (missLayerFrac       , _lowntrack);
        FILL_TRACK_HISTO (ipXY                , _lowntrack);
        FILL_TRACK_HISTO (ipSig               , _lowntrack);
        FILL_TRACK_HISTO (logIpSig            , _lowntrack);
        FILL_TRACK_HISTO (dRToJetAxis         , _lowntrack);
        FILL_TRACK_HISTO (distanceToJet       , _lowntrack);
      }
      if (highntrack) {
        FILL_TRACK_HISTO (pt                  , _highntrack);
        FILL_TRACK_HISTO (eta                 , _highntrack);
        FILL_TRACK_HISTO (phi                 , _highntrack);
        FILL_TRACK_HISTO (algo                , _highntrack);
        FILL_TRACK_HISTO (originalAlgo        , _highntrack);
        FILL_TRACK_HISTO (nHits               , _highntrack);
        FILL_TRACK_HISTO (nMissHits           , _highntrack);
        FILL_TRACK_HISTO (missHitFrac         , _highntrack);
        FILL_TRACK_HISTO (nTrkLayers          , _highntrack);
        FILL_TRACK_HISTO (nMissInnerTrkLayers , _highntrack);
        FILL_TRACK_HISTO (nPxlLayers          , _highntrack);
        FILL_TRACK_HISTO (nMissInnerPxlLayers , _highntrack);
        FILL_TRACK_HISTO (nNetMissInnerLayers , _highntrack);
        FILL_TRACK_HISTO (missLayerFrac       , _highntrack);
        FILL_TRACK_HISTO (ipXY                , _highntrack);
        FILL_TRACK_HISTO (ipSig               , _highntrack);
        FILL_TRACK_HISTO (logIpSig            , _highntrack);
        FILL_TRACK_HISTO (dRToJetAxis         , _highntrack);
        FILL_TRACK_HISTO (distanceToJet       , _highntrack);
      }
      if (pt>5) {
        FILL_TRACK_HISTO (pt                  , _highpt);
        FILL_TRACK_HISTO (eta                 , _highpt);
        FILL_TRACK_HISTO (phi                 , _highpt);
        FILL_TRACK_HISTO (algo                , _highpt);
        FILL_TRACK_HISTO (originalAlgo        , _highpt);
        FILL_TRACK_HISTO (nHits               , _highpt);
        FILL_TRACK_HISTO (nMissHits           , _highpt);
        FILL_TRACK_HISTO (missHitFrac         , _highpt);
        FILL_TRACK_HISTO (nTrkLayers          , _highpt);
        FILL_TRACK_HISTO (nMissInnerTrkLayers , _highpt);
        FILL_TRACK_HISTO (nPxlLayers          , _highpt);
        FILL_TRACK_HISTO (nMissInnerPxlLayers , _highpt);
        FILL_TRACK_HISTO (nNetMissInnerLayers , _highpt);
        FILL_TRACK_HISTO (missLayerFrac       , _highpt);
        FILL_TRACK_HISTO (ipXY                , _highpt);
        FILL_TRACK_HISTO (ipSig               , _highpt);
        FILL_TRACK_HISTO (logIpSig            , _highpt);
        FILL_TRACK_HISTO (dRToJetAxis         , _highpt);
        FILL_TRACK_HISTO (distanceToJet       , _highpt);
      }
      if (regionA) {
        FILL_TRACK_HISTO (pt                  , _regionA);
        FILL_TRACK_HISTO (eta                 , _regionA);
        FILL_TRACK_HISTO (phi                 , _regionA);
        FILL_TRACK_HISTO (algo                , _regionA);
        FILL_TRACK_HISTO (originalAlgo        , _regionA);
        FILL_TRACK_HISTO (nHits               , _regionA);
        FILL_TRACK_HISTO (nMissHits           , _regionA);
        FILL_TRACK_HISTO (missHitFrac         , _regionA);
        FILL_TRACK_HISTO (nTrkLayers          , _regionA);
        FILL_TRACK_HISTO (nMissInnerTrkLayers , _regionA);
        FILL_TRACK_HISTO (nPxlLayers          , _regionA);
        FILL_TRACK_HISTO (nMissInnerPxlLayers , _regionA);
        FILL_TRACK_HISTO (nNetMissInnerLayers , _regionA);
        FILL_TRACK_HISTO (missLayerFrac       , _regionA);
        FILL_TRACK_HISTO (ipXY                , _regionA);
        FILL_TRACK_HISTO (ipSig               , _regionA);
        FILL_TRACK_HISTO (logIpSig            , _regionA);
        FILL_TRACK_HISTO (dRToJetAxis         , _regionA);
        FILL_TRACK_HISTO (distanceToJet       , _regionA);
      }
      if (regionB) {
        FILL_TRACK_HISTO (pt                  , _regionB);
        FILL_TRACK_HISTO (eta                 , _regionB);
        FILL_TRACK_HISTO (phi                 , _regionB);
        FILL_TRACK_HISTO (algo                , _regionB);
        FILL_TRACK_HISTO (originalAlgo        , _regionB);
        FILL_TRACK_HISTO (nHits               , _regionB);
        FILL_TRACK_HISTO (nMissHits           , _regionB);
        FILL_TRACK_HISTO (missHitFrac         , _regionB);
        FILL_TRACK_HISTO (nTrkLayers          , _regionB);
        FILL_TRACK_HISTO (nMissInnerTrkLayers , _regionB);
        FILL_TRACK_HISTO (nPxlLayers          , _regionB);
        FILL_TRACK_HISTO (nMissInnerPxlLayers , _regionB);
        FILL_TRACK_HISTO (nNetMissInnerLayers , _regionB);
        FILL_TRACK_HISTO (missLayerFrac       , _regionB);
        FILL_TRACK_HISTO (ipXY                , _regionB);
        FILL_TRACK_HISTO (ipSig               , _regionB);
        FILL_TRACK_HISTO (logIpSig            , _regionB);
        FILL_TRACK_HISTO (dRToJetAxis         , _regionB);
        FILL_TRACK_HISTO (distanceToJet       , _regionB);
      }
#undef FILL_TRACK_HISTO
      histo_->track_pt_VS_track_eta->Fill(eta, pt, weight);
      histo_->track_phi_VS_track_eta->Fill(eta, phi, weight);
      histo_->track_ipXY_VS_track_phi->Fill(phi, ipXY, weight);
      histo_->track_ipXY_VS_track_nHits->Fill(nHits, ipXY, weight);
      histo_->track_ipSig_VS_track_nHits->Fill(nHits, ipSig, weight);
      if (nHits<=15) {
        histo_->track_pt_VS_track_eta_sorted_by_cuts[0] ->Fill(eta, pt, weight);
        histo_->track_phi_VS_track_eta_sorted_by_cuts[0] ->Fill(eta, phi, weight);
      }
      else           {
        histo_->track_pt_VS_track_eta_sorted_by_cuts[1] ->Fill(eta, pt, weight);
        histo_->track_phi_VS_track_eta_sorted_by_cuts[1] ->Fill(eta, phi, weight);
      }
      if (dRToJetAxis<=0.1) {
        histo_->track_phi_VS_track_eta_by_dRToJetAxis[0] ->Fill(eta, phi, weight);
      }
      else           {
        histo_->track_phi_VS_track_eta_by_dRToJetAxis[1] ->Fill(eta, phi, weight);
      }
      if (nMissInnerPxlLayers==0) {
        histo_->track_phi_VS_track_eta_by_nMissInnerPxlLayers[0] ->Fill(eta, phi, weight);
      }
      else           {
        histo_->track_phi_VS_track_eta_by_nMissInnerPxlLayers[1] ->Fill(eta, phi, weight);
      }
      if (nMissInnerTrkLayers==0) {
        histo_->track_phi_VS_track_eta_by_nMissInnerTrkLayers[0] ->Fill(eta, phi, weight);
      }
      else           {
        histo_->track_phi_VS_track_eta_by_nMissInnerTrkLayers[1] ->Fill(eta, phi, weight);
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
    // if (medLogIpSig<1.0) break; // :CUT: Skips jets with medLogIpSig < 1.0 (cut1)
    nJet++;

    // Jet-level histograms
    {
      for (unsigned i=0; i!=vec_ipXY.size(); i++) {
        int source = vec_source[i];
        double pt   = vec_pt[i];
        double ipXY = vec_ipXY[i];
        if (source != TRACKSOURCE) continue; // Only run over tracks from the specified source
        if ( ipXY < 0.025 ) prompt_pt += pt;
        total_pt += pt;
      }
      prompt_frac_E = prompt_pt/total_pt;
      histo_->jet_prompt_frac_E->Fill(prompt_frac_E, weight);
      if (sig) histo_->jet_prompt_frac_E_sig->Fill(prompt_frac_E, weight);
      if (gluond) histo_->jet_prompt_frac_E_gluond->Fill(prompt_frac_E, weight);
      if (lowntrack) histo_->jet_prompt_frac_E_lowntrack->Fill(prompt_frac_E, weight);
      if (highntrack) histo_->jet_prompt_frac_E_highntrack->Fill(prompt_frac_E, weight);

      double displaced_frac = 1 - prompt_frac_E;
      histo_->jet_displaced_frac->Fill(displaced_frac, weight);
      if (sig) histo_->jet_displaced_frac_sig->Fill(displaced_frac, weight);
      if (gluond) histo_->jet_displaced_frac_gluond->Fill(displaced_frac, weight);
      if (lowntrack) histo_->jet_displaced_frac_lowntrack->Fill(displaced_frac, weight);
      if (highntrack) histo_->jet_displaced_frac_highntrack->Fill(displaced_frac, weight);

      histo_->jet_pt->Fill(pt, weight);
      if (sig) histo_->jet_pt_sig->Fill(pt, weight);
      if (gluond) histo_->jet_pt_gluond->Fill(pt, weight);
      if (lowntrack) histo_->jet_pt_lowntrack->Fill(pt, weight);
      if (highntrack) histo_->jet_pt_highntrack->Fill(pt, weight);
      if (ijet>=0 && ijet<=3) histo_->jet_pt_sorted_by_pt[ijet]->Fill(pt, weight);

      histo_->jet_eta->Fill(eta, weight);
      if (ijet>=0 && ijet<=3) histo_->jet_eta_sorted_by_pt[ijet]->Fill(eta, weight);

      histo_->jet_nTracks->Fill(nTracks, weight);
      if (sig) histo_->jet_nTracks_sig->Fill(nTracks, weight);
      if (gluond) histo_->jet_nTracks_gluond->Fill(nTracks, weight);
      if (lowntrack) histo_->jet_nTracks_lowntrack->Fill(nTracks, weight);
      if (highntrack) histo_->jet_nTracks_highntrack->Fill(nTracks, weight);

      // histo_->jet_medianLogIpSig->Fill(medianLogIpSig, weight);
      // if (sig) histo_->jet_medianLogIpSig_sig->Fill(medianLogIpSig, weight);

      histo_->jet_alphaMax->Fill(alphaMax, weight);
      if (sig) histo_->jet_alphaMax_sig->Fill(alphaMax, weight);
      if (gluond) histo_->jet_alphaMax_gluond->Fill(alphaMax, weight);
      if (lowntrack) histo_->jet_alphaMax_lowntrack->Fill(alphaMax, weight);
      if (highntrack) histo_->jet_alphaMax_highntrack->Fill(alphaMax, weight);

      double alphaMaxNeg = 1 - alphaMax;
      histo_->jet_alphaMaxNeg->Fill(alphaMaxNeg, weight);
      if (sig) histo_->jet_alphaMaxNeg_sig->Fill(alphaMaxNeg, weight);
      if (gluond) histo_->jet_alphaMaxNeg_gluond->Fill(alphaMaxNeg, weight);
      if (lowntrack) histo_->jet_alphaMaxNeg_lowntrack->Fill(alphaMaxNeg, weight);
      if (highntrack) histo_->jet_alphaMaxNeg_highntrack->Fill(alphaMaxNeg, weight);

      histo_->jet_nDarkPions->Fill(nDarkPions, weight);
      if (sig) histo_->jet_nDarkPions_sig->Fill(nDarkPions, weight);
      if (gluond) histo_->jet_nDarkPions_gluond->Fill(nDarkPions, weight);
      if (lowntrack) histo_->jet_nDarkPions_lowntrack->Fill(nDarkPions, weight);
      if (highntrack) histo_->jet_nDarkPions_highntrack->Fill(nDarkPions, weight);

      histo_->jet_medLogIpSig->Fill(medLogIpSig, weight);
      if (sig) histo_->jet_medLogIpSig_sig->Fill(medLogIpSig, weight);
      if (gluond) histo_->jet_medLogIpSig_gluond->Fill(medLogIpSig, weight);
      if (lowntrack) histo_->jet_medLogIpSig_lowntrack->Fill(medLogIpSig, weight);
      if (highntrack) histo_->jet_medLogIpSig_highntrack->Fill(medLogIpSig, weight);

      histo_->jet_medLogIpSig_VS_jet_alphaMax->Fill(alphaMax, medLogIpSig, weight);
      if (sig) histo_->jet_medLogIpSig_VS_jet_alphaMax_sig->Fill(alphaMax, medLogIpSig, weight);
      if (gluond) histo_->jet_medLogIpSig_VS_jet_alphaMax_gluond->Fill(alphaMax, medLogIpSig, weight);
      if (lowntrack) histo_->jet_medLogIpSig_VS_jet_alphaMax_lowntrack->Fill(alphaMax, medLogIpSig, weight);
      if (highntrack) histo_->jet_medLogIpSig_VS_jet_alphaMax_highntrack->Fill(alphaMax, medLogIpSig, weight);

      histo_->jet_prompt_frac_E_VS_jet_alphaMax->Fill(alphaMax, prompt_frac_E, weight);
      if (sig) histo_->jet_prompt_frac_E_VS_jet_alphaMax_sig->Fill(alphaMax, prompt_frac_E, weight);
      if (gluond) histo_->jet_prompt_frac_E_VS_jet_alphaMax_gluond->Fill(alphaMax, prompt_frac_E, weight);
      if (lowntrack) histo_->jet_prompt_frac_E_VS_jet_alphaMax_lowntrack->Fill(alphaMax, prompt_frac_E, weight);
      if (highntrack) histo_->jet_prompt_frac_E_VS_jet_alphaMax_highntrack->Fill(alphaMax, prompt_frac_E, weight);

      histo_->jet_displaced_frac_VS_jet_alphaMaxNeg->Fill(alphaMaxNeg, displaced_frac, weight);
      if (sig) histo_->jet_displaced_frac_VS_jet_alphaMaxNeg_sig->Fill(alphaMaxNeg, displaced_frac, weight);
      if (gluond) histo_->jet_displaced_frac_VS_jet_alphaMaxNeg_gluond->Fill(alphaMaxNeg, displaced_frac, weight);
      if (lowntrack) histo_->jet_displaced_frac_VS_jet_alphaMaxNeg_lowntrack->Fill(alphaMaxNeg, displaced_frac, weight);
      if (highntrack) histo_->jet_displaced_frac_VS_jet_alphaMaxNeg_highntrack->Fill(alphaMaxNeg, displaced_frac, weight);
    }

    // Loop through vertices for given jet
    int nvtx = vertex_Lxy->at(ijet).size();
    histo_->jet_nVertex->Fill(nvtx, weight);
    for (unsigned ivtx=0; ivtx!=nvtx; ivtx++) {
      double source = vertex_source ->at(ijet)[ivtx];
      double x      = vertex_x      ->at(ijet)[ivtx];
      double y      = vertex_y      ->at(ijet)[ivtx];
      double z      = vertex_z      ->at(ijet)[ivtx];
      double xError = vertex_xError ->at(ijet)[ivtx];
      double yError = vertex_yError ->at(ijet)[ivtx];
      double zError = vertex_zError ->at(ijet)[ivtx];
      double deltaR = vertex_deltaR ->at(ijet)[ivtx];
      double Lxy    = vertex_Lxy    ->at(ijet)[ivtx];
      double mass   = vertex_mass   ->at(ijet)[ivtx];
      double chi2   = vertex_chi2   ->at(ijet)[ivtx];
      double ndof   = vertex_ndof   ->at(ijet)[ivtx];
      double pt2sum = vertex_pt2sum ->at(ijet)[ivtx];
      if (Lxy<0.1) continue; // :CUT: Skip vertices with Lxy<0.1cm
      if (source!=VERTEXSOURCE) continue; // :CUT: Only process vertices from specific source
      histo_ ->vertex_Lxy    ->Fill(Lxy    , weight);
      histo_ ->vertex_mass   ->Fill(mass   , weight);
      histo_ ->vertex_chi2   ->Fill(chi2   , weight);
      histo_ ->vertex_ndof   ->Fill(ndof   , weight);
      histo_ ->vertex_pt2sum ->Fill(pt2sum , weight);
      if (sig) {
        histo_ ->vertex_Lxy_sig    ->Fill(Lxy    , weight);
        histo_ ->vertex_mass_sig   ->Fill(mass   , weight);
        histo_ ->vertex_chi2_sig   ->Fill(chi2   , weight);
        histo_ ->vertex_ndof_sig   ->Fill(ndof   , weight);
        histo_ ->vertex_pt2sum_sig ->Fill(pt2sum , weight);
      }
    }
  }

  histo_->nJetPostCut->Fill(nJet, weight);

  // Event-level quantities
  if (sample_==Sample::SIGNAL || sample_==Sample::QCD) {
    // nJet >= 4 at this point
    assert(jet_pt->size()>=4);
    double deltaPt = jet_pt->at(0) - jet_pt->at(1);
    double sigmaPt = 0;
    double sigmaPt2 = 0;
    for ( unsigned i=0; i<3; i++ ) {
      float delta = jet_pt->at(i) - jet_pt->at(i+1);
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
    // std::sort(vec_medianLogIpSig.begin(), vec_medianLogIpSig.end(), std::greater<double>());
    // histo_->jet_medianLogIpSig_sorted_by_medianLogIpSig[0]->Fill( vec_medianLogIpSig[0], weight ) ;
    // histo_->jet_medianLogIpSig_sorted_by_medianLogIpSig[1]->Fill( vec_medianLogIpSig[1], weight ) ;
    // histo_->jet_medianLogIpSig_sorted_by_medianLogIpSig[2]->Fill( vec_medianLogIpSig[2], weight ) ;
    // histo_->jet_medianLogIpSig_sorted_by_medianLogIpSig[3]->Fill( vec_medianLogIpSig[3], weight ) ;
  }
}

void EmJetHistoMaker::FillPileupHistograms(long eventnumber)
{
  double weight = CalculateEventWeight(eventnumber);
  histo_nTrueInt_->Fill(nTrueInt, weight);
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

