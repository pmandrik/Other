
#include "mvas.C"
#include "tmva_train.C"

int f_begin = 10;
int f_end   = 14;

void draw_comp_hist(string path="/Method_BDT/BDT/MVA_BDT_rejBvsS", string name="c"){
  // draw 
  vector<TH1D * > hists;
  for(int i = f_begin; i < f_end; i++){
    TH1D * hist = nullptr;
    TFile * file = TFile::Open( ("TMVA_v" + to_string(i) + ".root").c_str() );
    cout << file << endl;
    if(not file) break;
    file->ls();
    hist = (TH1D*)((TH1D*)file->Get( ("TMVA_v" + to_string(i)+path).c_str() ))->Clone();
    if(not hist) break;
    hist->SetName(("TMVA_v" + to_string(i)).c_str());
    hists.push_back( hist );
  }

  TCanvas *canv = new TCanvas(name.c_str(), name.c_str(), 640, 640);
  for(int i = 0; i < hists.size(); i++){
    TH1D * hist = hists.at(i);
    if(i) hist->Draw("hist same");
    else hist->Draw("hist");

    hist->SetLineColor(1 + i);
  }
}

void draw_comp_ttree(string path="/TestTree", string vname="MLP", string name="c"){
  // draw 
  vector<TH1D * > hists;
  for(int i = f_begin; i < f_end; i++){
    TFile * file = TFile::Open( ("TMVA_v" + to_string(i) + ".root").c_str() );
    if(not file) break;
    file->ls();
    TTree * tree = (TTree*)file->Get( ("TMVA_v" + to_string(i)+path).c_str() );
    if(not tree) break;
    string hname = "TMVA_v" + to_string(i) + "_" + vname + "_" + name;
    TH1D * hist = new TH1D(hname.c_str(), hname.c_str(), 50, 0, 1);
    tree->Draw( (vname + ">>" + hname).c_str() );
    hists.push_back( hist );
  }

  TCanvas *canv = new TCanvas(name.c_str(), name.c_str(), 640, 640);
  for(int i = 0; i < hists.size(); i++){
    TH1D * hist = hists.at(i);
    if(i) hist->Draw("hist same");
    else hist->Draw("hist");
    hist->Scale( 1./hist->Integral() );
    hist->SetLineColor(1 + i);
  }
}

void draw_mva_distributions(string vname="MLP", string name="c"){
  for(int i = f_begin; i < f_end; i++){
    TFile * file = TFile::Open( ("TMVA_v" + to_string(i) + ".root").c_str() );
    if(not file) break;
    file->ls();

    string tpath = ("TMVA_v" + to_string(i) + "/TestTree");
    mvas( (TTree*)file->Get( tpath.c_str() ), "MLP", "", ("TMVA_v" + to_string(i)) );
  }
}

void draw_variables(){
  auto vars = get_variables();

  TFile * file = TFile::Open( "TMVA_v6.root" );
  string tpath = ("TMVA_v6/TestTree");
  TTree * tree = (TTree*)file->Get( tpath.c_str() );
  for(string var : vars){
    mvas( tree, var, "MLP > 0.75", var );
  }
}

void tmva_check(){
  // draw_comp_ttree("/TrainTree", "MLP", "c2");
  draw_comp_hist("/Method_MLP/MLP/MVA_MLP_rejBvsS", "c1");
  draw_mva_distributions();
  // draw_variables();
} 













