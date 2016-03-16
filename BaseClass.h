//////////////////////////////////////////////////////////
// This class has been automatically generated on
<<<<<<< Updated upstream
// Wed Mar  2 04:58:51 2016 by ROOT version 6.02/05
=======
// Tue Mar 15 19:47:29 2016 by ROOT version 6.02/05
>>>>>>> Stashed changes
// from TTree emergingJetsTree/emergingJetsTree
// found on file: /afs/cern.ch/user/y/yoshin/CMSSW_7_4_12/src/EmergingJetAnalysis/ntuple.root
//////////////////////////////////////////////////////////

#ifndef BaseClass_h
#define BaseClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"
using std::vector;
#include "vector"
using std::vector;
#include "vector"
using std::vector;

class BaseClass {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           run;
   Int_t           lumi;
   Int_t           event;
   Int_t           bx;
   Float_t         met_pt;
   Float_t         met_phi;
   vector<float>   *jets_pt;
   vector<float>   *jets_eta;
   vector<float>   *jets_phi;
   vector<float>   *jets_cef;
   vector<float>   *jets_nef;
   vector<float>   *jets_chf;
   vector<float>   *jets_nhf;
   vector<float>   *jets_phf;
   vector<int>     *jets_nPromptTracks;
   vector<int>     *jets_nDispTracks;
   vector<int>     *jets_nSV;
   vector<float>   *jets_medianLogIpSig;
   vector<int>     *jets_missHits;
   vector<int>     *jets_muonHits;
   vector<float>   *jets_alphaMax;
   vector<int>     *jets_nDarkPions;
   vector<float>   *jets_minDRDarkPion;
   vector<vector<int> > *tracks_nHits;
   vector<vector<int> > *tracks_nMissInnerHits;
   vector<vector<float> > *tracks_ipXY;
   vector<vector<float> > *tracks_ipZ;
   vector<vector<float> > *tracks_ipXYSig;
   vector<float>   *vertex_x;
   vector<float>   *vertex_y;
   vector<float>   *vertex_z;
   vector<float>   *vertex_xError;
   vector<float>   *vertex_yError;
   vector<float>   *vertex_zError;
   vector<float>   *vertex_Lxy;
   vector<float>   *vertex_mass;
   vector<float>   *vertex_chi2;
   vector<float>   *vertex_ndof;
   vector<float>   *vertex_pt2sum;
   Float_t         jet0_pt;
   Float_t         jet0_eta;
   Int_t           jet0_promptTracks;
   Int_t           jet0_dispTracks;
   Int_t           jet0_nSV;
   Float_t         jet0_cef;
   Float_t         jet0_nef;
   Float_t         jet0_chf;
   Float_t         jet0_nhf;
   Float_t         jet0_phf;
   Float_t         jet0_medianLogIpSig;
   Int_t           jet0_missHits;
   Int_t           jet0_muonHits;
   Float_t         jet1_pt;
   Float_t         jet1_eta;
   Int_t           jet1_promptTracks;
   Int_t           jet1_dispTracks;
   Int_t           jet1_nSV;
   Float_t         jet1_cef;
   Float_t         jet1_nef;
   Float_t         jet1_chf;
   Float_t         jet1_nhf;
   Float_t         jet1_phf;
   Float_t         jet1_medianLogIpSig;
   Int_t           jet1_missHits;
   Int_t           jet1_muonHits;
   Float_t         jet2_pt;
   Float_t         jet2_eta;
   Int_t           jet2_promptTracks;
   Int_t           jet2_dispTracks;
   Int_t           jet2_nSV;
   Float_t         jet2_cef;
   Float_t         jet2_nef;
   Float_t         jet2_chf;
   Float_t         jet2_nhf;
   Float_t         jet2_phf;
   Float_t         jet2_medianLogIpSig;
   Int_t           jet2_missHits;
   Int_t           jet2_muonHits;
   Float_t         jet3_pt;
   Float_t         jet3_eta;
   Int_t           jet3_promptTracks;
   Int_t           jet3_dispTracks;
   Int_t           jet3_nSV;
   Float_t         jet3_cef;
   Float_t         jet3_nef;
   Float_t         jet3_chf;
   Float_t         jet3_nhf;
   Float_t         jet3_phf;
   Float_t         jet3_medianLogIpSig;
   Int_t           jet3_missHits;
   Int_t           jet3_muonHits;

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_event;   //!
   TBranch        *b_bx;   //!
   TBranch        *b_met_pt;   //!
   TBranch        *b_met_phi;   //!
   TBranch        *b_jets_pt;   //!
   TBranch        *b_jets_eta;   //!
   TBranch        *b_jets_phi;   //!
   TBranch        *b_jets_cef;   //!
   TBranch        *b_jets_nef;   //!
   TBranch        *b_jets_chf;   //!
   TBranch        *b_jets_nhf;   //!
   TBranch        *b_jets_phf;   //!
   TBranch        *b_jets_nPromptTracks;   //!
   TBranch        *b_jets_nDispTracks;   //!
   TBranch        *b_jets_nSV;   //!
   TBranch        *b_jets_medianLogIpSig;   //!
   TBranch        *b_jets_missHits;   //!
   TBranch        *b_jets_muonHits;   //!
   TBranch        *b_jets_alphaMax;   //!
   TBranch        *b_jets_nDarkPions;   //!
   TBranch        *b_jets_minDRDarkPion;   //!
   TBranch        *b_tracks_nHits;   //!
   TBranch        *b_tracks_nMissInnerHits;   //!
   TBranch        *b_tracks_ipXY;   //!
   TBranch        *b_tracks_ipZ;   //!
   TBranch        *b_tracks_ipXYSig;   //!
   TBranch        *b_vertex_x;   //!
   TBranch        *b_vertex_y;   //!
   TBranch        *b_vertex_z;   //!
   TBranch        *b_vertex_xError;   //!
   TBranch        *b_vertex_yError;   //!
   TBranch        *b_vertex_zError;   //!
   TBranch        *b_vertex_Lxy;   //!
   TBranch        *b_vertex_mass;   //!
   TBranch        *b_vertex_chi2;   //!
   TBranch        *b_vertex_ndof;   //!
   TBranch        *b_vertex_pt2sum;   //!
   TBranch        *b_jet0_pt;   //!
   TBranch        *b_jet0_eta;   //!
   TBranch        *b_jet0_promptTracks;   //!
   TBranch        *b_jet0_dispTracks;   //!
   TBranch        *b_jet0_nSV;   //!
   TBranch        *b_jet0_cef;   //!
   TBranch        *b_jet0_nef;   //!
   TBranch        *b_jet0_chf;   //!
   TBranch        *b_jet0_nhf;   //!
   TBranch        *b_jet0_phf;   //!
   TBranch        *b_jet0_medianLogIpSig;   //!
   TBranch        *b_jet0_missHits;   //!
   TBranch        *b_jet0_muonHits;   //!
   TBranch        *b_jet1_pt;   //!
   TBranch        *b_jet1_eta;   //!
   TBranch        *b_jet1_promptTracks;   //!
   TBranch        *b_jet1_dispTracks;   //!
   TBranch        *b_jet1_nSV;   //!
   TBranch        *b_jet1_cef;   //!
   TBranch        *b_jet1_nef;   //!
   TBranch        *b_jet1_chf;   //!
   TBranch        *b_jet1_nhf;   //!
   TBranch        *b_jet1_phf;   //!
   TBranch        *b_jet1_medianLogIpSig;   //!
   TBranch        *b_jet1_missHits;   //!
   TBranch        *b_jet1_muonHits;   //!
   TBranch        *b_jet2_pt;   //!
   TBranch        *b_jet2_eta;   //!
   TBranch        *b_jet2_promptTracks;   //!
   TBranch        *b_jet2_dispTracks;   //!
   TBranch        *b_jet2_nSV;   //!
   TBranch        *b_jet2_cef;   //!
   TBranch        *b_jet2_nef;   //!
   TBranch        *b_jet2_chf;   //!
   TBranch        *b_jet2_nhf;   //!
   TBranch        *b_jet2_phf;   //!
   TBranch        *b_jet2_medianLogIpSig;   //!
   TBranch        *b_jet2_missHits;   //!
   TBranch        *b_jet2_muonHits;   //!
   TBranch        *b_jet3_pt;   //!
   TBranch        *b_jet3_eta;   //!
   TBranch        *b_jet3_promptTracks;   //!
   TBranch        *b_jet3_dispTracks;   //!
   TBranch        *b_jet3_nSV;   //!
   TBranch        *b_jet3_cef;   //!
   TBranch        *b_jet3_nef;   //!
   TBranch        *b_jet3_chf;   //!
   TBranch        *b_jet3_nhf;   //!
   TBranch        *b_jet3_phf;   //!
   TBranch        *b_jet3_medianLogIpSig;   //!
   TBranch        *b_jet3_missHits;   //!
   TBranch        *b_jet3_muonHits;   //!

