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
  TDirectory * dir = (TDirectory*)f->Get((filename+":/emergingJetAnalyzer").c_str());
  dir->GetObject("emergingJetsTree",tree_);
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
    double pileup_lumi_weight = LumiWeights_->weight(nTrueInt);
    weight *= pileup_lumi_weight;
  }
  return weight;
}

void EmJetHistoMaker::FillJetHistograms(long eventnumber)
{
  double weight = CalculateEventWeight(eventnumber);
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
      return; // :CUT: Skip event with nJet<4
    }
  }
  // Count number of jets with alphaMax higher than cut
  {
    float cut_value = 0.05;
    int nJet_high = std::count_if(jets_alphaMax->begin(), jets_alphaMax->end(), [cut_value](float i) {return i >= cut_value;});
    int nJet_low  = nJet - nJet_high;
    histo_->nJet_by_alphaMax[0]->Fill(nJet_low, weight);
    histo_->nJet_by_alphaMax[1]->Fill(nJet_high, weight);
  }

  for (unsigned ijet=0; ijet!=jets_pt->size(); ijet++) {
    // Skip jets beyond the fourth
    if (ijet>3) break; // :CUT:
    double pt             = jets_pt             -> at(ijet);
    double eta            = jets_eta            -> at(ijet);
    double medianLogIpSig = jets_medianLogIpSig -> at(ijet);
    double alphaMax       = jets_alphaMax       -> at(ijet);
    auto& vec_pt                  = tracks_pt                  -> at(ijet);
    auto& vec_eta                 = tracks_eta                 -> at(ijet);
    auto& vec_phi                 = tracks_phi                 -> at(ijet);
    auto& vec_algo                = tracks_algo                -> at(ijet);
    auto& vec_originalAlgo        = tracks_originalAlgo        -> at(ijet);
    auto& vec_nHits               = tracks_nHits               -> at(ijet);
    auto& vec_nMissHits           = tracks_nMissInnerHits      -> at(ijet);
    auto& vec_nTrkLayers          = tracks_nTrkLayers          -> at(ijet);
    auto& vec_nMissInnerTrkLayers = tracks_nMissInnerTrkLayers -> at(ijet);
    auto& vec_nMissOuterTrkLayers = tracks_nMissOuterTrkLayers -> at(ijet);
    auto& vec_nMissTrkLayers      = tracks_nMissTrkLayers      -> at(ijet);
    auto& vec_nPxlLayers          = tracks_nPxlLayers          -> at(ijet);
    auto& vec_nMissInnerPxlLayers = tracks_nMissInnerPxlLayers -> at(ijet);
    auto& vec_nMissOuterPxlLayers = tracks_nMissOuterPxlLayers -> at(ijet);
    auto& vec_nMissPxlLayers      = tracks_nMissPxlLayers      -> at(ijet);
    auto& vec_dRToJetAxis         = tracks_dRToJetAxis         -> at(ijet);
    auto& vec_distanceToJet       = tracks_distanceToJet       -> at(ijet);
    auto& vec_ipXY                = tracks_ipXY                -> at(ijet);
    auto& vec_ipXYSig             = tracks_ipXYSig             -> at(ijet);
    int nTracks = vec_ipXY.size();
    if (nTracks==0) continue; // :CUT: Skip jets with zero tracks
    if (TMath::Abs(eta) > 2.5) continue; // :CUT: Skip jets with |eta|>2.5
    int nDarkPions = 0;
    bool sig = false;
    if (isSignal_) {
      nDarkPions = jets_nDarkPions->at(ijet);
      if (nDarkPions > 0) sig = true;
    }

    {
      const float ipSig_cut_values [4] = {2.0, 5.0, 10.0, 20.0};
      for (int icut=0; icut<4; icut++)
        {
          // Count number of tracks with ipXYSig above and below cut value
          float cut_value = ipSig_cut_values[icut];
          int nTracks_high = std::count_if(vec_ipXYSig.begin(), vec_ipXYSig.end(), [cut_value](float i) {return i >= cut_value;});
          int nTracks_low  = nTracks - nTracks_high;
          histo_->jet_nTracks_by_ipSig_cut_low[icut]->Fill(nTracks_low, weight);
          histo_->jet_nTracks_by_ipSig_cut_high[icut]->Fill(nTracks_high, weight);
          if (sig) {
            histo_->jet_nTracks_by_ipSig_cut_low_sig[icut]->Fill(nTracks_low, weight);
            histo_->jet_nTracks_by_ipSig_cut_high_sig[icut]->Fill(nTracks_high, weight);
          }
        }
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

    histo_->jet_medianLogIpSig_VS_jet_alphaMax->Fill(alphaMax, medianLogIpSig, weight);
    if (sig) histo_->jet_medianLogIpSig_VS_jet_alphaMax_sig->Fill(alphaMax, medianLogIpSig, weight);

    ht += pt;
    sumMedianLogIpSig += medianLogIpSig;
    vec_medianLogIpSig.push_back(medianLogIpSig);

    // Loop through tracks for given jet
    vector<int> index_ipXYSig_ordered;
    for (unsigned itk=0; itk!=vec_ipXY.size(); itk++) {
      string name; string prefix =""; string postfix="";
      index_ipXYSig_ordered.push_back(itk);
      // int nHits = vec_nHits[itk];
      // int nMissHits = vec_nMissHits[itk];
#define GET_TRACK_VAR_FROM_VEC(var) double var = vec_##var[itk]
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

    histo_->jet_medLogIpSig->Fill(medLogIpSig, weight);
    if (sig) histo_->jet_medLogIpSig_sig->Fill(medLogIpSig, weight);

    // Loop through vertices for given jet
    int nvtx = jet_vertex_Lxy->at(ijet).size();
    histo_->jet_nVertex->Fill(nvtx, weight);
    for (unsigned ivtx=0; ivtx!=nvtx; ivtx++) {
      double source = jet_vertex_source ->at(ijet)[ivtx];
      double x      = jet_vertex_x      ->at(ijet)[ivtx];
      double y      = jet_vertex_y      ->at(ijet)[ivtx];
      double z      = jet_vertex_z      ->at(ijet)[ivtx];
      double xError = jet_vertex_xError ->at(ijet)[ivtx];
      double yError = jet_vertex_yError ->at(ijet)[ivtx];
      double zError = jet_vertex_zError ->at(ijet)[ivtx];
      double deltaR = jet_vertex_deltaR ->at(ijet)[ivtx];
      double Lxy    = jet_vertex_Lxy    ->at(ijet)[ivtx];
      double mass   = jet_vertex_mass   ->at(ijet)[ivtx];
      double chi2   = jet_vertex_chi2   ->at(ijet)[ivtx];
      double ndof   = jet_vertex_ndof   ->at(ijet)[ivtx];
      double pt2sum = jet_vertex_pt2sum ->at(ijet)[ivtx];
      if (Lxy<0.1) continue; // :CUT: Skip vertices with Lxy<0.1cm
      histo_ ->jet_vertex_Lxy    ->Fill(Lxy    , weight);
      histo_ ->jet_vertex_mass   ->Fill(mass   , weight);
      histo_ ->jet_vertex_chi2   ->Fill(chi2   , weight);
      histo_ ->jet_vertex_ndof   ->Fill(ndof   , weight);
      histo_ ->jet_vertex_pt2sum ->Fill(pt2sum , weight);
      if (sig) {
        histo_ ->jet_vertex_Lxy_sig    ->Fill(Lxy    , weight);
        histo_ ->jet_vertex_mass_sig   ->Fill(mass   , weight);
        histo_ ->jet_vertex_chi2_sig   ->Fill(chi2   , weight);
        histo_ ->jet_vertex_ndof_sig   ->Fill(ndof   , weight);
        histo_ ->jet_vertex_pt2sum_sig ->Fill(pt2sum , weight);
      }
    }
  }

  // Event-level quantities
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
    histo_->jet_medianLogIpSig_sorted_by_medianLogIpSig[0]->Fill( vec_medianLogIpSig[0], weight ) ;
    histo_->jet_medianLogIpSig_sorted_by_medianLogIpSig[1]->Fill( vec_medianLogIpSig[1], weight ) ;
    histo_->jet_medianLogIpSig_sorted_by_medianLogIpSig[2]->Fill( vec_medianLogIpSig[2], weight ) ;
    histo_->jet_medianLogIpSig_sorted_by_medianLogIpSig[3]->Fill( vec_medianLogIpSig[3], weight ) ;
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

