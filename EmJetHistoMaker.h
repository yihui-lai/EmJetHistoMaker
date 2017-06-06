#include "HistoMakerBase.h"
#include "EmJetHistos.h"
#include "EmJetSample.h"
#include "LumiReWeightingStandAlone.h"

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TStopwatch.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TMath.h>
#include <TParameter.h>

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

#ifndef OUTPUT
#define OUTPUT(x) std::cout<<#x << ": " << x << std::endl
#endif

const int debug = 0;

class EmJetHistos;
class EmJetSample;
typedef EmJetHistos Histos;
const int nTrackSort=50;
enum class Sample {SIGNAL, QCD, WJET};

class EmJetHistoMaker : public HistoMakerBase
{
 public:
  EmJetHistoMaker();
  EmJetHistoMaker(string ifilename);
  EmJetHistoMaker(EmJetSample isample);
  ~EmJetHistoMaker() {};
  int TRACKSOURCE;
  int VERTEXSOURCE;
  void InitHistograms();
  void FillEventCount (long eventCount1, long eventCount2);
  void FillHistograms    (long eventnumber);
  void FillEventHistograms  (long eventnumber, string tag, bool printout);
  void FillNJetHistograms(long eventnumber, vector<int> vjetindex, string tag);
  void FillJetHistograms_pT (long eventnumber, int ij, string tag);
  void FillJetHistograms    (long eventnumber, int ij, string tag);
  void FillTrackHistograms  (long eventnumber, int ij, int itk, string tag);
  void FillPileupHistograms (long eventnumber, string tag);
  void PrintEvent (long eventnumber, string comment);
  int SetTree(string ifilename);
  int SetTree();
  int GetEventCountHistAndClone(string ihistname);
  int GetEventCount(string ihistname);
  void SetOptions(Sample sample=Sample::SIGNAL, bool isData=false, double xsec=1.0, long nevent=1, bool isSignal=false, bool pileupOnly=false);
  void InitLumiReweighting();
 private:
  double CalculateEventWeight(long eventnumber);
  bool JetFilter(long eventnumber);
  bool PVRecoSuccess(long eventnumber);
  void FillBquarkIndex(long eventnumber);
  float DeltaR(float eta1, float phi1, float eta2, float phi2);
  bool BTaggingPass(int ij);
  int FlavourTagging(int ij);
  double GetAlpha(int ij);
  double GetAlpha2D(int ij);
  double GetAlpha2DSig(int ij);
  double GetLTKFrac(int ij);
  double GetNonPUFrac(int ij);
  double GetFrac2DSig(int ij);
  int GetNTracks(int ij);
  bool SelectJet_basic(int jet_index);
  bool SelectJet_alphaMax(int jet_index);
  bool SelectJet_alphaMax2(int jet_index);
  bool SelectJet_ipCut(int jet_index);
  unique_ptr<TTree> ntree_;
  unique_ptr<Histos> histo_;
  unique_ptr<TH1F> histo_nTrueInt_;
  unique_ptr<TH1D> histo_eventCountperrun_;
  Sample sample_;
  bool isData_;
  double xsec_;
  double nevent_;
  bool isSignal_;
  bool pileupOnly_;
  vector<double> jet_Alpha;
  vector<double> jet_Alpha2DSig;
  vector<double> jet_Alpha2D;
  vector<double> jet_ltkfrac;
  vector<double> jet_nonpufrac;
  vector<double> jet_frac2DSig;
  vector<int>    jet_flavour;
  vector<int>    jet_NTracks;
  vector<int> BquarkIndex;
  string file_; // Path to current input file
  unique_ptr<reweight::LumiReWeighting> LumiWeights_;
  // Calculated variables
  double event_ht_;
};

EmJetHistoMaker::EmJetHistoMaker()
{
  SetMaxEntries(-1);
  std::cout << "EmJetHistoMaker::EmJetHistoMaker()" << std::endl;
  TRACKSOURCE = 0;
  InitLumiReweighting();
}

EmJetHistoMaker::EmJetHistoMaker(string ifilename)
{
  SetMaxEntries(-1);
  std::cout << "EmJetHistoMaker::EmJetHistoMaker(\"" << ifilename << "\")" << std::endl;
  file_ = ifilename;
  int status = SetTree(ifilename);
  if (!status==0) std::cerr << "Error when opening file: " << ifilename << std::endl;
  TRACKSOURCE = 0;
  InitLumiReweighting();
}

EmJetHistoMaker::EmJetHistoMaker(EmJetSample isample)
{
  SetMaxEntries(-1);
  std::cout << "EmJetHistoMaker::EmJetHistoMaker(" << isample.name << ")" << std::endl;
  TChain* chain = new TChain("emJetAnalyzer/emJetTree");
  for (string file : isample.files) {
    OUTPUT(file);
    chain->Add(file.c_str(), -1); // -1: the file is connected and the tree header read in memory to get the number of entries.
  }
  tree_ = chain;
  Init(tree_);
}

