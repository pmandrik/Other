

void tmva_check(){
  vector<TH1D * > hists;
  for(int i = 0; i < 100; i++){
    TFile * file = TFile::Open( ("TMVA_v" + to_string(i) + ".root").c_str() );
    if(not file) break;
    file->ls();
    TH1D * hist = (TH1D*)file->Get( ("TMVA_v" + to_string(i)+"/Method_BDT/BDT/MVA_BDT_rejBvsS").c_str() );
    if(not hist) break;
    hist->SetName(("TMVA_v" + to_string(i)).c_str());
    hists.push_back( hist );
  }

  TCanvas *canv = new TCanvas("c", "c", 640, 640);
  for(int i = 0; i < hists.size(); i++){
    TH1D * hist = hists.at(i);
    if(i) hist->Draw("hist same");
    else hist->Draw("hist");

    hist->SetLineColor(1 + i);
  }
} 
