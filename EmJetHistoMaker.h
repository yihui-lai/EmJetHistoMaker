#include "HistoMakerBase.h"
#include "EmJetHistos.h"
#include "EmJetSample.h"
#include "LumiReWeightingStandAlone.h"

#include "Fakerate.h"

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
  void FillEventHistograms  (long eventnumber, string tag);
  void FillSignalHistograms(long eventnumber, const vector<int> &pos_basicJT, string tag);
  void FillClosureTestHistograms (long eventnumber, double vFR[], const vector<int> &pos_basicJT, string tag, int nTag);
  void FillClosureTestHistograms_Truth(long eventnumber, const vector<int> &pos_basicJT, string tag, int nTag);
  void FillJetHistograms_pT (long eventnumber, int ij, string tag);
  void FillJetHistograms_pT (long eventnumber, int ij, string tag, double w);
  void FillJetHistograms    (long eventnumber, int ij, string tag, double w);
  void FillTrackHistograms  (long eventnumber, int ij, int itk, string tag, double w);
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
  float DeltaR(float eta1, float phi1, float eta2, float phi2);
  int FlavourTagging(int ij);
  double GetAlpha2DSig(int ij);
  double GetAlpha3DSigM(int ij);
  double GetLTKFrac(int ij);
  double GetNonPUFrac(int ij);
  double GetFrac2DSig(int ij);
  double GetMedianIP(int ij);
  double GetfabsMedianIP(int ij);
  int GetNTrack(int ij);
  bool GetSignalPartonIndex(long eventnumber);
  bool PartonJetMatching(int ij, int igp);
  bool SelectJet_basic(int jet_index);
  bool SelectJet_Emerging(int jet_index);
  double ModifyCsv(double csv);
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
  int nEvecount;
  vector<double> jet_Alpha2DSig;
  vector<double> jet_Alpha3DSigM;
  vector<double> jet_ltkfrac;
  vector<double> jet_nonpufrac;
  vector<double> jet_frac2DSig;
  vector<double> jet_medianIP;
  vector<double> jet_fabsmedianIP;
  vector<int>    jet_nTrack; 
  vector<int>    jet_flavour;
  string file_; // Path to current input file
  unique_ptr<reweight::LumiReWeighting> LumiWeights_;
  // Calculated variables
  int dkqk_index;
  int dnqk_index;
  int adkqk_index;
  int adnqk_index;
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
  std::cout<< "Number of Processed Events: " << std::setw(12) << eventCount2 <<std::endl;
  histo_eventCountperrun_ = unique_ptr<TH1D>(new TH1D("eventCountperrun", "eventCountperrun", 2, 0., 2.));
  for(int i=0; i< eventCount2; i++){
     histo_eventCountperrun_->Fill(1.);
  }
}

void EmJetHistoMaker::FillHistograms(long eventnumber)
{
  if (debug==1) std::cout << "Entering FillHistograms" << std::endl;

  FillPileupHistograms(eventnumber, "");
  if (pileupOnly_) return;

  // turn on HLT trigger and jetfilter for QCD and signal sampels
  if( HLT_PFHT800==0 ) return;
  if( (*pv_index)[0]!=-1 || fabs((*pv_z)[0])>15.0 ) return;
  if( !JetFilter(eventnumber) ) return;
  FillEventHistograms(eventnumber, "");
  ++nEvecount;
}

