//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Mar 28 05:17:09 2016 by ROOT version 6.02/05
// from TTree emergingJetsTree/emergingJetsTree
// found on file: /afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/Analysis-20160325-v0/ModelA/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/Analysis-20160325/160325_192306/ntuple_merged_ModelA.root
//////////////////////////////////////////////////////////

#ifndef BaseClass_h
#define BaseClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
using std::vector;
#include "vector"
using std::vector;
#include "vector"
using std::vector;
#include "vector"

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
   Int_t           nVtx;
   Int_t           nGoodVtx;
   Int_t           nTrueInt;
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
   vector<vector<int> > *tracks_source;
   vector<vector<float> > *tracks_pt;
   vector<vector<float> > *tracks_eta;
   vector<vector<float> > *tracks_phi;
   vector<vector<int> > *tracks_algo;
   vector<vector<int> > *tracks_originalAlgo;
   vector<vector<int> > *tracks_nHits;
   vector<vector<int> > *tracks_nMissInnerHits;
   vector<vector<int> > *tracks_nTrkLayers;
   vector<vector<int> > *tracks_nMissInnerTrkLayers;
   vector<vector<int> > *tracks_nMissOuterTrkLayers;
   vector<vector<int> > *tracks_nMissTrkLayers;
   vector<vector<int> > *tracks_nPxlLayers;
   vector<vector<int> > *tracks_nMissInnerPxlLayers;
   vector<vector<int> > *tracks_nMissOuterPxlLayers;
   vector<vector<int> > *tracks_nMissPxlLayers;
   vector<vector<float> > *tracks_ipXY;
   vector<vector<float> > *tracks_ipZ;
   vector<vector<float> > *tracks_ipXYSig;
   vector<vector<float> > *tracks_dRToJetAxis;
   vector<vector<float> > *tracks_distanceToJet;
   vector<vector<float> > *tracks_vertexLxy;
   vector<vector<int> > *jet_vertex_source;
   vector<vector<float> > *jet_vertex_x;
   vector<vector<float> > *jet_vertex_y;
   vector<vector<float> > *jet_vertex_z;
   vector<vector<float> > *jet_vertex_xError;
   vector<vector<float> > *jet_vertex_yError;
   vector<vector<float> > *jet_vertex_zError;
   vector<vector<float> > *jet_vertex_deltaR;
   vector<vector<float> > *jet_vertex_Lxy;
   vector<vector<float> > *jet_vertex_mass;
   vector<vector<float> > *jet_vertex_chi2;
   vector<vector<float> > *jet_vertex_ndof;
   vector<vector<float> > *jet_vertex_pt2sum;
   vector<int>     *vertex_source;
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

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_event;   //!
   TBranch        *b_bx;   //!
   TBranch        *b_nVtx;   //!
   TBranch        *b_nGoodVtx;   //!
   TBranch        *b_nTrueInt;   //!
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
   TBranch        *b_tracks_source;   //!
   TBranch        *b_tracks_pt;   //!
   TBranch        *b_tracks_eta;   //!
   TBranch        *b_tracks_phi;   //!
   TBranch        *b_tracks_algo;   //!
   TBranch        *b_tracks_originalAlgo;   //!
   TBranch        *b_tracks_nHits;   //!
   TBranch        *b_tracks_nMissInnerHits;   //!
   TBranch        *b_tracks_nTrkLayers;   //!
   TBranch        *b_tracks_nMissInnerTrkLayers;   //!
   TBranch        *b_tracks_nMissOuterTrkLayers;   //!
   TBranch        *b_tracks_nMissTrkLayers;   //!
   TBranch        *b_tracks_nPxlLayers;   //!
   TBranch        *b_tracks_nMissInnerPxlLayers;   //!
   TBranch        *b_tracks_nMissOuterPxlLayers;   //!
   TBranch        *b_tracks_nMissPxlLayers;   //!
   TBranch        *b_tracks_ipXY;   //!
   TBranch        *b_tracks_ipZ;   //!
   TBranch        *b_tracks_ipXYSig;   //!
   TBranch        *b_tracks_dRToJetAxis;   //!
   TBranch        *b_tracks_distanceToJet;   //!
   TBranch        *b_tracks_vertexLxy;   //!
   TBranch        *b_jet_vertex_source;   //!
   TBranch        *b_jet_vertex_x;   //!
   TBranch        *b_jet_vertex_y;   //!
   TBranch        *b_jet_vertex_z;   //!
   TBranch        *b_jet_vertex_xError;   //!
   TBranch        *b_jet_vertex_yError;   //!
   TBranch        *b_jet_vertex_zError;   //!
   TBranch        *b_jet_vertex_deltaR;   //!
   TBranch        *b_jet_vertex_Lxy;   //!
   TBranch        *b_jet_vertex_mass;   //!
   TBranch        *b_jet_vertex_chi2;   //!
   TBranch        *b_jet_vertex_ndof;   //!
   TBranch        *b_jet_vertex_pt2sum;   //!
   TBranch        *b_vertex_source;   //!
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
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/Analysis-20160325-v0/ModelA/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/Analysis-20160325/160325_192306/ntuple_merged_ModelA.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/Analysis-20160325-v0/ModelA/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/Analysis-20160325/160325_192306/ntuple_merged_ModelA.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/Analysis-20160325-v0/ModelA/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/Analysis-20160325/160325_192306/ntuple_merged_ModelA.root:/emergingJetAnalyzer");
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
   tracks_source = 0;
   tracks_pt = 0;
   tracks_eta = 0;
   tracks_phi = 0;
   tracks_algo = 0;
   tracks_originalAlgo = 0;
   tracks_nHits = 0;
   tracks_nMissInnerHits = 0;
   tracks_nTrkLayers = 0;
   tracks_nMissInnerTrkLayers = 0;
   tracks_nMissOuterTrkLayers = 0;
   tracks_nMissTrkLayers = 0;
   tracks_nPxlLayers = 0;
   tracks_nMissInnerPxlLayers = 0;
   tracks_nMissOuterPxlLayers = 0;
   tracks_nMissPxlLayers = 0;
   tracks_ipXY = 0;
   tracks_ipZ = 0;
   tracks_ipXYSig = 0;
   tracks_dRToJetAxis = 0;
   tracks_distanceToJet = 0;
   tracks_vertexLxy = 0;
   jet_vertex_source = 0;
   jet_vertex_x = 0;
   jet_vertex_y = 0;
   jet_vertex_z = 0;
   jet_vertex_xError = 0;
   jet_vertex_yError = 0;
   jet_vertex_zError = 0;
   jet_vertex_deltaR = 0;
   jet_vertex_Lxy = 0;
   jet_vertex_mass = 0;
   jet_vertex_chi2 = 0;
   jet_vertex_ndof = 0;
   jet_vertex_pt2sum = 0;
   vertex_source = 0;
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
   fChain->SetBranchAddress("nVtx", &nVtx, &b_nVtx);
   fChain->SetBranchAddress("nGoodVtx", &nGoodVtx, &b_nGoodVtx);
   fChain->SetBranchAddress("nTrueInt", &nTrueInt, &b_nTrueInt);
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
   fChain->SetBranchAddress("tracks_source", &tracks_source, &b_tracks_source);
   fChain->SetBranchAddress("tracks_pt", &tracks_pt, &b_tracks_pt);
   fChain->SetBranchAddress("tracks_eta", &tracks_eta, &b_tracks_eta);
   fChain->SetBranchAddress("tracks_phi", &tracks_phi, &b_tracks_phi);
   fChain->SetBranchAddress("tracks_algo", &tracks_algo, &b_tracks_algo);
   fChain->SetBranchAddress("tracks_originalAlgo", &tracks_originalAlgo, &b_tracks_originalAlgo);
   fChain->SetBranchAddress("tracks_nHits", &tracks_nHits, &b_tracks_nHits);
   fChain->SetBranchAddress("tracks_nMissInnerHits", &tracks_nMissInnerHits, &b_tracks_nMissInnerHits);
   fChain->SetBranchAddress("tracks_nTrkLayers", &tracks_nTrkLayers, &b_tracks_nTrkLayers);
   fChain->SetBranchAddress("tracks_nMissInnerTrkLayers", &tracks_nMissInnerTrkLayers, &b_tracks_nMissInnerTrkLayers);
   fChain->SetBranchAddress("tracks_nMissOuterTrkLayers", &tracks_nMissOuterTrkLayers, &b_tracks_nMissOuterTrkLayers);
   fChain->SetBranchAddress("tracks_nMissTrkLayers", &tracks_nMissTrkLayers, &b_tracks_nMissTrkLayers);
   fChain->SetBranchAddress("tracks_nPxlLayers", &tracks_nPxlLayers, &b_tracks_nPxlLayers);
   fChain->SetBranchAddress("tracks_nMissInnerPxlLayers", &tracks_nMissInnerPxlLayers, &b_tracks_nMissInnerPxlLayers);
   fChain->SetBranchAddress("tracks_nMissOuterPxlLayers", &tracks_nMissOuterPxlLayers, &b_tracks_nMissOuterPxlLayers);
   fChain->SetBranchAddress("tracks_nMissPxlLayers", &tracks_nMissPxlLayers, &b_tracks_nMissPxlLayers);
   fChain->SetBranchAddress("tracks_ipXY", &tracks_ipXY, &b_tracks_ipXY);
   fChain->SetBranchAddress("tracks_ipZ", &tracks_ipZ, &b_tracks_ipZ);
   fChain->SetBranchAddress("tracks_ipXYSig", &tracks_ipXYSig, &b_tracks_ipXYSig);
   fChain->SetBranchAddress("tracks_dRToJetAxis", &tracks_dRToJetAxis, &b_tracks_dRToJetAxis);
   fChain->SetBranchAddress("tracks_distanceToJet", &tracks_distanceToJet, &b_tracks_distanceToJet);
   fChain->SetBranchAddress("tracks_vertexLxy", &tracks_vertexLxy, &b_tracks_vertexLxy);
   fChain->SetBranchAddress("jet_vertex_source", &jet_vertex_source, &b_jet_vertex_source);
   fChain->SetBranchAddress("jet_vertex_x", &jet_vertex_x, &b_jet_vertex_x);
   fChain->SetBranchAddress("jet_vertex_y", &jet_vertex_y, &b_jet_vertex_y);
   fChain->SetBranchAddress("jet_vertex_z", &jet_vertex_z, &b_jet_vertex_z);
   fChain->SetBranchAddress("jet_vertex_xError", &jet_vertex_xError, &b_jet_vertex_xError);
   fChain->SetBranchAddress("jet_vertex_yError", &jet_vertex_yError, &b_jet_vertex_yError);
   fChain->SetBranchAddress("jet_vertex_zError", &jet_vertex_zError, &b_jet_vertex_zError);
   fChain->SetBranchAddress("jet_vertex_deltaR", &jet_vertex_deltaR, &b_jet_vertex_deltaR);
   fChain->SetBranchAddress("jet_vertex_Lxy", &jet_vertex_Lxy, &b_jet_vertex_Lxy);
   fChain->SetBranchAddress("jet_vertex_mass", &jet_vertex_mass, &b_jet_vertex_mass);
   fChain->SetBranchAddress("jet_vertex_chi2", &jet_vertex_chi2, &b_jet_vertex_chi2);
   fChain->SetBranchAddress("jet_vertex_ndof", &jet_vertex_ndof, &b_jet_vertex_ndof);
   fChain->SetBranchAddress("jet_vertex_pt2sum", &jet_vertex_pt2sum, &b_jet_vertex_pt2sum);
   fChain->SetBranchAddress("vertex_source", &vertex_source, &b_vertex_source);
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
