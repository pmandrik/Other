
string scalar_mult(string A, string B, bool only_definition = false){
  answer = "(" + A + "_P, " + B + "_P) := ";
  if(only_definition) answer = "";
  answer += A+"_E * " + B + "_E - ";
  answer += A+"_Px * " + B + "_Px - ";
  answer += A+"_Py * " + B + "_Py - ";
  answer += A+"_Pz * " + B + "_Pz ";
  return answer;
}

TH1D * tmva_use_dataset(string path, string fname, string tname){
  
  TFile * file = TFile::Open( (path + "/" + fname).c_str(), "READ" );
  TTreeReader * ttree_reader = new TTreeReader(tname.c_str(), file);
  TMVA::Reader* tmva_reader = new TMVA::Reader( "<options>" );

  vector<string> vars_names = {"MVA_bJetLepton_Mass", "MVA_bJetTop_dPhi", "MVA_bJetTop_dEta",
                               "cos_lightJetLepton_tsystem", "cos_lightJetW_tsystem", "cos_leptonW_Wsystem",
                               "Planarity", "Lepton_Type",
                               "Lepton_Pt", "t_Pt", "bJet_Pt",
                               "LeadingJetSecondJet_Pt", "LeadingJetSecondJet_dR", 
                               "Jets_SumPt","Jets_M_without_bJet"};

  vector<string> objects = {"Lepton", "LeadingJet", "SecondJet"};

  vector<TTreeFormula*> values;
  vector<float*> values_f;
  TTreeFormula weight("weight", "weight", ttree_reader->GetTree());

  for(auto vname : vars_names){
    values.push_back( new TTreeFormula(vname.c_str(), vname.c_str(), ttree_reader->GetTree()) );
    values_f.push_back( new float );
    tmva_reader->AddVariable( vname.c_str(), (values_f.at( values.size() - 1 )) );
  }
  for(int i = 0; i < objects.size(); i++){
    for(int j = i+1; j < objects.size(); j++){
      string objA = objects.at(i);
      string objB = objects.at(j);
      values.push_back( new TTreeFormula(scalar_mult(objA, objB).c_str(), scalar_mult(objA, objB, true).c_str(), ttree_reader->GetTree()) );
      values_f.push_back( new float );
      tmva_reader->AddVariable( scalar_mult(objA, objB).c_str(), (values_f.at( values.size() - 1 )) );
    }
  }

  tmva_reader->BookMVA( "BDT", "dataset/weights/TMVA_BDT.weights.xml" );
  auto tmva_bdt = tmva_reader->FindMVA("BDT");
  cout << tmva_bdt->GetMvaValue() << endl;

  TH1D * hist = new TH1D(fname.c_str(), fname.c_str(), 100, -1, 1);
  int event_number_max = 100000;
  while(ttree_reader->Next()){
    for(int i = 0; i < values_f.size(); i++) *(values_f[i]) = values[i]->EvalInstance();
    hist->Fill( tmva_bdt->GetMvaValue(), weight.EvalInstance() );
  }
  hist->Print();
  return hist;
} 

void tmva_use(){
  vector<TH1D*> hists;
  hists.push_back( tmva_use_dataset( "samples", "QCD.root",     "ttree;3" ) );
  hists.push_back( tmva_use_dataset( "samples", "Wjets.root",   "ttree;3" ) );
  hists.push_back( tmva_use_dataset( "samples", "tW.root",      "ttree;3" ) );
  hists.push_back( tmva_use_dataset( "samples", "TT.root",      "ttree;3" ) );
  hists.push_back( tmva_use_dataset( "samples", "ST.root",      "ttree;3" ) );
  hists.push_back( tmva_use_dataset( "samples", "DY.root",      "ttree;3" ) );
  hists.push_back( tmva_use_dataset( "samples", "Diboson.root", "ttree;3" ) );
  hists.push_back( tmva_use_dataset( "samples", "tcg.root",     "ttree;5" ) );

  TCanvas *canv = new TCanvas("c", "c", 640, 640);
  for(int i = 0; i < hists.size(); i++){
    TH1D* hist = hists.at(i);

    if(not i) hist->Draw("hist");
    else      hist->Draw("hist same");
    
    if(i == hists.size()-1) hist->SetLineColor(2);
    else                    hist->SetLineColor(3);
  }
  // canv->Print("answer.");
}