void EmJetHistoMaker::FillEventHistograms(long eventnumber, string tag)
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

  // Prepare jet-level variables
  if(!jet_Alpha2DSig.empty()) jet_Alpha2DSig.clear();
  if(!jet_Alpha3DSigM.empty()) jet_Alpha3DSigM.clear();
  if(!jet_ltkfrac.empty()) jet_ltkfrac.clear();
  if(!jet_nonpufrac.empty()) jet_nonpufrac.clear();
  if(!jet_frac2DSig.empty()) jet_frac2DSig.clear();
  if(!jet_medianIP.empty()) jet_medianIP.clear();
  if(!jet_fabsmedianIP.empty()) jet_fabsmedianIP.clear();
  if(!jet_nTrack.empty()) jet_nTrack.clear();
  if(!jet_flavour.empty()) jet_flavour.clear();
 
  jet_Alpha2DSig.reserve((*jet_pt).size());
  jet_Alpha3DSigM.reserve((*jet_pt).size()); 
  jet_ltkfrac.reserve((*jet_pt).size());
  jet_nonpufrac.reserve((*jet_pt).size());
  jet_frac2DSig.reserve((*jet_pt).size());
  jet_medianIP.reserve((*jet_pt).size());
  jet_fabsmedianIP.reserve((*jet_pt).size()); 
  jet_nTrack.reserve((*jet_pt).size());
  jet_flavour.reserve((*jet_pt).size());

  for (unsigned ij = 0; ij< (*jet_pt).size(); ij++){
    double Alpha2DSig = GetAlpha2DSig(ij);//with track quality cut, deltaR matching and ipXYSig cut
    double Alpha3DSigM = GetAlpha3DSigM(ij);
    jet_Alpha2DSig.push_back(Alpha2DSig);
    jet_Alpha3DSigM.push_back(Alpha3DSigM);

    double ltkfrac = GetLTKFrac(ij);
    double nonpufrac = GetNonPUFrac(ij);
    double frac2DSig = GetFrac2DSig(ij);
    double medianip = GetMedianIP(ij);
    double fabsmedianip = GetfabsMedianIP(ij);
    int nTrack = GetNTrack(ij);
    jet_ltkfrac.push_back(ltkfrac);
    jet_nonpufrac.push_back(nonpufrac);
    jet_frac2DSig.push_back(frac2DSig);
    jet_medianIP.push_back(medianip);
    jet_fabsmedianIP.push_back(fabsmedianip);
    jet_nTrack.push_back(nTrack);
  
    // tag the jet flavour using MC truth info, for MC only   
    if ( !isData_ ){
      int flavour = FlavourTagging(ij);
      jet_flavour.push_back(flavour);
    }

  }

  if( jet_Alpha2DSig.size()!=(*jet_pt).size() || jet_ltkfrac.size()!=(*jet_pt).size() || jet_nonpufrac.size()!=(*jet_pt).size() || jet_frac2DSig.size()!=(*jet_pt).size() || jet_medianIP.size()!=(*jet_pt).size() || jet_fabsmedianIP.size()!= (*jet_pt).size() ){
    std::cout << "Error!!!Jet-Level variable Store Problem!!!!" << std::endl;
  }

  unsigned int nJet_basic = 0, nJet_tag = 0;
  int nJet_BTagL = 0, nJet_BTagM = 0, nJet_BTagT = 0;
  double minAlpha3DSigM = 10.0;
  int minAlpha3DSigM_index = -1;
  double maxCsv = -10.0;
  int maxCsv_index = -1;
  vector<int> pos_basicJT;

  // for closure test
  //double fr1[4] = {-1.0, -1.0, -1.0, -1.0};// first kind of test: without using flavour info
  //double fr2[4] = {-1.0, -1.0, -1.0, -1.0};// second kind of test: using flavour info

  // Jet loop
  for (unsigned ij = 0; ij < (*jet_pt).size(); ij++) {
    //if (ij>=4) break; // :JETCUT:
    if( nJet_basic>=4 ) break;
    if( !SelectJet_basic(ij) ) continue;
    nJet_basic++;
    pos_basicJT.push_back(ij);
    FillJetHistograms_pT(eventnumber, ij, ""+tag);
  
    // use the b-tagging information
    if( (*jet_csv)[ij]>0.5426 ) nJet_BTagL++; // loose wp
    if( (*jet_csv)[ij]>0.8484 ) nJet_BTagM++; // medium wp
    if( (*jet_csv)[ij]>0.9535 ) nJet_BTagT++; // tight wp
   
    if( SelectJet_Emerging(ij) ) {
      FillJetHistograms_pT(eventnumber, ij, "__JTAFM"+tag);
      nJet_tag++;
    }
     
    // Closure Test( for QCD )
    //int ibasicjet = nJet_basic-1;
    //int nTrack = jet_nTrack[ij];
    //fr1[ibasicjet] = frCal(nTrack, 0);
    //if( !isData_ ){
    //  if (jet_flavour[ij]<5 || jet_flavour[ij]==21 || jet_flavour[ij]==10 ) fr2[ibasicjet] = frCal(nTrack, 1);
    //  else if(jet_flavour[ij]==5 || jet_flavour[ij]==19 ) fr2[ibasicjet]=frCal(nTrack, 2);
    //}
    //if( (*jet_csv)[ij]>0.8484 ) fr2[ibasicjet] = frCal(nTrack, 1);
    //else fr2[ibasicjet] = frCal(nTrack, 2);
    
    if( jet_Alpha3DSigM[ij]< minAlpha3DSigM ){
      minAlpha3DSigM = jet_Alpha3DSigM[ij];
      minAlpha3DSigM_index = ij;
    }

    if( (*jet_csv)[ij] > maxCsv ){
      maxCsv = (*jet_csv)[ij];
      maxCsv_index = ij;
    }
  }// Finish the loop over all jets in one event
  
  // Existing quantities (in ntuple)
  histo_->hist1d["nVtx"+tag]->Fill(nVtx, w);

  // Calculated quantities
  histo_->hist1d["ht"+tag]->Fill(ht, w);
  histo_->hist1d["ht4"+tag]->Fill(ht4, w);
  histo_->hist1d["pv_z"+tag]->Fill((*pv_z)[0], w);
  histo_->hist1d["nJets"+tag]->Fill((*jet_pt).size(), w);
  histo_->hist1d["nJets_JTbasic"+tag]->Fill(pos_basicJT.size(), w);
  histo_->hist1d["nJets_BTagL"+tag]->Fill(nJet_BTagL, w);
  histo_->hist1d["nJets_BTagM"+tag]->Fill(nJet_BTagM, w);
  histo_->hist1d["nJets_BTagT"+tag]->Fill(nJet_BTagT, w);  

  FillJetHistograms_pT(eventnumber, minAlpha3DSigM_index, "__JTminAlpha3DSigM"+tag);
  FillJetHistograms_pT(eventnumber, maxCsv_index,         "__JTmaxCsv"+tag);
 
  // debug 
  if( pos_basicJT.size()  != nJet_basic )  std::cout << "Error!!! jet multiplicity does not match" << std::endl;

  // Closure Test ( for QCD )
  /*
  for(int ij=0; ij<4; ij++){
    if( fr1[ij]<0. ){
      std::cout << "Error: Event "<< event << " "<< run << " "<< lumi << " with fr1 "<< fr1[ij] <<  " of index "<< ij << " does not make sense"<< std::endl;
    }
    if( fr2[ij]<0. ){
      std::cout << "Error: Event "<< event << " "<< run << " "<< lumi << " with fr2 "<< fr2[ij] <<  " of index "<< ij << " does not make sense"<< std::endl;
    }
  }
  */
  //FillClosureTestHistograms(eventnumber, fr1, pos_basicJT, "__withoutflavour", 0);
  //FillClosureTestHistograms(eventnumber, fr1, pos_basicJT, "__withoutflavour", 1);
  //FillClosureTestHistograms(eventnumber, fr1, pos_basicJT, "__withoutflavour", 2);  

  //FillClosureTestHistograms(eventnumber, fr2, pos_basicJT, "__withflavour", 0);  
  //FillClosureTestHistograms(eventnumber, fr2, pos_basicJT, "__withflavour", 1);
  //FillClosureTestHistograms(eventnumber, fr2, pos_basicJT, "__withflavour", 2);  
  if( nJet_tag<=2 ){
    FillClosureTestHistograms_Truth(eventnumber, pos_basicJT, "", nJet_tag);
  }

  // tag and probe
  /*
  for( auto &jindex: pos_basicJT){
    if( (*jet_csv)[jindex]<0.8 || (*jet_pt)[jindex]<50.0 ) continue;//tag
    for( auto &jindex2: pos_basicJT ){ // probe
      if( jindex2==jindex ) continue;
      FillJetHistograms_pT(eventnumber, jindex2, "__TypeV"+tag);
      if( jet_Alpha3DSigM[jindex2]<0.3 && jet_fabsmedianIP[jindex2]>0.04 ){
         FillJetHistograms_pT(eventnumber, jindex2, "__JTAlpha__TypeV"+tag);
      }
    }
  }
  
  for( auto &jindex: pos_basicJT){
    if( (*jet_csv)[jindex]>0.2 || (*jet_pt)[jindex]<50.0 ) continue;//tag
    for( auto &jindex2: pos_basicJT ){ // probe
      if( jindex2==jindex ) continue;
      FillJetHistograms_pT(eventnumber, jindex2, "__TypeVI"+tag);
      if( jet_Alpha3DSigM[jindex2]<0.3 && jet_fabsmedianIP[jindex2]>0.04 ){
         FillJetHistograms_pT(eventnumber, jindex2, "__JTAlpha__TypeVI"+tag);
      }
    }
  } 
  */
}