void EmJetHistoMaker::InitLumiReweighting()
{
  // Initialize lumi reweighting utility
  std::string mcFile   = "~/www/2016-03-23/pileup_mc_2015_25ns_Startup_PoissonOOTPU.root";
  std::string dataFile = "~/www/2016-03-21/pileup-DataSkim-20160302.root";
  std::string mcHist   = "nTrueInt";
  std::string dataHist = "pileup";
  LumiWeights_ = unique_ptr<reweight::LumiReWeighting>(new reweight::LumiReWeighting(mcFile, dataFile, mcHist, dataHist));
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

int EmJetHistoMaker::SetTree()
{
  string filename = file_;
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

int EmJetHistoMaker::GetEventCountHistAndClone(string ihistname)
{
  if (isData_) {
    std::cerr << "Error: Attempting to get event count histogram for data\n";
    return -1;
  }
  string filename = file_;
  TFile *f = new TFile(filename.c_str());
  // File read error
  if (f->IsZombie()) {
    std::cout << "File read error for file: " << filename << std::endl;
    return 1;
  }
  TDirectory * dir = (TDirectory*)f->Get((filename+":/"+ihistname).c_str());
  TH1F* eventcounthist = (TH1F*)dir->Get(ihistname.c_str())->Clone();
  eventcounthist->SetDirectory(ofile_);
  eventcounthist->Write();
  return 0; // No error
}

int EmJetHistoMaker::GetEventCount(string ihistname)
{
  if (isData_) {
    std::cerr << "Error: Attempting to get event count histogram for data\n";
    return -1;
  }
  string filename = file_;
  TFile *f = new TFile(filename.c_str());
  // File read error
  if (f->IsZombie()) {
    std::cout << "File read error for file: " << filename << std::endl;
    return -1;
  }
  TDirectory * dir = (TDirectory*)f->Get((filename+":/"+ihistname).c_str());
  TH1F* eventcounthist = (TH1F*)dir->Get(ihistname.c_str());
  if (eventcounthist) return eventcounthist->Integral();
  return -1;
}

void EmJetHistoMaker::InitHistograms()
{
  TH1::SetDefaultSumw2();
  histo_nTrueInt_ = unique_ptr<TH1F>(new TH1F("nTrueInt", "nTrueInt", 100, 0., 100.));
  if (!pileupOnly_) {
    histo_ = unique_ptr<Histos>(new Histos());
  }
}

void EmJetHistoMaker::FillEventCount(long eventCount1, long eventCount2)
{
//  ntree_ = unique_ptr<TTree>(new TTree("ntree_", "to store list info"));
//  auto a = new TParameter<long> ("eventCount1", eventCount1, 'M');
  //auto b = new TParameter<long> ("eventCount2", eventCount2, '+');
//  std::cout<< "Total Number of Events:     " << std::setw(12) << eventCount1 <<std::endl;
  std::cout<< "Number of Processed Events: " << std::setw(12) << eventCount2 <<std::endl;
  //ntree_->GetUserInfo()->AddLast( a );
  //ntree_->GetUserInfo()->AddLast( b );
  histo_eventCountperrun_ = unique_ptr<TH1D>(new TH1D("eventCountperrun", "eventCountperrun", 2, 0., 2.));
  for(int i=0; i< eventCount2; i++){
     histo_eventCountperrun_->Fill(1.);
  }
}

void EmJetHistoMaker::FillHistograms(long eventnumber)
{
  double w = CalculateEventWeight(eventnumber);
  if (debug==1) std::cout << "Entering FillHistograms" << std::endl;

  FillPileupHistograms(eventnumber, "");
  if (pileupOnly_) return;

  if( HLT_PFHT800==0 ) return;
  if( pv_indexInColl!=0 ) return;
  if( JetFilter(eventnumber) )    FillEventHistograms(eventnumber, "", false);
  //if( JetFilter(eventnumber) && fabs(pv_z)<=15.0 && PVRecoSuccess(eventnumber) ) FillEventHistograms(eventnumber, "__PVfiltered", true);
  if( JetFilter(eventnumber) && fabs(pv_z)<=15.0 && !PVRecoSuccess(eventnumber)) std::cout << "pv filtered: "<< event << "\t" << run << "\t" << lumi <<std::endl; 
}

void EmJetHistoMaker::FillEventHistograms(long eventnumber, string tag, bool printout)
{
  if (debug==1) std::cout << "Entering FillEventHistograms" << std::endl;

  double w = CalculateEventWeight(eventnumber);

  // Calculate ht
  double ht = 0;
  double ht4 = 0;
  for (unsigned ij = 0; ij < (*jet_pt).size(); ij++) {
    ht += (*jet_pt)[ij];
    if (ij<4) ht4 += (*jet_pt)[ij];
  }
  event_ht_ = ht;

  //calculate useful variables
  if(!jet_Alpha.empty()) jet_Alpha.clear();
  if(!jet_Alpha2D.empty()) jet_Alpha2D.clear();
  if(!jet_Alpha2DSig.empty()) jet_Alpha2DSig.clear();
  if(!jet_ltkfrac.empty()) jet_ltkfrac.clear();
  if(!jet_nonpufrac.empty()) jet_nonpufrac.clear();
  if(!jet_frac2DSig.empty()) jet_frac2DSig.clear();
  if(!jet_flavour.empty()) jet_flavour.clear();
  if(!jet_NTracks.empty()) jet_NTracks.clear();

  for (unsigned ij = 0; ij< (*jet_pt).size(); ij++){
    double Alpha =  GetAlpha(ij);// with track quality cut, deltaR matching and PV weight
    double Alpha2D = GetAlpha2D(ij);//with track quality cut, deltaR matching and ipXY cut
    double Alpha2DSig = GetAlpha2DSig(ij);//with track quality cut, deltaR matching and ipXYSig cut
    jet_Alpha.push_back(Alpha);
    jet_Alpha2D.push_back(Alpha2D);
    jet_Alpha2DSig.push_back(Alpha2DSig);

    double ltkfrac = GetLTKFrac(ij);
    double nonpufrac = GetNonPUFrac(ij);
    double frac2DSig = GetFrac2DSig(ij);
    jet_ltkfrac.push_back(ltkfrac);
    jet_nonpufrac.push_back(nonpufrac);
    jet_frac2DSig.push_back(frac2DSig);
  
    int flavour = FlavourTagging(ij);
    jet_flavour.push_back(flavour);
    int ntracks = GetNTracks(ij);
    jet_NTracks.push_back(ntracks);
  }

  //DEBUG
  if( jet_Alpha.size()!=(*jet_pt).size() || jet_Alpha2D.size()!=(*jet_pt).size() || jet_Alpha2DSig.size()!=(*jet_pt).size() || jet_ltkfrac.size()!=(*jet_pt).size() || jet_nonpufrac.size()!=(*jet_pt).size() || jet_frac2DSig.size()!=(*jet_pt).size() || jet_flavour.size()!=(*jet_pt).size() || jet_NTracks.size()!=(*jet_pt).size() ){
    std::cout << "Jet Alpha Store Problem!!!!" << std::endl;
  }

  unsigned int nJet_basic = 0, nJet_basic3 = 0, nJet_basic4 =0 ;
  vector<int> pos_basicJT;
  vector<int> pos_basicJT3;
  vector<int> pos_basicJT4;
  // Jet loop
  for (unsigned ij = 0; ij < (*jet_pt).size(); ij++) {
    if (ij>=4) break; // :JETCUT:
    FillJetHistograms_pT(eventnumber, ij, ""+tag);

    if (SelectJet_basic(ij)) {
      FillJetHistograms_pT(eventnumber, ij, "__JTbasic"+tag);
      //if( printout ) std::cout << "eventnumber " << event << " jet " << ij << " tagged as " << jet_flavour[ij] << std::endl;
      nJet_basic++;
      pos_basicJT.push_back(ij);
      if ( jet_Alpha2DSig[ij]<0.3 ) {
        FillJetHistograms_pT(eventnumber, ij, "__JTAlpha2DSig"+tag);
        if( jet_frac2DSig[ij]<0.3 ){
          FillJetHistograms_pT(eventnumber, ij, "__JTAF2DSig"+tag);
        }
      }

      if ( jet_ltkfrac[ij]<0.6 && jet_nonpufrac[ij]>0.4 ){
        FillJetHistograms_pT(eventnumber, ij, "__JTbasic3"+tag);
        pos_basicJT3.push_back(ij);
        nJet_basic3++;
	if ( jet_Alpha2DSig[ij]<0.3 ){
          FillJetHistograms_pT(eventnumber, ij, "__JTAlpha2DSig3"+tag);
          if( jet_frac2DSig[ij]<0.3 ){
            FillJetHistograms_pT(eventnumber, ij, "__JTAF2DSig3"+tag);
          } 
	}

        if( jet_NTracks[ij]>5 ){
          FillJetHistograms_pT(eventnumber, ij, "__JTbasic4"+tag);
          pos_basicJT4.push_back(ij);
          nJet_basic4++;
          if( jet_Alpha2DSig[ij]<0.3 && jet_frac2DSig[ij]<0.3 ){
            FillJetHistograms_pT(eventnumber, ij, "__JTAF2DSig4"+tag);
          }
        }
      }
    }
  }
  
  // Existing quantities (in ntuple)
  histo_->hist1d["nVtx"+tag]->Fill(nVtx, w);

  // Calculated quantities
  histo_->hist1d["ht"+tag]->Fill(ht, w);
  histo_->hist1d["ht4"+tag]->Fill(ht4, w);
  histo_->hist1d["pv_indexInColl"+tag]->Fill(pv_indexInColl, w);
  histo_->hist1d["pv_z"+tag]->Fill(pv_z, w);
 
  // debug 
  if( pos_basicJT.size()  != nJet_basic )  std::cout << "WARNING!!! jet multiplicity does not match" << std::endl;
  if( pos_basicJT3.size() != nJet_basic3 ) std::cout << "WARNING!! third type of basic jet multiplicity does not match" << std::endl;
  if( pos_basicJT4.size() != nJet_basic4 ) std::cout << "WARNING!! fourth type of basic jet multiplicity does not match" << std::endl;
 

  // second type of tag and probe to measure fakerate
  for( auto &jindex: pos_basicJT){
    if( jet_Alpha2DSig[jindex]>0.3 || (*jet_pt)[jindex]<50.0 ) continue;//tag
    for( auto &jindex2: pos_basicJT ){ // probe
      if( jindex2==jindex ) continue;
      FillJetHistograms_pT(eventnumber, jindex2, "__JTbasic__TypeIII"+tag);
      if( jet_Alpha2DSig[jindex2]<0.3 ){
         FillJetHistograms_pT(eventnumber, jindex2, "__JTAlpha__TypeIII"+tag);
      }
    }
  }

  for( auto &jindex: pos_basicJT){
    if( jet_Alpha2DSig[jindex]>0.3 || jet_frac2DSig[jindex]>0.3 || (*jet_pt)[jindex]<50.0 ) continue;//tag
    for( auto &jindex2: pos_basicJT ){ // probe
      if( jindex2==jindex ) continue;
      FillJetHistograms_pT(eventnumber, jindex2, "__JTbasic__TypeIV"+tag);
      if( jet_Alpha2DSig[jindex2]<0.3 && jet_frac2DSig[jindex2]<0.3 ){
         FillJetHistograms_pT(eventnumber, jindex2, "__JTAlpha__TypeIV"+tag);
      }
    }
  }

  bool interestingevent = false;
  for( auto &jindex: pos_basicJT3){
    if( jet_Alpha2DSig[jindex]>0.3 || (*jet_pt)[jindex]<50.0 ) continue;//tag
    for( auto &jindex2: pos_basicJT3 ){ // probe
      if( jindex2==jindex ) continue;
      FillJetHistograms_pT(eventnumber, jindex2, "__JTbasic__TypeVII"+tag);
      if( jet_Alpha2DSig[jindex2]<0.3 ){
         FillJetHistograms_pT(eventnumber, jindex2, "__JTAlpha__TypeVII"+tag);
         interestingevent = true;
      }
    }
  }

  for( auto &jindex: pos_basicJT3){
    if( jet_Alpha2DSig[jindex]>0.3 || jet_frac2DSig[jindex]>0.3 || (*jet_pt)[jindex]<50.0 ) continue;//tag
    for( auto &jindex2: pos_basicJT3 ){ // probe
      if( jindex2==jindex ) continue;
      FillJetHistograms_pT(eventnumber, jindex2, "__JTbasic__TypeVIII"+tag);
      if( jet_Alpha2DSig[jindex2]<0.3 && jet_frac2DSig[jindex2]<0.3 ){
         FillJetHistograms_pT(eventnumber, jindex2, "__JTAlpha__TypeVIII"+tag);
      }
    }
  }
 
  for( auto &jindex: pos_basicJT4){
    if( jet_Alpha2DSig[jindex]>0.3 || jet_frac2DSig[jindex]>0.3 || (*jet_pt)[jindex]<50.0 ) continue;//tag
    for( auto &jindex2: pos_basicJT4 ){ // probe
      if( jindex2==jindex ) continue;
      FillJetHistograms_pT(eventnumber, jindex2, "__JTbasic__TypeX"+tag);
      if( jet_Alpha2DSig[jindex2]<0.3 && jet_frac2DSig[jindex2]<0.3 ){
         FillJetHistograms_pT(eventnumber, jindex2, "__JTAlpha__TypeX"+tag);
      }
    }
  }

  if(printout && interestingevent) std::cout << "Alpha2DSig tag " << "eventnumber : " << event << " run: " << run << " lumi "<< lumi <<" jet_Alpha2DSig: " << jet_Alpha2DSig[0] << "\t" << jet_Alpha2DSig[1] << "\t" << jet_Alpha2DSig[2] << "\t" << jet_Alpha2DSig[3] << std::endl;
}


void EmJetHistoMaker::FillNJetHistograms(long eventnumber, vector<int> vjetindex, string tag)
{
  double w = CalculateEventWeight(eventnumber);  
  int nJet = vjetindex.size();
  for(auto &ij: vjetindex){
    histo_->hist2d[string()+"jet_N"+"_VS_"+"jet_pt__JTbasic"+tag]->Fill((*jet_pt)[ij], nJet, w);
    histo_->hist2d[string()+"jet_N"+"_VS_"+"jet_eta__JTbasic"+tag]->Fill((*jet_eta)[ij], nJet, w);
    histo_->hist2d[string()+"jet_N"+"_VS_"+"jet_phi__JTbasic"+tag]->Fill((*jet_phi)[ij], nJet, w);
  }
}

void EmJetHistoMaker::FillJetHistograms_pT(long eventnumber, int ij, string tag)
{
  FillJetHistograms(eventnumber, ij, tag);
  if( (*jet_pt)[ij]<100.0 )                         FillJetHistograms(eventnumber, ij, "__pt0"+tag);
  if( (*jet_pt)[ij]>=100.0 )                        FillJetHistograms(eventnumber, ij, "__pt1"+tag);
  string ftag = "__"+std::to_string(jet_flavour[ij]) + tag;
  FillJetHistograms(eventnumber, ij, ftag);
  if( (*jet_pt)[ij]<100.0 )                         FillJetHistograms(eventnumber, ij, "__pt0"+ftag);
  if( (*jet_pt)[ij]>=100.0 )                        FillJetHistograms(eventnumber, ij, "__pt1"+ftag);
}

void EmJetHistoMaker::FillJetHistograms(long eventnumber, int ij, string tag)
{
  if (debug==1) std::cout << "Entering FillJetHistograms" << std::endl;
  double w = CalculateEventWeight(eventnumber);

  //double ipXYcut = 0.025;
  // Calculate median 2D impact parameter (source=0, high quality)
  int nTrack = 0;
  int nTrack_pt300 = 0;
  int nTrack_pt500 = 0;
  int nTrack_pt1000 = 0;
  double medianIP = 0.;
  double maxIP = 0.;
  {
    vector<double> vector_ipXY;
    for (unsigned itk=0; itk < (*track_pt)[ij].size(); itk++) {
      if ( (*track_source)[ij][itk] == 0 &&  ((*track_quality)[ij][itk] & 4)>0 ) {
        vector_ipXY.push_back( (*track_ipXY)[ij][itk] );
        FillTrackHistograms(eventnumber, ij, itk, tag);
        if( (*track_pt)[ij][itk]>0.3 ) nTrack_pt300++;
        if( (*track_pt)[ij][itk]>0.5 ) nTrack_pt500++;
        if( (*track_pt)[ij][itk]>1.0 ) nTrack_pt1000++;
      }
    }
    std::sort(vector_ipXY.begin(), vector_ipXY.end());
    nTrack = vector_ipXY.size();
    medianIP = 0.;
    if (nTrack>0) {
      if ( nTrack % 2 == 0 ) {
        medianIP = (vector_ipXY[nTrack/2 - 1] + vector_ipXY[nTrack/2]) / 2;
      }
      else {
        medianIP = (vector_ipXY[nTrack/2]);
      }
      maxIP = vector_ipXY[nTrack-1];
    }
  }

  // Calculate median 2D impact parameter (source=0, pt>1)
  int nTrackPostCut = 0;
  double medianIPPostCut = 0.;
  {
    vector<double> vector_ipXY;
    for (unsigned itk=0; itk < (*track_pt)[ij].size(); itk++) {
      if ( (*track_source)[ij][itk] == 0 && (*track_pt)[ij][itk] >= 1.0 ) {
        vector_ipXY.push_back( (*track_ipXY)[ij][itk] );
        //FillTrackHistograms(eventnumber, ij, itk, tag);
      }
    }
    std::sort(vector_ipXY.begin(), vector_ipXY.end());
    int nTrack = vector_ipXY.size();
    double medianIP = 0.;
    if (nTrack>0) {
      if ( nTrack % 2 == 0 ) {
        medianIP = (vector_ipXY[nTrack/2 - 1] + vector_ipXY[nTrack/2]) / 2;
      }
      else {
        medianIP = (vector_ipXY[nTrack/2]);
      }
    }
    nTrackPostCut = nTrack;
    medianIPPostCut = medianIP;
  }

  // Calculate prompt/displaced energy fraction (source=0)
  double prompt_frac = 0;
  double disp_frac = 0;
  {
    double prompt_sum = 0;
    double disp_sum = 0;
    for (unsigned itk=0; itk < (*track_pt)[ij].size(); itk++) {
      if ( (*track_source)[ij][itk] == 0 ) {
        if ( (*track_ipXY)[ij][itk] > 0.1 ) {
          disp_sum += (*track_pt)[ij][itk];
        }
        else {
          prompt_sum += (*track_pt)[ij][itk];
        }
      }
    }
    prompt_frac = prompt_sum / (prompt_sum + disp_sum);
    disp_frac = disp_sum / (prompt_sum + disp_sum);
  }

  // Calculate missing hit track energy fraction (source=0)
  double missInnerHit_frac = 0;
  {
    double missInnerHit_sum = 0;
    double sum = 0;
    for (unsigned itk=0; itk < (*track_pt)[ij].size(); itk++) {
      if ( (*track_source)[ij][itk] == 0 ) {
        if ( (*track_nMissInnerHits)[ij][itk] >= 1 ) {
          missInnerHit_sum += (*track_pt)[ij][itk];
        }
        sum += (*track_pt)[ij][itk];
      }
    }
    missInnerHit_frac = missInnerHit_sum / sum;
  }

  // Existing quantities (in ntuple)
  histo_->hist1d["jet_pt"+tag]->Fill((*jet_pt)[ij], w);
  histo_->hist1d["jet_eta"+tag]->Fill((*jet_eta)[ij], w);
  histo_->hist1d["jet_phi"+tag]->Fill((*jet_phi)[ij], w);
  histo_->hist1d["jet_alphaMax"+tag]->Fill((*jet_alphaMax)[ij], w);
  histo_->hist1d["jet_cef"+tag]->Fill((*jet_cef)[ij], w);
  histo_->hist1d["jet_nef"+tag]->Fill((*jet_nef)[ij], w);

  // Calculated quantities
  histo_->hist1d["jet_Alpha"+tag]->Fill(jet_Alpha[ij], w);
  histo_->hist1d["jet_Alpha2D"+tag]->Fill(jet_Alpha2D[ij], w);
  histo_->hist1d["jet_Alpha2DSig"+tag]->Fill(jet_Alpha2DSig[ij], w);
  histo_->hist1d["jet_ltkfrac"+tag]->Fill(jet_ltkfrac[ij], w);
  histo_->hist1d["jet_nonpufrac"+tag]->Fill(jet_nonpufrac[ij], w);
  histo_->hist1d["jet_frac2DSig"+tag]->Fill(jet_frac2DSig[ij], w);
  histo_->hist1d["jet_nTrack"+tag]->Fill(nTrack, w);
  histo_->hist1d["jet_nTrack_pt1000"+tag]->Fill(nTrack_pt1000, w);
  histo_->hist1d["jet_maxIP"+tag]->Fill(maxIP, w);
  histo_->hist1d["jet_medianIP"+tag]->Fill(medianIP, w);
  histo_->hist1d["jet_prompt_frac"+tag]->Fill(prompt_frac, w);
  histo_->hist1d["jet_disp_frac"+tag]->Fill(disp_frac, w);

  // 2D histos
  /*
  histo_->hist2d[string()+"jet_disp_frac"+"_VS_"+"jet_alphaMax"+tag]->Fill((*jet_alphaMax)[ij], disp_frac, w);
  histo_->hist2d[string()+"jet_disp_frac"+"_VS_"+"jet_pt"+tag]->Fill((*jet_pt)[ij], disp_frac, w);
  histo_->hist2d[string()+"jet_alphaMax"+"_VS_"+"jet_pt"+tag]->Fill((*jet_pt)[ij], (*jet_alphaMax)[ij], w);
  histo_->hist2d[string()+"jet_alphaMax2"+"_VS_"+"jet_pt"+tag]->Fill((*jet_pt)[ij], (*jet_alphaMax2)[ij], w);
  histo_->hist2d[string()+"jet_Alpha"+"_VS_"+"jet_pt"+tag]->Fill((*jet_pt)[ij], jet_Alpha[ij], w);
  histo_->hist2d[string()+"jet_alphaMax"+"_VS_"+"ht"+tag]->Fill(event_ht_, (*jet_alphaMax)[ij], w);
  histo_->hist2d[string()+"jet_alphaMax"+"_VS_"+"jet_alphaMax2"+tag]->Fill((*jet_alphaMax2)[ij], (*jet_alphaMax)[ij], w);
  histo_->hist2d[string()+"jet_alphaMax"+"_VS_"+"nVtx"+tag]->Fill(nVtx, (*jet_alphaMax)[ij], w);
  histo_->hist2d[string()+"pv_indexInColl"+"_VS_"+"jet_alphaMax"+tag]->Fill((*jet_alphaMax)[ij], pv_indexInColl, w);
  histo_->hist2d[string()+"pv_indexInColl"+"_VS_"+"jet_alphaMax2"+tag]->Fill((*jet_alphaMax2)[ij], pv_indexInColl, w);
  histo_->hist2d[string()+"jet_maxIP"+"_VS_"+"jet_alphaMax"+tag]->Fill((*jet_alphaMax)[ij], maxIP, w);
  histo_->hist2d[string()+"jet_nTrackPostCut"+"_VS_"+"jet_pt"+tag]->Fill((*jet_pt)[ij], nTrack, w);  
  histo_->hist2d[string()+"jet_nTrack"+"_VS_"+"jet_pt"+tag]->Fill((*jet_pt)[ij], nTrackPostCut, w);  
  */
}

void EmJetHistoMaker::FillTrackHistograms(long eventnumber, int ij, int itk, string tag)
{
  if (debug==1) std::cout << "Entering FillTrackHistograms" << std::endl;
  if (debug==2) OUTPUT(tag);

  double w = CalculateEventWeight(eventnumber);

  // Existing quantities (in ntuple)
  histo_->hist1d["track_pt"+tag]->Fill((*track_pt)[ij][itk], w);
  histo_->hist1d["track_eta"+tag]->Fill((*track_eta)[ij][itk], w);
  histo_->hist1d["track_phi"+tag]->Fill((*track_phi)[ij][itk], w);
  histo_->hist1d["track_quality"+tag]->Fill((*track_quality)[ij][itk], w);
  histo_->hist1d["track_ipXY"+tag]->Fill((*track_ipXY)[ij][itk], w);
  histo_->hist1d["track_ipXYSig"+tag]->Fill((*track_ipXYSig)[ij][itk], w);
}

void EmJetHistoMaker::FillPileupHistograms(long eventnumber, string tag)
{
  double weight = CalculateEventWeight(eventnumber);
  histo_nTrueInt_->Fill(nTrueInt, weight);
}
 
double EmJetHistoMaker::CalculateEventWeight(long eventnumber)
{
  double weight = 0.0;
  if (isData_) weight = 1.0;
  else {
    weight = 1.0;
    double generator_weight = xsec_/nevent_;
    // double generator_weight = xsec_;
    weight *= generator_weight;
    // double pileup_lumi_weight = LumiWeights_->weight(nTrueInt);
    // weight *= pileup_lumi_weight;
  }
  //return weight;
  weight = 1.0;
  return weight;//normalize the histogram in the end
}

bool EmJetHistoMaker::JetFilter(long eventnumber)
{
  double maxEta[4] = {2.5, 2.5, 2.5, 2.5};
  double minPt[4] = {400.0, 200.0, 125.0, 50.0};
  int nJetsPassing = 0;
  bool EventPass = false;

  for (unsigned ij = 0; ij < (*jet_pt).size() && nJetsPassing<4 ; ij++) {
    if( fabs((*jet_eta)[ij]) < maxEta[nJetsPassing] && (*jet_pt)[ij] > minPt[nJetsPassing] ) nJetsPassing++;
    if( nJetsPassing>3 ) EventPass = true;
  }

  return EventPass;
}

bool EmJetHistoMaker::PVRecoSuccess(long eventnumber)
{
  if( (*jet_pt).size()<4 ){
    std::cout << "problem with JetFilter!!! Events with less than 4 jets have passed!!" << std::endl;
    return false;
  }
  bool pvreco = true; 
  if( (*jet_alphaMax)[0]<0.04 && (*jet_alphaMax)[1]<0.04 && (*jet_alphaMax)[2]<0.04 && (*jet_alphaMax)[3]<0.04 ) pvreco=false;
  return pvreco;
}

double EmJetHistoMaker::GetAlpha(int ij) // Calculate alpha for given jet
{
  double ptsum_total=0, ptsum=0;
  for (unsigned itk=0; itk < (*track_pt)[ij].size(); itk++) {
    if ( (*track_source)[ij][itk] != 0 ) continue; // Only process tracks with source=0
    if ( ( (*track_quality)[ij][itk] & 4 ) == 0 ) continue; // Only process tracks with "highPurity" quality

    ptsum_total += (*track_pt)[ij][itk];
    if ( (*track_pvWeight)[ij][itk] > 0 ) ptsum += (*track_pt)[ij][itk];//PV weight matching
  }

  double alpha = ptsum/ptsum_total;
  return alpha;
}

double EmJetHistoMaker::GetAlpha2D(int ij) // Calculate alpha for given jet
{
  double ptsum_total=0, ptsum=0;
  for (unsigned itk=0; itk < (*track_pt)[ij].size(); itk++) {
    if ( (*track_source)[ij][itk] != 0 ) continue; // Only process tracks with source=0
    if ( ( (*track_quality)[ij][itk] & 4 ) == 0 ) continue; // Only process tracks with "highPurity" quality

    ptsum_total += (*track_pt)[ij][itk];
    if ( fabs((*track_ipXY)[ij][itk]) < 0.08 ) ptsum += (*track_pt)[ij][itk];//PV weight matching
  }

  double alpha = ptsum/ptsum_total;
  return alpha;
}

double EmJetHistoMaker::GetAlpha2DSig(int ij) // Calculate alpha for given jet
{
  double ptsum_total=0, ptsum=0;
  for (unsigned itk=0; itk < (*track_pt)[ij].size(); itk++) {
    if ( (*track_source)[ij][itk] != 0 ) continue; // Only process tracks with source=0
    if ( ( (*track_quality)[ij][itk] & 4 ) == 0 ) continue; // Only process tracks with "highPurity" quality

    ptsum_total += (*track_pt)[ij][itk];
    if ( fabs((*track_ipXYSig)[ij][itk]) < 4.0 ) ptsum += (*track_pt)[ij][itk];// 2D significance matching
  }

  double alpha = ptsum/ptsum_total;
  return alpha;
}

double EmJetHistoMaker::GetLTKFrac(int ij)
{
  double maxtkpT = -1;
  for (unsigned itk=0; itk < (*track_pt)[ij].size(); itk++) {
    if ( (*track_source)[ij][itk] != 0 ) continue; // Only process tracks with source=0
    if ( ( (*track_quality)[ij][itk] & 4 ) == 0 ) continue; // Only process tracks with "highPurity" quality
    if ( (*track_pt)[ij][itk] > maxtkpT ) maxtkpT=(*track_pt)[ij][itk];
  }
  double ltkfrac = maxtkpT/(*jet_pt)[ij];
  return ltkfrac;
}

double EmJetHistoMaker::GetNonPUFrac(int ij)
{
  double sumpT = 0., ptsum_total = 0.;
  for (unsigned itk=0; itk< (*track_pt)[ij].size(); itk++){
    if ( (*track_source)[ij][itk]!=0 ) continue;
    if ( ( (*track_quality)[ij][itk] & 4 ) == 0 ) continue; // Only process tracks with "highPurity" quality

    ptsum_total += (*track_pt)[ij][itk];
    if ( fabs(pv_z-(*track_ref_z)[ij][itk]) < 1.5 ) sumpT += (*track_pt)[ij][itk]; // distance bigger than 1.5cm is considered pile-up tracks
  }

  double nonpilefrac = sumpT/ptsum_total;
  return nonpilefrac;
}

double EmJetHistoMaker::GetFrac2DSig(int ij)
{
  int nTrack = 0, nTrackpassing = 0;
  for (unsigned itk=0; itk< (*track_pt)[ij].size(); itk++){
    if ( (*track_source)[ij][itk]!=0 ) continue;
    if ( ( (*track_quality)[ij][itk] & 4 ) == 0 ) continue; // Only process tracks with "highPurity" quality

    nTrack++;
    if( fabs((*track_ipXYSig)[ij][itk]) < 2.0 ) nTrackpassing++;
  }

  double frac2DSig = (double)nTrackpassing/nTrack;
  return frac2DSig;
}

int EmJetHistoMaker::GetNTracks(int ij)
{
  int nTracks = 0;
    for (unsigned itk=0; itk< (*track_pt)[ij].size(); itk++){
    if ( (*track_source)[ij][itk]!=0 ) continue;
    if ( ( (*track_quality)[ij][itk] & 4 ) == 0 ) continue; // Only process tracks with "highPurity" quality
    nTracks++; 
  }
  return nTracks;
}

bool EmJetHistoMaker::SelectJet_basic(int ij)
{
  // Count number of tracks
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
  bool cut_eta = abs((*jet_eta)[ij]) < 2.0        ; result = result && cut_eta      ;
  bool cut_tracks = nTrackPassingCut > 0          ; result = result && cut_tracks   ;
  return result;
}

bool EmJetHistoMaker::SelectJet_alphaMax(int ij)
{
  bool result = true;
  bool cut_alphaMax = (*jet_alphaMax)[ij] < 0.04; result = result && cut_alphaMax ;
  return result;
}

bool EmJetHistoMaker::SelectJet_alphaMax2(int ij)
{
  bool result = true;
  bool cut_alphaMax = (*jet_alphaMax2)[ij] < 0.04; result = result && cut_alphaMax ;
  return result;
}

bool EmJetHistoMaker::SelectJet_ipCut(int ij)
{
  bool result = true;
  double maxIp = 0.;
  for (unsigned itk = 0; itk < (*track_pt)[ij].size(); itk++) {
    if( (*track_source)[ij][itk] == 0 ) {
      if ( (*track_ipXY)[ij][itk] > maxIp ) maxIp = (*track_ipXY)[ij][itk];
    }
  }
  bool ipCut = maxIp > 0.4;
  result = ipCut;
  return result;
}

void EmJetHistoMaker::PrintEvent (long eventnumber, string comment)
{
  std::cout << "run,lumi,event,comment: " << run << ", " << lumi << ", " << event << ", " << comment << "\n";
}

bool EmJetHistoMaker::BTaggingPass(int ij)
{
  bool isBJet = false;
  for(unsigned int ib=0; ib<BquarkIndex.size(); ib++){
    float dR = DeltaR((*jet_eta)[ij], (*jet_phi)[ij], (*gp_eta)[BquarkIndex[ib]], (*gp_phi)[BquarkIndex[ib]]);
    bool dRmatching = dR<0.4 ; isBJet = dRmatching;
    //bool dPtmatching = fabs((*jet_pt)[ij]-(*gp_pt)[BquarkIndex[ib]])/(*gp_pt)[BquarkIndex[ib]]<0.3; isBJet = isBJet && dPtmatching;
    if( isBJet ) return isBJet;
  }
  return isBJet;
}

void EmJetHistoMaker::FillBquarkIndex(long eventnumber)
{
  for(unsigned int igp=0; igp<(*gp_pt).size()-1; igp++){
    if( abs((*gp_pdgId)[igp])!=5 || fabs((*gp_pt)[igp])<50.0 || fabs((*gp_eta)[igp])>4.0 ) continue;// b quarks with pT>50.0GeV
    bool notmatched = true;
    for(unsigned int ib=0; ib<BquarkIndex.size(); ib++){
      float dR = DeltaR((*gp_eta)[igp], (*gp_phi)[igp], (*gp_eta)[BquarkIndex[ib]], (*gp_phi)[BquarkIndex[ib]]);
      if( dR<0.4 ) notmatched=false;
    }
    if( notmatched ) BquarkIndex.push_back(igp);
  }
}

int EmJetHistoMaker::FlavourTagging(int ij)
{
  double maxpT = 0.0;
  int flavour = 10;
  for(unsigned int igp=0; igp<(*gp_pt).size()-1; igp++){
    if( abs((*gp_pdgId)[igp])>5 && abs((*gp_pdgId)[igp])!=21 ) continue;
    if( DeltaR((*jet_eta)[ij], (*jet_phi)[ij], (*gp_eta)[igp], (*gp_phi)[igp])>0.4 ) continue;
    if( (*gp_pt)[igp]> maxpT ){
       maxpT = (*gp_pt)[igp];
       flavour = (*gp_pdgId)[igp];
    } 
  } 
  if( abs(flavour)==21 ){
    double maxpT2 = 0.0;
    for(unsigned int igp=0; igp<(*gp_pt).size()-1; igp++){
      if(abs((*gp_pdgId)[igp])!=5 || fabs((*gp_pt)[igp])<10.0 ) continue;
      if( DeltaR((*jet_eta)[ij], (*jet_phi)[ij], (*gp_eta)[igp], (*gp_phi)[igp])>0.4 ) continue;
      if( (*gp_pt)[igp] > maxpT2 ){
        maxpT2 = (*gp_pt)[igp];
        flavour = 19;
      }
    }
  } 
  return abs(flavour);
}

float EmJetHistoMaker::DeltaR(float eta1, float phi1, float eta2, float phi2)
{
  float dR=0.;
  float deta = std::fabs(eta1-eta2);
  float dphi = std::fabs(phi1-phi2);
  if( dphi>3.1415926 ) dphi = 2.*3.1415926-dphi;
  dR=std::sqrt(deta*deta+dphi*dphi);
  return dR;
}

void
EmJetHistoMaker::SetOptions(Sample sample, bool isData, double xsec, long nevent, bool isSignal, bool pileupOnly)
{
  sample_ = sample;
  isData_ = isData;
  xsec_ = xsec;
  nevent_ = nevent;
  isSignal_ = isSignal;
  pileupOnly_ = pileupOnly;
}
