#define PreSelection_cxx
#include "PreSelection_2.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void PreSelection::Loop()
{
  if (fChain == 0) return;

  // fChain->SetBranchStatus("*",0);  // disable all branches
  // fChain->SetBranchStatus("GenParticles_ParentId",1);
  // fChain->SetBranchStatus("GenParticles_ParentIdx",1);
  // fChain->SetBranchStatus("GenParticles_PdgId",1);
  // fChain->SetBranchStatus("GenParticles_Status",1);
  // fChain->SetBranchStatus("GenParticles",1);
  // fChain->SetBranchStatus("GenJets",1);
  // fChain->SetBranchStatus("GenJetsAK8",1);
  // fChain->SetBranchStatus("GenMET",1);
  // fChain->SetBranchStatus("GenHT",1);
  Long64_t nentries = fChain->GetEntriesFast();
  cout<<"Total events: "<<nentries<<endl;

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if (Cut(ientry) > 0) continue;

    int nParticles[pid_list.size()];
    int nChi = 0;		// No. of chi's
    float GvPt[2] = {0,0};
    TLorentzVector tlv;		// To add gvino Pt
    TLorentzVector tlv_tot;	// To add chi's Pt
    int hist_id{-1};		// index used for histograms with or without lepton veto
    if(LeptonVeto) hist_id = 0;
    else hist_id = 1;
    
    for(int i=0;i<pid_list.size();i++) nParticles[i]=0;
      
    for(int pp=0;pp<GenParticles_PdgId->size();pp++){ // Loop over gen particles
      int part_id = GenParticles_PdgId->at(pp);
      if(abs(part_id)== myPDG->GetID("sgino") && GenParticles_Status->at(pp)!=22) continue;// Ignore singlinos without status 22
      for(int ii=0;ii<pid_list.size();ii++){
	if(abs(part_id)==pid_list[ii]){
	  nParticles[ii]++;
	  if(ii<7)
	    hGenMass[ii]->Fill(GenParticles->at(pp).M());
	  continue;
	}
      }
	
      for(int cc=0;cc<2;cc++){
	if(abs(GenParticles_ParentId->at(pp))==pid_list[7+cc])
	  BosDecay[cc]->Fill(abs(GenParticles_PdgId->at(pp)));
	if(abs(part_id)==pid_list[7+cc]){
	  BosPt[cc]->Fill(GenParticles->at(pp).Pt());
	  if(LeptonVeto && GenAK8Jet3) hBosPt_3AK8[cc]->Fill(GenParticles->at(pp).Pt());
	}
      }
      
      if(abs(part_id)== myPDG->GetID("gvino")){
	if(nParticles[6]==0) tlv = GenParticles->at(pp);
	else tlv += GenParticles->at(pp);
      }

      nChi = nParticles[0]+nParticles[3];
      if(abs(part_id)== myPDG->GetID("chi1_+") || abs(part_id)== myPDG->GetID("chi2_0")){
	if(nChi==0) tlv_tot = GenParticles->at(pp);
	else tlv_tot += GenParticles->at(pp);
      }

    }	// Loop over gen particles END

    // Gen Jet analysis
    hGenMET[hist_id]->Fill(GenMET);
    hGenHT[hist_id]->Fill(GenHT);

    hGvPt[hist_id]->Fill(tlv.Pt());
    hUKMET[hist_id]->Fill(GenMET-tlv.Pt());
    hTotPt[hist_id]->Fill(tlv_tot.Pt());
    hMET_Gv_dPhi[hist_id]->Fill(abs(GenMETPhi-tlv.Phi()));

    TLorentzVector  v1,v2; 
    v1.SetPtEtaPhiM(GenMET, 0.0, GenMETPhi, 0.0);
    v2.SetPtEtaPhiM(tlv.Pt(), tlv.Eta(), tlv.Phi(), tlv.M());
    hUKMET_Vec[hist_id]->Fill ((v1-v2).Pt()); //eta and M of v1-v2 have no meaning.
    
    // if(LeptonVeto && tlv.Pt()>200){
    //   printf("pp\tGenParticles_PdgId->at(pp)\tGenParticles_ParentId->at(pp)\tGenParticles_ParentIdx->at(pp)\tGenParticles_Status->at(pp)\tGenParticles->at(pp).Px()\tGenParticles->at(pp).Py()\tGenParticles->at(pp).Pz()\tGenParticles->at(pp).M()\tGenMET\tNJetsISR\tMET-GvPt\n%f\t%d\n",GenMET,NJetsISR);
      
    //   for(int pp=0;pp<GenParticles_PdgId->size();pp++){ // Loop over gen particles
    // 	printf("%d\t%d\t%d\t%d\t%d\t%f\t%f\t%f\t%f\t%f\n",
    // 	       pp,
    // 	       GenParticles_PdgId->at(pp),
    // 	       GenParticles_ParentId->at(pp),
    // 	       GenParticles_ParentIdx->at(pp),
    // 	       GenParticles_Status->at(pp),
    // 	       GenParticles->at(pp).Px(),
    // 	       GenParticles->at(pp).Py(),
    // 	       GenParticles->at(pp).Pz(),
    // 	       GenParticles->at(pp).M(),
    // 	       (v1-v2).Pt()
    // 	       );
    //   }
    // }

    auto SortedGenJets = SortPt(GenJets);	// Sort Gen AK4 jets according to Pt
    auto SortedGenJetsAK8 = SortPt(GenJetsAK8); // Sort Gen AK8 jets according to Pt
    auto SortedJetsAK8 = SortPt(JetsAK8);	// Sort Reco AK8 jets according to Pt

    hNGenJets[hist_id][0]->Fill(GenJetsAK8->size());
    hNGenJets[hist_id][1]->Fill(GenJets->size());

    hGenJetPt[hist_id][0]->Fill(SortedGenJetsAK8[0].Pt());
    hGenJetMass[hist_id][0]->Fill(SortedGenJetsAK8[0].M());

    hGenJetPt[hist_id][1]->Fill(SortedGenJets[0].Pt());
    hGenJetMass[hist_id][1]->Fill(SortedGenJets[0].M());

    for(int i=0;i<2;i++){
      hJetPt[hist_id][i]->Fill(SortedJetsAK8[i].Pt());
      hJetMass[hist_id][i]->Fill(SortedJetsAK8[i].M());
    }
    
    for(int ii=0;ii<pid_list.size();ii++)
      hGenNum[ii]->Fill(nParticles[ii]);
  }
}