void EmJetHistoMaker::FillSignalHistograms(long eventnumber, const vector<int> &pos_basicJT, string tag)
{
  double w = CalculateEventWeight(eventnumber);
  dkqk_index  = -1;
  dnqk_index  = -1;
  adkqk_index = -1;
  adnqk_index = -1;
  bool foundPartons = GetSignalPartonIndex(eventnumber);
  int nJet_dksignal = 0, nJet_dnsignal = 0;
  for(int ibasicjet=0; ibasicjet < static_cast<int>(pos_basicJT.size()); ibasicjet++){// loop over basic jets
    int ij = pos_basicJT[ibasicjet];

    bool isdksig = false; // is signal dark quark jet or not
    bool isdnsig = false; // is signal down quark jet or not
    if ( foundPartons && ( PartonJetMatching(ij, dkqk_index) || PartonJetMatching(ij, adkqk_index)) ) {
      FillJetHistograms_pT(eventnumber, ij, "__dksig"+tag);
      isdksig = true;
      nJet_dksignal++;
    }
    if ( foundPartons && ( PartonJetMatching(ij, dnqk_index) || PartonJetMatching(ij, adnqk_index) ) ){
      FillJetHistograms_pT(eventnumber, ij, "__dnsig"+tag);
      isdnsig = true;
      nJet_dnsignal++;
    }

    if( SelectJet_Emerging(ij) ) {
      if( isdksig ) FillJetHistograms_pT(eventnumber, ij, "__dksig__JTAFM"+tag);
      if( isdnsig ) FillJetHistograms_pT(eventnumber, ij ,"__dnsig__JTAFM"+tag);
    }
  }
  histo_->hist1d["nJets_dksignal"+tag]->Fill(nJet_dksignal, w);
  histo_->hist1d["nJets_dnsignal"+tag]->Fill(nJet_dnsignal, w);
  histo_->hist1d["nJets_signal"+tag]->Fill(nJet_dksignal+nJet_dnsignal, w);
}

