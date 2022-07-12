// A library of commonly used functions and variables

// Common list of strings, integers and floats
TString quark_names[6]={"d","u","s","c","b","t"};
vector<TString> pname_list={
			    "chi1_+", // 0
			    "chi2_+", // 1
			    "chi1_0", // 2
			    "chi2_0", // 3
			    "sgino",  // 4
			    "sget",   // 5
			    "gvino",  // 6
			    "w",     // 7
			    "z",     // 8
			    "d",     // 9
			    "u",     // 10
			    "s",     // 11
			    "c",     // 12
			    "b",     // 13
			    "t"     // 14
};

struct MyPDGParticles
{
  MyPDGParticles();
  vector<TString> codename;		// C++ friendly codenames of the particles. Something that can be used in histogram name, variable name etc.
  vector<TString> name;			// Human friendly names of the particle. Something that can be used in the histogram title etc.
  vector<int> pdgid;			// PDG IDs of the particles
  vector<float> mass;			// Masses of the particles in MeV (whichever are known)
  int nparticles;			// no. of particles in the list

  void AddParticle(TString,TString,int,float);	// Include a new particle to the list
  int GetID(TString);				// Returns index of the particle matching the input codename
  float GetMass(TString);			// Returns mass of the particle matching the input codename
  TString GetName(TString);			// Returns human readable name of the particle matching the input codename
};

void MyPDGParticles::AddParticle(TString codename_,TString name_,int pdgid_,float mass_=0.)
{
  codename.push_back(codename_);
  name.push_back(name_);
  pdgid.push_back(pdgid_);
  mass.push_back(mass_);
  nparticles++;
}

MyPDGParticles::MyPDGParticles()
{
  codename.clear();
  name.clear();
  pdgid.clear();
  mass.clear();
  nparticles = 0;
}

int MyPDGParticles::GetID(TString cc)
{
  for(int i=0;i<nparticles;i++)
    if(codename[i]==cc) return pdgid[i];
  return -1;
}

float MyPDGParticles::GetMass(TString cc)
{
  for(int i=0;i<nparticles;i++)
    if(codename[i]==cc) return mass[i];
  return -1;
}

TString MyPDGParticles::GetName(TString cc)
{
  for(int i=0;i<nparticles;i++)
    if(codename[i]==cc) return name[i];
  return "";
}

MyPDGParticles* GetParticles()
{
  MyPDGParticles* mm = new MyPDGParticles();

  // Quarks
  mm->AddParticle("d","down",1,4.1);
  mm->AddParticle("u","up",2,1.7);
  mm->AddParticle("s","strange",3,101);
  mm->AddParticle("c","charm",4,1270);
  mm->AddParticle("b","bottom",5,4200);
  mm->AddParticle("t","top",6,4200);

  // Leptons
  mm->AddParticle("e","electron",11,0.5);
  mm->AddParticle("mu","muon",13,105);
  mm->AddParticle("tau","tauon",15,1776);

  // Bosons
  mm->AddParticle("gamma","Photon",22,0);
  mm->AddParticle("z","Z-boson",23,91000);
  mm->AddParticle("w","W-boson",24,80000);
  mm->AddParticle("h","Higgs-boson",37,125000);

  // SUSY
  mm->AddParticle("chi1_0","#chi_{1}^{0}",1000022);
  mm->AddParticle("chi2_0","#chi_{2}^{0}",1000023);
  mm->AddParticle("chi1_+","#chi_{1}^{+}",1000024);
  mm->AddParticle("chi3_0","#chi_{3}^{0}",1000025);
  mm->AddParticle("chi4_0","#chi_{4}^{0}",1000035);
  mm->AddParticle("chi2_+","#chi_{2}^{+}",1000037);
  mm->AddParticle("gvino","gravitino",1000039);
  mm->AddParticle("sgino","singlino",5000001);
  mm->AddParticle("sget","singlet",5000002);
  return mm;
}

void SaveMe(TCanvas*,TString,TString outkey="");
void SaveMe(TCanvas* tc,TString fname,TString outkey){
  tc->SaveAs("/home/niramay/NIRAMAY/Work/Stealth_SUSY/NTuples_Analysis/PDF/"+fname+"_"+outkey+".pdf");
  // delete tc;
}

void GetMaxY(vector<TH1F*>hh);	// sets maximum Y in all the given histograms
void GetMaxY(vector<TH1F*>hh)
{
  float YMax = 0.;
  for(int i=0;i<hh.size();i++){
    if(YMax<hh[i]->GetMaximum()) YMax=hh[i]->GetMaximum();
  }
  hh[0]->GetYaxis()->SetRangeUser(.1+hh[0]->GetYaxis()->GetXmin(),1.1*YMax);
}

void Decorate(TH1F* hh,int color_,TString htitle="",int rebin=0); // Decorate histograms
void Decorate(TH1F* hh,int color_,TString htitle,int rebin)
{
  hh->SetLineWidth(3);
  hh->SetLineColor(color_);
  if(htitle!="") hh->SetTitle(htitle);
  if(rebin>0) hh->Rebin(rebin);
}
