


TTree * get_tree(string name, vector<float> * vals, vector<string> names){
  TFile * file = TFile::Open( name.c_str() );
  TTree* theTree = (TTree*) file->Get("ttree");

  for(int i = 0; i < names.size(); i++){
    theTree->SetBranchAddress( names.at(i).c_str(), &(vals->at(i)) );
  }

  return theTree;
}

int use_tmva(){
  TMVA::Tools::Instance();

  TMVA::Reader* reader = new TMVA::Reader( "<options>" );
  vector<string> vars_names = {"BJ_PT", "J_MULT"};
  vector<float> vals;
  for(auto var : vars_names){
    vals.push_back(0);
    
    reader->AddVariable( var.c_str(), &(vals.at( vals.size()-1 )));
  }

  reader->BookMVA( "Cuts", "weights/TMVA_Cuts.weights.xml" );

  TTree* s_ttree    = get_tree("ww_output.root", &vals, vars_names);
  TTree* b_ttree_a  = get_tree("wjets_output.root", &vals, vars_names);
  TTree* b_ttree_b  = get_tree("qcd_output.root", &vals, vars_names);

  vector<TTree*> trees = {s_ttree, b_ttree_a, b_ttree_b};

  TCanvas * canv_a = new TCanvas("canv_a", "canv", 640, 480);
  TCanvas * canv_b = new TCanvas("canv_b", "canv", 640, 480);
  int index = 2;
  for(auto tree : trees){
    TH1D * hist_a = new TH1D( (string("BJ_PT")  + to_string(index-2)).c_str(),(string("BJ_PT")  + to_string(index)).c_str(),100, 0, 1500 );
    TH1D * hist_b = new TH1D( (string("J_MULT") + to_string(index-2)).c_str(),(string("J_MULT") + to_string(index)).c_str(),100, 0, 100 );
    
    for(int i = 0; i < tree->GetEntries(); i++){
      tree->GetEntry(i);

      bool passed = reader->EvaluateMVA( "Cuts", 0.50 );
      // cout << vals.at(0) << " " << vals.at(1) << " " << passed << endl;
      if(passed){
        hist_a->Fill( vals.at(0) );
        hist_b->Fill( vals.at(1) );
      }
    }
    canv_a->cd();

    hist_a->Print();

    hist_a->SetLineColor( index ); hist_a->SetLineWidth( index );
    hist_b->SetLineColor( index ); hist_b->SetLineWidth( index );
    if(index == 2) hist_a->Draw();
    else hist_a->Draw("HIST same");

    canv_b->cd();
    if(index == 2) hist_b->Draw();
    else hist_b->Draw("HIST same");
    index++;
  }


  return 0;
}