void EmJetHistoMaker::FillClosureTestHistograms(long eventnumber, double vFR[], const vector<int> &pos_basicJT, string tag, int nTag)
{
  double p_ntag = PnTag(vFR, nTag);
  histo_->hist1d["nEvts_ntag"+tag]->Fill(nTag,  p_ntag);// fill in nTag bin with weight p_ntag;
  //if( pos_basicJT.size()!=4 ) {
  //  std::cout << "Error: basic jet size of this event" << event << " " << run << " " << lumi << " is "<< pos_basicJT.size() << ", not 4" << std::endl;
  //}
  /*
  for( int ibasicJet=0; ibasicJet< pos_basicJT.size(); ibasicJet++){
    int ij = pos_basicJT[ibasicJet];
    FillJetHistograms_pT(eventnumber, ij, tag+std::to_string(nTag), p_ntag);
    double pem_ntag = PEmergingnTag(vFR, nTag, ibasicJet); // probability of jet ij tagged as emerging in nTag event
    FillJetHistograms_pT(eventnumber, ij, tag+std::to_string(nTag)+"__Emerging", pem_ntag);
  }
  */
}

void EmJetHistoMaker::FillClosureTestHistograms_Truth(long eventnumber, const vector<int> &pos_basicJT, string tag, int nTag)
{ 
  histo_->hist1d["nJets_tag"+tag]->Fill(nTag, 1.0);	
  if( pos_basicJT.size()!=4 ) {
   std::cout << "Error: basic jet size of this event" << event << " " << run << " " << lumi << " is "<< pos_basicJT.size() << ", not 4" << std::endl;
  }
  int ntag=0;
  for( int ibasicJet=0; ibasicJet< static_cast<int>(pos_basicJT.size()); ibasicJet++){
    int ij = pos_basicJT[ibasicJet];
    FillJetHistograms_pT(eventnumber, ij, tag+"__truth_with"+std::to_string(nTag)+"Tag");
    if( SelectJet_Emerging(ij) ){
      FillJetHistograms_pT(eventnumber, ij, tag+"__Emerging_truth_with"+std::to_string(nTag)+"Tag");
      ntag++;
    }
  }
  if( nTag!=ntag ){
    std::cout << "Error(DEBUG): truth nTag passed does not match truth ntag" << std::endl;
  }
}

void EmJetHistoMaker::FillJetHistograms_pT(long eventnumber, int ij, string tag)
{  
  double w = CalculateEventWeight(eventnumber);
  FillJetHistograms_pT(eventnumber, ij, tag, w);
}

void EmJetHistoMaker::FillJetHistograms_pT(long eventnumber, int ij, string tag, double w)
{
  if((*jet_pt)[ij]>=100.0){
    FillJetHistograms(eventnumber, ij, tag, w);
    if( (*jet_pt)[ij]>=100.0 && (*jet_pt)[ij]<200.0 )      FillJetHistograms(eventnumber, ij, "__pt1"+tag, w);
    else if( (*jet_pt)[ij]>=200.0 && (*jet_pt)[ij]<300.0 ) FillJetHistograms(eventnumber, ij, "__pt2"+tag, w);
    else if( (*jet_pt)[ij]>=300.0 )                        FillJetHistograms(eventnumber, ij, "__pt3"+tag, w);

    // add flavour bin for MC 
    if( !isData_ ){
      string ftag;
      if( jet_flavour[ij]<5 ) ftag = "__1" + tag;
      else                    ftag = "__"+std::to_string(jet_flavour[ij]) + tag;
      FillJetHistograms(eventnumber, ij, ftag, w);
      if( (*jet_pt)[ij]>=100.0 && (*jet_pt)[ij]<200.0 )      FillJetHistograms(eventnumber, ij, "__pt1"+ftag, w);
      else if( (*jet_pt)[ij]>=200.0 && (*jet_pt)[ij]<300.0 ) FillJetHistograms(eventnumber, ij, "__pt2"+ftag, w);
      else if( (*jet_pt)[ij]>=300.0 )                        FillJetHistograms(eventnumber, ij, "__pt3"+ftag, w);
    }
  }
}