void PreSelection::BookHistograms(TString ofroot)
{
  for(int ii=0;ii<pname_list.size();ii++)
    pid_list.push_back(myPDG->GetID(pname_list[ii]));
  tf_out = new TFile(ofroot,"RECREATE");
  BosDecay[0] = new TH1F("BosDecay_W","Daughters of W",20,0,20);
  BosDecay[1] = new TH1F("BosDecay_Z","Daughters of Z",20,0,20);

  BosPt[0] = new TH1F("BosPt_W","W Pt",100,0,1000);
  BosPt[1] = new TH1F("BosPt_Z","Z Pt",100,0,1000);

  hBosPt_3AK8[0] = new TH1F("hBosPt_3AK8_W","W Pt | 3 AK8 jet events",100,0,1000);
  hBosPt_3AK8[1] = new TH1F("hBosPt_3AK8_Z","Z Pt | 3 AK8 jet events",100,0,1000);

  hSgetPt_3AK8[0] = new TH1F("hSgetPt_3AK8_W","Leading singlet Pt | 3 AK8 jet events",100,0,1000);
  hSgetPt_3AK8[1] = new TH1F("hSgetPt_3AK8_Z","Sub-leading singlet Pt | 3 AK8 jet events",100,0,1000);
  hJetPt[i][0] = new TH1F(Form("hLJetPt_%d",i),info1[i]+" | Leading AK8 Jet P_{T};[GeV]",500,0,2500);
  hJetPt[i][1] = new TH1F(Form("hSLJetPt_%d",i),info1[i]+" | Sub-leading AK8 Jet P_{T};[GeV]",500,0,2500);
  hJetMass[i][0] = new TH1F(Form("hLJetMass_%d",i),info1[i]+" | Leading AK8 Jet Mass;[GeV]",200,0,1e3);
  hJetMass[i][1] = new TH1F(Form("hSLJetMass_%d",i),info1[i]+" | Sub-leading AK8 Jet Mass;[GeV]",200,0,1e3);
  
  for(int ii=0;ii<pid_list.size();ii++){
    hGenNum[ii] = new TH1F("hGenNum_"+pname_list[ii],"No. of "+myPDG->GetName(pname_list[ii])+" per event;Gen particle multiplicity",10,0,10);
  }

  for(int ii=0;ii<7;ii++){
    hGenMass[ii] = new TH1F("hGenMass_"+pname_list[ii],myPDG->GetName(pname_list[ii])+" invariant mass;[GeV]",100,0,1e3);
  }

  // With and without Lepton veto
  TString info1[2] = {"passing lepton veto","failing lepton veto"};
  TString info2[2] = {"AK8","AK4"};
  for(int i=0;i<2;i++){		// Loop over veto presence
    hGenMET[i] = new TH1F(Form("hGenMET_%d",i),info1[i]+" | Gen MET;[GeV]",200,0,1e3);
    hGenHT[i] = new TH1F(Form("hGenHT_%d",i),info1[i]+" | Gen HT;[GeV]",100,0,4e3);
    hGvPt[i] = new TH1F(Form("hGvPt_%d",i),info1[i]+" | Total vector sum of gravitino P_{T};[GeV]",100,0,500);
    hTotPt[i] = new TH1F(Form("hTotPt_%d",i),info1[i]+" | Total vector summed P_{T} of #chi 's;[GeV]",100,0,500);
    hUKMET[i] = new TH1F(Form("hUKMET_%d",i),info1[i]+" | MET - Gravitino P_{T};[GeV]",100,0,500);
    hUKMET_Vec[i] = new TH1F(Form("hUKMET_Vec_%d",i),info1[i]+" | MET - Gravitino P_{T} (Vectorial);[GeV]",100,0,500);
    hMET_Gv_dPhi[i] = new TH1F(Form("hMET_Gv_dPhi_%d",i),info1[i]+" | abs(#Delta#phi(MET,Gravitino P_{T}));radians",70,0,7);
    hJetPt[i][0] = new TH1F(Form("hLJetPt_%d",i),info1[i]+" | Leading AK8 Jet P_{T};[GeV]",500,0,2500);
    hJetPt[i][1] = new TH1F(Form("hSLJetPt_%d",i),info1[i]+" | Sub-leading AK8 Jet P_{T};[GeV]",500,0,2500);
    hJetMass[i][0] = new TH1F(Form("hLJetMass_%d",i),info1[i]+" | Leading AK8 Jet Mass;[GeV]",200,0,1e3);
    hJetMass[i][1] = new TH1F(Form("hSLJetMass_%d",i),info1[i]+" | Sub-leading AK8 Jet Mass;[GeV]",200,0,1e3);
    
    for(int j=0;j<2;j++){	// Loop over jet type
      hGenJetPt[i][j] = new TH1F(Form("hGenJetPt_%d_%d",i,j),info1[i]+";Leading Gen "+info2[j]+" Jet P_{T} [GeV]",100,0,2e3);
      hGenJetMass[i][j] = new TH1F(Form("hGenJetMass_%d_%d",i,j),info1[i]+";Leading Gen "+info2[j]+" Jet Mass [GeV]",100,0,200);
      hNGenJets[i][j] = new TH1F(Form("hNGenJets_%d_%d",i,j),info1[i]+";Number of Gen "+info2[j]+" Jets",30,0,30);
    }
  }
}

