
#include "global_cfg/mRoot.cpp"

using namespace mRoot;

TCanvas * mvas(TTree * tree, string branch, string cut, string name){
  float val;
  Int_t type;
  tree->SetBranchAddress( branch.c_str(), &val  );
  tree->SetBranchAddress( "classID",      &type );

  string formula = "weight";
  if( cut != "" ) formula += " * (" + cut + ")";
  TTreeFormula weight(formula.c_str(), formula.c_str(), tree);

  int nevents = tree->GetEntries();

  float max = -9999;
  float min =  9999;

  for(int i = 0; i < nevents; i++){
    tree->GetEntry(i);
    max = TMath::Max(max, val);
    min = TMath::Max(min, val);
  }

  TH1D * hback = new TH1D((name+"b").c_str(), "b", 50, min, max);
  TH1D * hsign = new TH1D((name+"s").c_str(), "s", 50, min, max);
  
  for(int i = 0; i < nevents; i++){
    double wei = weight.EvalInstance();
    tree->GetEntry(i);
         if(type==1) hback->Fill(val, wei);
    else if(type==0) hsign->Fill(val, wei);
  }

  auto canv = get_canvas();
  hsign->Draw("hist");
  hsign->SetStats(0);
  hback->Draw("hist same");

  hback->SetLineWidth(3);
  hsign->SetLineWidth(3);

  hback->SetLineColor(1);
  hsign->SetLineColor(2);

  // hback->Scale( 1./hback->Integral() );
  // hsign->Scale( 1./hsign->Integral() );

  canv->SetTitle( name.c_str() );
  return canv;
}
 