void EmJetHistoMaker::FillJetHistograms(long eventnumber, int ij, string tag, double w)
{
  if (debug==1) std::cout << "Entering FillJetHistograms" << std::endl;

  // Calculate median 2D impact parameter (source=0, high quality)
  int nTrack = 0;
  double medianIP = 0.;
  double maxIP = 0.;
  double medianIPSig = 0.;
  double maxIPSig = 0.;
  {
    vector<double> vector_ipXY;
    vector<double> vector_ipXYSig;
    for (unsigned itk=0; itk < (*track_pt)[ij].size(); itk++) {
      if ( (*track_source)[ij][itk] == 0 &&  ((*track_quality)[ij][itk] & 4)>0 ) {
        vector_ipXY.push_back( (*track_ipXY)[ij][itk] );
        vector_ipXYSig.push_back( (*track_ipXYSig)[ij][itk]);
        FillTrackHistograms(eventnumber, ij, itk, tag, w);
      }
    }
    std::sort(vector_ipXY.begin(), vector_ipXY.end());
    std::sort(vector_ipXYSig.begin(), vector_ipXYSig.end());
    nTrack = vector_ipXY.size();
    medianIP = 0.;
    if (nTrack>0) {
      if ( nTrack % 2 == 0 ) {
        medianIP = (vector_ipXY[nTrack/2 - 1] + vector_ipXY[nTrack/2]) / 2;
        medianIPSig = (vector_ipXYSig[nTrack/2 - 1] + vector_ipXYSig[nTrack/2]) / 2;
      }
      else {
        medianIP = (vector_ipXY[nTrack/2]);
        medianIPSig = (vector_ipXYSig[nTrack/2]);
      }
      maxIP = vector_ipXY[nTrack-1];
      maxIPSig = vector_ipXYSig[nTrack-1];
    }
  }

  // Calculate prompt/displaced energy fraction (source=0)
  /*
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
  */
  if( medianIP != jet_medianIP[ij] ) {
    std::cout << "Error! Original medianIP and newly implemented medianIP inconsistent!!" << std::endl;
  }

  // Existing quantities (in ntuple)
  histo_->hist1d["jet_pt"+tag]->Fill((*jet_pt)[ij], w);
  histo_->hist1d["jet_eta"+tag]->Fill((*jet_eta)[ij], w);
  histo_->hist1d["jet_phi"+tag]->Fill((*jet_phi)[ij], w);
  histo_->hist1d["jet_alphaMax"+tag]->Fill((*jet_alphaMax)[ij], w);
  histo_->hist1d["jet_cef"+tag]->Fill((*jet_cef)[ij], w);
  histo_->hist1d["jet_nef"+tag]->Fill((*jet_nef)[ij], w);
  histo_->hist1d["jet_csv"+tag]->Fill(ModifyCsv((*jet_csv)[ij]), w);
  histo_->hist1d["jet_nDarkPions"+tag]->Fill((*jet_nDarkPions)[ij], w);  
  histo_->hist1d["jet_logtheta2D"+tag]->Fill(TMath::Log10((*jet_theta2D)[ij]), w);

  // Calculated quantities
  histo_->hist1d["jet_Alpha3DSigM"+tag]->Fill(jet_Alpha3DSigM[ij], w);
  histo_->hist1d["jet_ltkfrac"+tag]->Fill(jet_ltkfrac[ij], w);
  histo_->hist1d["jet_nonpufrac"+tag]->Fill(jet_nonpufrac[ij], w);
  histo_->hist1d["jet_frac2DSig"+tag]->Fill(jet_frac2DSig[ij], w);
  histo_->hist1d["jet_medianIP"+tag]->Fill(jet_medianIP[ij], w);
  histo_->hist1d["jet_fabsmedianIP"+tag]->Fill(jet_fabsmedianIP[ij], w);
  histo_->hist1d["jet_logfabsmedianIP"+tag]->Fill(TMath::Log10((jet_fabsmedianIP)[ij]), w);

  histo_->hist1d["jet_nTrack"+tag]->Fill(nTrack, w);
  histo_->hist1d["jet_maxIP"+tag]->Fill(maxIP, w);
  histo_->hist1d["jet_maxIPSig"+tag]->Fill(maxIPSig, w);
  histo_->hist1d["jet_medianIPSig"+tag]->Fill(medianIPSig, w);
  //histo_->hist1d["jet_prompt_frac"+tag]->Fill(prompt_frac, w);
  //histo_->hist1d["jet_disp_frac"+tag]->Fill(disp_frac, w);

  /* 
  if( nTrack>=0 && nTrack<24 ) {
    histo_->hist1d["jet_Alpha3DSigM__nTrack"+std::to_string(nTrack)+tag]->Fill(jet_Alpha3DSigM[ij], w);
    histo_->hist1d["jet_csv__nTrack"+std::to_string(nTrack)+tag]->Fill(ModifyCsv((*jet_csv)[ij]), w);
  }   
  if( nTrack>=24 ) {
     histo_->hist1d["jet_Alpha3DSigM__nTrack24"+tag]->Fill(jet_Alpha3DSigM[ij], w);
     histo_->hist1d["jet_csv__nTrack24"+tag]->Fill(ModifyCsv((*jet_csv)[ij]), w);
  }
  */

  // 2D histos
  histo_->hist2d[string()+"jet_medianIP"+"_VS_"+"jet_Alpha3DSigM"+tag]->Fill(jet_Alpha3DSigM[ij], jet_medianIP[ij], w);
  histo_->hist2d[string()+"jet_fabsmedianIP"+"_VS_"+"jet_Alpha3DSigM"+tag]->Fill(jet_Alpha3DSigM[ij], jet_fabsmedianIP[ij], w);  
  /*
  histo_->hist2d[string()+"jet_disp_frac"+"_VS_"+"jet_alphaMax"+tag]->Fill((*jet_alphaMax)[ij], disp_frac, w);
  histo_->hist2d[string()+"jet_disp_frac"+"_VS_"+"jet_pt"+tag]->Fill((*jet_pt)[ij], disp_frac, w);
  histo_->hist2d[string()+"jet_alphaMax"+"_VS_"+"jet_pt"+tag]->Fill((*jet_pt)[ij], (*jet_alphaMax)[ij], w);
  histo_->hist2d[string()+"jet_alphaMax2"+"_VS_"+"jet_pt"+tag]->Fill((*jet_pt)[ij], (*jet_alphaMax2)[ij], w);
  histo_->hist2d[string()+"jet_alphaMax"+"_VS_"+"ht"+tag]->Fill(event_ht_, (*jet_alphaMax)[ij], w);
  histo_->hist2d[string()+"jet_alphaMax"+"_VS_"+"jet_alphaMax2"+tag]->Fill((*jet_alphaMax2)[ij], (*jet_alphaMax)[ij], w);
  histo_->hist2d[string()+"jet_alphaMax"+"_VS_"+"nVtx"+tag]->Fill(nVtx, (*jet_alphaMax)[ij], w);
  histo_->hist2d[string()+"pv_indexInColl"+"_VS_"+"jet_alphaMax"+tag]->Fill((*jet_alphaMax)[ij], pv_indexInColl, w);
  histo_->hist2d[string()+"pv_indexInColl"+"_VS_"+"jet_alphaMax2"+tag]->Fill((*jet_alphaMax2)[ij], pv_indexInColl, w);
  histo_->hist2d[string()+"jet_maxIP"+"_VS_"+"jet_alphaMax"+tag]->Fill((*jet_alphaMax)[ij], maxIP, w);
  */
}