vector<TLorentzVector> PreSelection::SortPt(vector<TLorentzVector>* tv)
{
  vector<TLorentzVector> tv2 = *tv;

  for(int i=0;i<tv2.size();i++){
    for(int j=i+1;i<tv2.size();i++){
      if(tv2[j].Pt()>tv2[i].Pt())
	swap(tv2[i],tv2[j]);
    }
  }
  return tv2;
}

void PreSelection::MakePDF(TString outkey)
{
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  TCanvas* tc = new TCanvas("aa","bb",800,600);
  vector<TH1F*>hlist;

  // Leading, subleading reco jet Pt
  {
    hlist = {hJetPt[0][0],hJetPt[0][1]};
    hlist[0]->Draw("hist"); Decorate(hlist[0],1,"",4);
    hlist[1]->Draw("sames"); Decorate(hlist[1],2,"",4);
    GetMaxY(hlist);
    tc->BuildLegend(.3,.7,.9,.9);
    tc->SetGrid();
    SaveMe(tc,"LJetAK8Pt",outkey);
    hlist.clear();    
  }

  // Leading, subleading reco jet Mass
  {
    hlist = {hJetMass[0][0],hJetMass[0][1]};
    hlist[0]->Draw("hist"); Decorate(hlist[0],1);
    hlist[1]->Draw("sames"); Decorate(hlist[1],2);
    GetMaxY(hlist);
    tc->BuildLegend(.3,.7,.9,.9);
    tc->SetGrid();
    hlist[0]->GetXaxis()->SetRangeUser(0,500);
    SaveMe(tc,"LJetAK8Mass",outkey);
    hlist.clear();    
  }

  // MET related overlays
  {
    hlist = {hGenMET[0],hGvPt[0],hUKMET_Vec[0],hTotPt[0]};
    hlist[0]->Draw("hist"); Decorate(hlist[0],1);
    hlist[1]->Draw("sames"); Decorate(hlist[1],2);
    hlist[2]->Draw("sames"); Decorate(hlist[2],3);
    hlist[3]->Draw("sames"); Decorate(hlist[3],4);
    GetMaxY(hlist);
    tc->BuildLegend(.3,.7,.9,.9);
    tc->SetGrid();
    tc->SetLogy();
    hlist[0]->GetXaxis()->SetRangeUser(0,500);
    SaveMe(tc,"MET",outkey);
    hlist.clear();    
  }
}
