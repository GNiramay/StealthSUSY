// Driver script to analyze the final ntuples
#include"PreSelection.C"
bool DEBUG_DRIVER = false;
void driver(){
  
  // // TChiWZ_ZtoLL
  // TString InROOT = "ROOT_IN/newSample1.root";
  // TString OutROOT = "ROOT_OUT/results.root";

  // // SUS-Stealth
  // TString InROOT = "ROOT_IN/SUS-Stealth.root";
  // TString OutROOT = "ROOT_OUT/SUS-Stealth.root";

  // // SUS-Stealth higher statistics
  // TString InROOT = "ROOT_IN/SUS-Stealth_2k.root";
  // TString OutROOT = "ROOT_OUT/SUS-Stealth_2k.root";

  // SUS-Stealth 2k events, prepared at fnal
  TString InROOT = "ROOT_IN/Autumn18Fast.Newsample6_RA2AnalysisTree.root";
  TString OutROOT = "ROOT_OUT/Autumn18Fast.Newsample6_RA2AnalysisTree_2.root";
  TString outkey = "sgetBB";

  // // SUS-Stealth 2k events, prepared at fnal, sget->2 gluino
  // TString InROOT = "ROOT_IN/Autumn18Fast.glgl.root";
  // TString OutROOT = "ROOT_OUT/Autumn18Fast.glgl.root";
  // TString outkey = "sgetGG";
  
  TFile* ff = new TFile(InROOT);
  TTree* tr = (TTree*)ff->Get("TreeMaker2/PreSelection");

  auto t = new PreSelection(tr);
  if(DEBUG_DRIVER) t->Show(13);
  t->BookHistograms(OutROOT);
  t->Loop();
  t->MakePDF(outkey);
  delete t;
}