void EmJetHistoMaker::FillTrackHistograms(long eventnumber, int ij, int itk, string tag, double w)
{
  if (debug==1) std::cout << "Entering FillTrackHistograms" << std::endl;
  if (debug==2) OUTPUT(tag);

  // Existing quantities (in ntuple)
  histo_->hist1d["track_pt"+tag]->Fill((*track_pt)[ij][itk], w);
  histo_->hist1d["track_eta"+tag]->Fill((*track_eta)[ij][itk], w);
  histo_->hist1d["track_phi"+tag]->Fill((*track_phi)[ij][itk], w);
  histo_->hist1d["track_quality"+tag]->Fill((*track_quality)[ij][itk], w);
  histo_->hist1d["track_ipXY"+tag]->Fill((*track_ipXY)[ij][itk], w);
  histo_->hist1d["track_ipXYSig"+tag]->Fill((*track_ipXYSig)[ij][itk], w);

  histo_->hist1d["track_ref_z"+tag]->Fill((*track_ref_z)[ij][itk], w);
  double dist = (*pv_z)[0]-(*track_ref_z)[ij][itk];
  histo_->hist1d["track_dis_z"+tag]->Fill(dist, w);
  double tk3dsig2 = TMath::Power(((*pv_z)[0]-(*track_ref_z)[ij][itk])/0.01, 2.0) + TMath::Power((*track_ipXYSig)[ij][itk], 2.0);
  histo_->hist1d["track_ip3DSigM"+tag]->Fill(TMath::Sqrt(tk3dsig2), w);
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

/*
bool EmJetHistoMaker::JetFilter(long eventnumber)
{
  double w = CalculateEventWeight(eventnumber);
  double ht4 = (*jet_pt)[0] + (*jet_pt)[1] + (*jet_pt)[2] + (*jet_pt)[3];
  const int nCut = 7;
  bool cuts[nCut]; string labels[nCut];
  cuts[0] = true                                                  ; labels[0]=("nocut")                                    ;
  cuts[1] = cuts[1-1] && jet_pt->size()>=4                        ; labels[1]=("jet_pt->size()>=4 ")                       ;
  cuts[2] = cuts[2-1] && ht4 > 1000                               ; labels[2]=("ht4 > 1000        ")                       ;
  cuts[3] = cuts[3-1] && SelectJet_basic(0) && (*jet_pt)[0] > 400 ; labels[3]=("SelectJet_basic(0) && (*jet_pt)[0] > 400") ;
  cuts[4] = cuts[4-1] && SelectJet_basic(1) && (*jet_pt)[1] > 200 ; labels[4]=("SelectJet_basic(1) && (*jet_pt)[1] > 200") ;
  cuts[5] = cuts[5-1] && SelectJet_basic(2) && (*jet_pt)[2] > 200 ; labels[5]=("SelectJet_basic(2) && (*jet_pt)[2] > 200") ;
  cuts[6] = cuts[6-1] && SelectJet_basic(3) && (*jet_pt)[3] > 100 ; labels[6]=("SelectJet_basic(3) && (*jet_pt)[3] > 100") ;
  //Fill the cutflow table
  for (int ic = 0; ic < nCut; ic ++) {
    if ( cuts[ic] ) histo_->hist1d["cutflow"]->Fill(labels[ic].c_str(), w);
  }
  return cuts[6]; 
}
*/

bool EmJetHistoMaker::JetFilter(long eventnumber)
{
  double w = CalculateEventWeight(eventnumber);

  //find the index of basic jets
  vector<int> vBasicJet;
  for(int ij=0; ij<static_cast<int>((*jet_pt).size()); ij++){
    if( SelectJet_basic(ij) ) vBasicJet.push_back(ij);
    if( vBasicJet.size()>=4 ) break;
  }

  // ht of the four leading basic jets
  double ht4 = 0, pt0 = 0, pt1 = 0, pt2 = 0, pt3 = 0;
  if( vBasicJet.size()>=4 ) {
    pt0 = (*jet_pt)[vBasicJet[0]]; 
    pt1 = (*jet_pt)[vBasicJet[1]];
    pt2 = (*jet_pt)[vBasicJet[2]];
    pt3 = (*jet_pt)[vBasicJet[3]];
    ht4 = pt0 + pt1 + pt2 + pt3;
  }

  const int nCut = 7;
  bool cuts[nCut]; string labels[nCut];
  cuts[0] = true                                 ; labels[0]=("nocut")                            ;
  cuts[1] = cuts[1-1] && vBasicJet.size()>=4     ; labels[1]=("basicjet->size()>=4 ")             ;
  cuts[2] = cuts[2-1] && ht4 > 1000              ; labels[2]=("ht4 > 1000        ")               ;
  cuts[3] = cuts[3-1] && pt0 > 300               ; labels[3]=("(*jet_pt)[basicjet0] > 300")       ;
  cuts[4] = cuts[4-1] && pt1 > 300               ; labels[4]=("(*jet_pt)[basicjet1] > 300")       ;
  cuts[5] = cuts[5-1] && pt2 > 200               ; labels[5]=("(*jet_pt)[basicjet2] > 200")       ;
  cuts[6] = cuts[6-1] && pt3 > 150               ; labels[6]=("(*jet_pt)[basicjet3] > 150")       ;

  //Fill the cutflow table
  for (int ic = 0; ic < nCut; ic ++) {
    if ( cuts[ic] ) histo_->hist1d["cutflow"]->Fill(labels[ic].c_str(), w);
  }
  return cuts[6];
}

double EmJetHistoMaker::GetAlpha2DSig(int ij) // Calculate alpha for given jet
{
  double ptsum_total=0, ptsum=0;
  for (unsigned itk=0; itk < (*track_pt)[ij].size(); itk++) {
    if ( (*track_source)[ij][itk] != 0 ) continue; // Only process tracks with source=0
    if ( ( (*track_quality)[ij][itk] & 4 ) == 0 ) continue; // Only process tracks with "highPurity" quality
    if ( fabs((*pv_z)[0]-(*track_ref_z)[ij][itk]) > 1.5 ) continue;//remove pileup tracks

    ptsum_total += (*track_pt)[ij][itk];
    if ( fabs((*track_ipXYSig)[ij][itk]) < 4.0 ) ptsum += (*track_pt)[ij][itk];// 2D significance matching
  }

  double alpha = (ptsum_total > 0 ? ptsum/ptsum_total : 0.);
  return alpha;
}

double EmJetHistoMaker::GetAlpha3DSigM(int ij)
{
  double ptsum_total=0, ptsum=0;
  for (unsigned itk=0; itk< (*track_pt)[ij].size(); itk++){
    if ( (*track_source)[ij][itk]!=0 ) continue;
    if ( ( (*track_quality)[ij][itk] & 4 ) == 0 ) continue; // Only process tracks with "highPurity" quality

    ptsum_total += (*track_pt)[ij][itk];
    double tk3dsig2 = TMath::Power(((*pv_z)[0]-(*track_ref_z)[ij][itk])/0.01, 2.0) + TMath::Power((*track_ipXYSig)[ij][itk], 2.0);
    double tk3dsig  = TMath::Sqrt(tk3dsig2);
    if( tk3dsig< 8.0 ) ptsum += (*track_pt)[ij][itk];
  }

  double alpha3dsigm = (ptsum_total>0 ? ptsum/ptsum_total: -1.);
  return alpha3dsigm;
}

double EmJetHistoMaker::GetMedianIP(int ij)
{
  double medip = -10.0;
  vector<double> vector_ipXY;
  for (unsigned itk=0; itk<(*track_pt)[ij].size(); itk++) {
    if( (*track_source)[ij][itk]!=0 ) continue;
    if ( ( (*track_quality)[ij][itk] & 4 ) == 0 ) continue; // Only process tracks with "highPurity" quality
    vector_ipXY.push_back( (*track_ipXY)[ij][itk] );
  }
  
  std::sort(vector_ipXY.begin(), vector_ipXY.end());
  int nTrack = vector_ipXY.size();
  if ( nTrack>0 ) {
    if ( nTrack%2 ==0 )	
      medip = (vector_ipXY[nTrack/2 - 1] + vector_ipXY[nTrack/2]) / 2;
    else
      medip = (vector_ipXY[nTrack/2]);
  }
  return medip;
}

double EmJetHistoMaker::GetfabsMedianIP(int ij)
{
  double medip = -10.0;
  vector<double> vector_ipXY;
  for (unsigned itk=0; itk<(*track_pt)[ij].size(); itk++) {
    if( (*track_source)[ij][itk]!=0 ) continue;
    if ( ( (*track_quality)[ij][itk] & 4 ) == 0 ) continue; // Only process tracks with "highPurity" quality
    vector_ipXY.push_back( fabs((*track_ipXY)[ij][itk]) );
  }

  std::sort(vector_ipXY.begin(), vector_ipXY.end());
  int nTrack = vector_ipXY.size();
  if ( nTrack>0 ) {
    if ( nTrack%2 ==0 )
      medip = (vector_ipXY[nTrack/2 - 1] + vector_ipXY[nTrack/2]) / 2;
    else
      medip = (vector_ipXY[nTrack/2]);
  }
  return medip;
}

int EmJetHistoMaker::GetNTrack(int ij)
{
  int nTrack = 0;
  for( unsigned itk=0; itk<(*track_pt)[ij].size(); itk++){
    if( (*track_source)[ij][itk]!=0 ) continue;
    if( ( (*track_quality)[ij][itk] & 4 ) == 0 ) continue; // Only process tracks with "highPurity" quality
    nTrack++;
  }
  return nTrack;
}

double EmJetHistoMaker::GetLTKFrac(int ij)
{
  double maxtkpT = -1;
  for (unsigned itk=0; itk < (*track_pt)[ij].size(); itk++) {
    if ( (*track_source)[ij][itk] != 0 ) continue; // Only process tracks with source=0
    if ( ( (*track_quality)[ij][itk] & 4 ) == 0 ) continue; // Only process tracks with "highPurity" quality
    if ( fabs((*pv_z)[0]-(*track_ref_z)[ij][itk]) > 1.5 ) continue;//remove pile-up tracks
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
    if ( fabs((*pv_z)[0]-(*track_ref_z)[ij][itk]) < 1.5 ) sumpT += (*track_pt)[ij][itk]; // distance bigger than 1.5cm is considered pile-up tracks
  }

  double nonpilefrac = (ptsum_total > 0 ? sumpT/ptsum_total : 0.);
  return nonpilefrac;
}

double EmJetHistoMaker::GetFrac2DSig(int ij)
{
  int nTrack = 0, nTrackpassing = 0;
  for (unsigned itk=0; itk< (*track_pt)[ij].size(); itk++){
    if ( (*track_source)[ij][itk]!=0 ) continue;
    if ( ( (*track_quality)[ij][itk] & 4 ) == 0 ) continue; // Only process tracks with "highPurity" quality
    if ( fabs((*pv_z)[0]-(*track_ref_z)[ij][itk]) > 1.5 ) continue;

    nTrack++;
    if( fabs((*track_ipXYSig)[ij][itk]) < 2.0 ) nTrackpassing++;
  }

  double frac2DSig = (nTrack > 0 ? (double)nTrackpassing/nTrack : 0.);
  return frac2DSig;
}

bool EmJetHistoMaker::SelectJet_basic(int ij)
{
  // Count number of tracks
  int nTrackPassingCut = 0;
  for (unsigned itk = 0; itk < (*track_pt)[ij].size(); itk++) {
    if( (*track_source)[ij][itk] == 0 && ((*track_quality)[ij][itk] & 4)>0 ) {
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
  
  double ltkfrac   = GetLTKFrac(ij);
  double nonpufrac = GetNonPUFrac(ij);

  bool cut_ltkfrac = ltkfrac >0.0 && ltkfrac<0.6  ; result = result && cut_ltkfrac  ;
  bool cut_nonpufrac = nonpufrac > 0.4            ; result = result && cut_nonpufrac;
  return result;
}

bool EmJetHistoMaker::SelectJet_Emerging(int ij)
{
  bool isEmerging = false;	
  if( ij>= static_cast<int>(jet_Alpha3DSigM.size()) || ij >= static_cast<int>(jet_fabsmedianIP.size()) ) {
    std::cout << "Error!!! vector out of range,  size " << jet_Alpha3DSigM.size() << " " << jet_fabsmedianIP.size() << " ij "<< ij << std::endl;
    return false;
  }
  if( jet_Alpha3DSigM[ij]<0.20 && jet_fabsmedianIP[ij]>0.05 ){
    isEmerging = true;
  }
  return isEmerging;
}

// change the csv from -10.0 to -1.0, if csv == -10.0, make it easier 
// to be shown in the csv histograms
double EmJetHistoMaker::ModifyCsv(double csv)
{
  return (fabs(csv - (-10.0) ) < 0.0001 ) ? -1.0 : csv; 
}

void EmJetHistoMaker::PrintEvent (long eventnumber, string comment)
{
  std::cout << "run,lumi,event,comment: " << run << ", " << lumi << ", " << event << ", " << comment << "\n";
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

bool EmJetHistoMaker::GetSignalPartonIndex(long eventnumber)
{
  bool foundallpartons = false;
  for (unsigned igp=0; igp< (*gp_pt).size(); igp++){
    if( (*gp_pdgId)[igp]==4900101 && dkqk_index==-1 && (*gp_pdgId)[igp+1]==1 ){
      dkqk_index = igp;
      dnqk_index = igp+1;
    }
    if( ((*gp_pdgId)[igp]==4900101) && dkqk_index==-1 && (*gp_pdgId)[igp-1]==1 ){
      dkqk_index = igp;
      dnqk_index = igp-1;
    }

    if( (*gp_pdgId)[igp]==-4900101 && adkqk_index==-1 && (*gp_pdgId)[igp+1]==-1 ){
      adkqk_index = igp;
      adnqk_index = igp+1;
    }
    else if( (*gp_pdgId)[igp]==-4900101 && adkqk_index==-1 && (*gp_pdgId)[igp-1]==-1 ){
      adkqk_index = igp;
      adnqk_index = igp-1;
    }
  }

  if( dkqk_index!=-1 && dnqk_index!=-1 && adkqk_index!=-1 && adnqk_index!=-1 ) foundallpartons=true;
  return foundallpartons;
}

bool EmJetHistoMaker::PartonJetMatching(int ij, int igp)
{
  bool matched = false;
  if( DeltaR((*jet_eta)[ij], (*jet_phi)[ij], (*gp_eta)[igp], (*gp_phi)[igp])< 0.4 )  matched=true;
  return matched;
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

  //
  nEvecount = 0;
}