   BaseClass(TTree *tree=0);
   virtual ~BaseClass();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef BaseClass_cxx
BaseClass::BaseClass(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/afs/cern.ch/user/y/yoshin/CMSSW_7_4_12/src/EmergingJetAnalysis/ntuple.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/afs/cern.ch/user/y/yoshin/CMSSW_7_4_12/src/EmergingJetAnalysis/ntuple.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("/afs/cern.ch/user/y/yoshin/CMSSW_7_4_12/src/EmergingJetAnalysis/ntuple.root:/emergingJetAnalyzer");
      dir->GetObject("emergingJetsTree",tree);

   }
   Init(tree);
}

BaseClass::~BaseClass()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t BaseClass::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t BaseClass::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void BaseClass::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   jets_pt = 0;
   jets_eta = 0;
   jets_phi = 0;
   jets_cef = 0;
   jets_nef = 0;
   jets_chf = 0;
   jets_nhf = 0;
   jets_phf = 0;
   jets_nPromptTracks = 0;
   jets_nDispTracks = 0;
   jets_nSV = 0;
   jets_medianLogIpSig = 0;
   jets_missHits = 0;
   jets_muonHits = 0;
   jets_alphaMax = 0;
   jets_nDarkPions = 0;
   jets_minDRDarkPion = 0;
   tracks_nHits = 0;
   tracks_nMissInnerHits = 0;
   tracks_ipXY = 0;
   tracks_ipZ = 0;
   tracks_ipXYSig = 0;
   vertex_x = 0;
   vertex_y = 0;
   vertex_z = 0;
   vertex_xError = 0;
   vertex_yError = 0;
   vertex_zError = 0;
   vertex_Lxy = 0;
   vertex_mass = 0;
   vertex_chi2 = 0;
   vertex_ndof = 0;
   vertex_pt2sum = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("bx", &bx, &b_bx);
   fChain->SetBranchAddress("met_pt", &met_pt, &b_met_pt);
   fChain->SetBranchAddress("met_phi", &met_phi, &b_met_phi);
   fChain->SetBranchAddress("jets_pt", &jets_pt, &b_jets_pt);
   fChain->SetBranchAddress("jets_eta", &jets_eta, &b_jets_eta);
   fChain->SetBranchAddress("jets_phi", &jets_phi, &b_jets_phi);
   fChain->SetBranchAddress("jets_cef", &jets_cef, &b_jets_cef);
   fChain->SetBranchAddress("jets_nef", &jets_nef, &b_jets_nef);
   fChain->SetBranchAddress("jets_chf", &jets_chf, &b_jets_chf);
   fChain->SetBranchAddress("jets_nhf", &jets_nhf, &b_jets_nhf);
   fChain->SetBranchAddress("jets_phf", &jets_phf, &b_jets_phf);
   fChain->SetBranchAddress("jets_nPromptTracks", &jets_nPromptTracks, &b_jets_nPromptTracks);
   fChain->SetBranchAddress("jets_nDispTracks", &jets_nDispTracks, &b_jets_nDispTracks);
   fChain->SetBranchAddress("jets_nSV", &jets_nSV, &b_jets_nSV);
   fChain->SetBranchAddress("jets_medianLogIpSig", &jets_medianLogIpSig, &b_jets_medianLogIpSig);
   fChain->SetBranchAddress("jets_missHits", &jets_missHits, &b_jets_missHits);
   fChain->SetBranchAddress("jets_muonHits", &jets_muonHits, &b_jets_muonHits);
   fChain->SetBranchAddress("jets_alphaMax", &jets_alphaMax, &b_jets_alphaMax);
   fChain->SetBranchAddress("jets_nDarkPions", &jets_nDarkPions, &b_jets_nDarkPions);
   fChain->SetBranchAddress("jets_minDRDarkPion", &jets_minDRDarkPion, &b_jets_minDRDarkPion);
   fChain->SetBranchAddress("tracks_nHits", &tracks_nHits, &b_tracks_nHits);
   fChain->SetBranchAddress("tracks_nMissInnerHits", &tracks_nMissInnerHits, &b_tracks_nMissInnerHits);
   fChain->SetBranchAddress("tracks_ipXY", &tracks_ipXY, &b_tracks_ipXY);
   fChain->SetBranchAddress("tracks_ipZ", &tracks_ipZ, &b_tracks_ipZ);
   fChain->SetBranchAddress("tracks_ipXYSig", &tracks_ipXYSig, &b_tracks_ipXYSig);
   fChain->SetBranchAddress("vertex_x", &vertex_x, &b_vertex_x);
   fChain->SetBranchAddress("vertex_y", &vertex_y, &b_vertex_y);
   fChain->SetBranchAddress("vertex_z", &vertex_z, &b_vertex_z);
   fChain->SetBranchAddress("vertex_xError", &vertex_xError, &b_vertex_xError);
   fChain->SetBranchAddress("vertex_yError", &vertex_yError, &b_vertex_yError);
   fChain->SetBranchAddress("vertex_zError", &vertex_zError, &b_vertex_zError);
   fChain->SetBranchAddress("vertex_Lxy", &vertex_Lxy, &b_vertex_Lxy);
   fChain->SetBranchAddress("vertex_mass", &vertex_mass, &b_vertex_mass);
   fChain->SetBranchAddress("vertex_chi2", &vertex_chi2, &b_vertex_chi2);
   fChain->SetBranchAddress("vertex_ndof", &vertex_ndof, &b_vertex_ndof);
   fChain->SetBranchAddress("vertex_pt2sum", &vertex_pt2sum, &b_vertex_pt2sum);
   fChain->SetBranchAddress("jet0_pt", &jet0_pt, &b_jet0_pt);
   fChain->SetBranchAddress("jet0_eta", &jet0_eta, &b_jet0_eta);
   fChain->SetBranchAddress("jet0_promptTracks", &jet0_promptTracks, &b_jet0_promptTracks);
   fChain->SetBranchAddress("jet0_dispTracks", &jet0_dispTracks, &b_jet0_dispTracks);
   fChain->SetBranchAddress("jet0_nSV", &jet0_nSV, &b_jet0_nSV);
   fChain->SetBranchAddress("jet0_cef", &jet0_cef, &b_jet0_cef);
   fChain->SetBranchAddress("jet0_nef", &jet0_nef, &b_jet0_nef);
   fChain->SetBranchAddress("jet0_chf", &jet0_chf, &b_jet0_chf);
   fChain->SetBranchAddress("jet0_nhf", &jet0_nhf, &b_jet0_nhf);
   fChain->SetBranchAddress("jet0_phf", &jet0_phf, &b_jet0_phf);
   fChain->SetBranchAddress("jet0_medianLogIpSig", &jet0_medianLogIpSig, &b_jet0_medianLogIpSig);
   fChain->SetBranchAddress("jet0_missHits", &jet0_missHits, &b_jet0_missHits);
   fChain->SetBranchAddress("jet0_muonHits", &jet0_muonHits, &b_jet0_muonHits);
   fChain->SetBranchAddress("jet1_pt", &jet1_pt, &b_jet1_pt);
   fChain->SetBranchAddress("jet1_eta", &jet1_eta, &b_jet1_eta);
   fChain->SetBranchAddress("jet1_promptTracks", &jet1_promptTracks, &b_jet1_promptTracks);
   fChain->SetBranchAddress("jet1_dispTracks", &jet1_dispTracks, &b_jet1_dispTracks);
   fChain->SetBranchAddress("jet1_nSV", &jet1_nSV, &b_jet1_nSV);
   fChain->SetBranchAddress("jet1_cef", &jet1_cef, &b_jet1_cef);
   fChain->SetBranchAddress("jet1_nef", &jet1_nef, &b_jet1_nef);
   fChain->SetBranchAddress("jet1_chf", &jet1_chf, &b_jet1_chf);
   fChain->SetBranchAddress("jet1_nhf", &jet1_nhf, &b_jet1_nhf);
   fChain->SetBranchAddress("jet1_phf", &jet1_phf, &b_jet1_phf);
   fChain->SetBranchAddress("jet1_medianLogIpSig", &jet1_medianLogIpSig, &b_jet1_medianLogIpSig);
   fChain->SetBranchAddress("jet1_missHits", &jet1_missHits, &b_jet1_missHits);
   fChain->SetBranchAddress("jet1_muonHits", &jet1_muonHits, &b_jet1_muonHits);
   fChain->SetBranchAddress("jet2_pt", &jet2_pt, &b_jet2_pt);
   fChain->SetBranchAddress("jet2_eta", &jet2_eta, &b_jet2_eta);
   fChain->SetBranchAddress("jet2_promptTracks", &jet2_promptTracks, &b_jet2_promptTracks);
   fChain->SetBranchAddress("jet2_dispTracks", &jet2_dispTracks, &b_jet2_dispTracks);
   fChain->SetBranchAddress("jet2_nSV", &jet2_nSV, &b_jet2_nSV);
   fChain->SetBranchAddress("jet2_cef", &jet2_cef, &b_jet2_cef);
   fChain->SetBranchAddress("jet2_nef", &jet2_nef, &b_jet2_nef);
   fChain->SetBranchAddress("jet2_chf", &jet2_chf, &b_jet2_chf);
   fChain->SetBranchAddress("jet2_nhf", &jet2_nhf, &b_jet2_nhf);
   fChain->SetBranchAddress("jet2_phf", &jet2_phf, &b_jet2_phf);
   fChain->SetBranchAddress("jet2_medianLogIpSig", &jet2_medianLogIpSig, &b_jet2_medianLogIpSig);
   fChain->SetBranchAddress("jet2_missHits", &jet2_missHits, &b_jet2_missHits);
   fChain->SetBranchAddress("jet2_muonHits", &jet2_muonHits, &b_jet2_muonHits);
   fChain->SetBranchAddress("jet3_pt", &jet3_pt, &b_jet3_pt);
   fChain->SetBranchAddress("jet3_eta", &jet3_eta, &b_jet3_eta);
   fChain->SetBranchAddress("jet3_promptTracks", &jet3_promptTracks, &b_jet3_promptTracks);
   fChain->SetBranchAddress("jet3_dispTracks", &jet3_dispTracks, &b_jet3_dispTracks);
   fChain->SetBranchAddress("jet3_nSV", &jet3_nSV, &b_jet3_nSV);
   fChain->SetBranchAddress("jet3_cef", &jet3_cef, &b_jet3_cef);
   fChain->SetBranchAddress("jet3_nef", &jet3_nef, &b_jet3_nef);
   fChain->SetBranchAddress("jet3_chf", &jet3_chf, &b_jet3_chf);
   fChain->SetBranchAddress("jet3_nhf", &jet3_nhf, &b_jet3_nhf);
   fChain->SetBranchAddress("jet3_phf", &jet3_phf, &b_jet3_phf);
   fChain->SetBranchAddress("jet3_medianLogIpSig", &jet3_medianLogIpSig, &b_jet3_medianLogIpSig);
   fChain->SetBranchAddress("jet3_missHits", &jet3_missHits, &b_jet3_missHits);
   fChain->SetBranchAddress("jet3_muonHits", &jet3_muonHits, &b_jet3_muonHits);
   Notify();
}

Bool_t BaseClass::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void BaseClass::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t BaseClass::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef BaseClass_cxx
