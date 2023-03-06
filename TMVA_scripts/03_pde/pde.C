
#include "/home/me/work/TMVA_Study/02_rectangular_cut/rectangular_cut.C"

void make_toy_pde(){
  auto background  = tmva_file();
  auto signal      = tmva_file();

  background.make_files("background");
  signal.make_files("signal");

  float VAR_X, VAR_Y;
  background.add_var("VAR_X", VAR_X);
  background.add_var("VAR_Y", VAR_Y);

  signal.add_var("VAR_X", VAR_X);
  signal.add_var("VAR_Y", VAR_Y);

  for(int i = 0; i < 20000; i++){
    VAR_X = _random_engine.Uniform(0, 2);
    VAR_Y = _random_engine.Uniform(0, 2);
         if(i < 5000)  background.tree_train->Fill();
    else if(i < 10000) background.tree_test->Fill();
    else               background.tree_data->Fill();

    VAR_X = 1. + 0.1 * _random_engine.Gaus();
    VAR_Y = 1. + 0.3 * _random_engine.Gaus();
         if(i < 5000)  signal.tree_train->Fill();
    else if(i < 10000) signal.tree_test->Fill();
    else               signal.tree_data->Fill();
  }

  background.save();
  signal.save();
}

void train_pde(){
  TFile* outputFile = TFile::Open( "TMVA.root", "RECREATE" );
  TMVA::Factory *factory = new TMVA::Factory( "TMVA", outputFile, "" );
  TMVA::DataLoader *dataloader=new TMVA::DataLoader("dataset");

  dataloader->AddSignalTree    (get_tree("signal_train.root"), 1.0, TMVA::Types::kTraining);
  dataloader->AddBackgroundTree(get_tree("background_train.root"), 1.0, TMVA::Types::kTraining);

  dataloader->AddSignalTree    (get_tree("signal_test.root"), 1.0, TMVA::Types::kTesting);
  dataloader->AddBackgroundTree(get_tree("background_test.root"), 1.0, TMVA::Types::kTesting);

  vector<string> vars_names = {"VAR_X", "VAR_Y"};
  for(auto var : vars_names){
    dataloader->AddVariable( var.c_str(), 'F');
  }

  factory->BookMethod( dataloader, TMVA::Types::kLikelihood, "PDE", "!TransformOutput:" );
  //factory->BookMethod( dataloader, TMVA::Types::kPDERS, "PDERS", "" );

  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();
}

void use_pde(){
  TMVA::Reader* reader = new TMVA::Reader( "<options>" );
  vector<string> vars_names = {"VAR_X", "VAR_Y"};
  vector<float> vals;
  for(auto var : vars_names){
    vals.push_back(0);
  }

  for(int i = 0; i < vars_names.size(); i++){
    reader->AddVariable( vars_names.at(i).c_str(), &(vals.at( i )) );
  }

  reader->BookMVA( "PDE", "dataset/weights/TMVA_PDE.weights.xml" );
  reader->BookMVA( "PDERS", "dataset/weights/TMVA_PDERS.weights.xml" );
  
  TH2D * hist_s = new TH2D( "signal@VAR_X@VAR_Y",     "signal_VAR_X@VAR_Y",     100, 0, 2, 100, 0, 2 );
  TH2D * hist_b = new TH2D( "background@VAR_X@VAR_Y", "background_VAR_X@VAR_Y", 100, 0, 2, 100, 0, 2 );

  TH1D * hist_s_mva = new TH1D( "signal@MVA",     "signal@MVA",     100, 0, 1);
  TH1D * hist_b_mva = new TH1D( "background@MVA", "background@MVA", 100, 0, 1);

  auto st = get_tree("signal_data.root", &vals, vars_names);
  auto bt = get_tree("background_data.root", &vals, vars_names);

  auto pde = reader->FindMVA("PDE");
  cout << pde->GetMvaValue() << endl;

  for(int i = 0; i < st->GetEntries(); i++){
    st->GetEntry(i);
    hist_s->Fill(vals.at(0), vals.at(1));
    double mva_value = reader->EvaluateMVA("PDE");
    hist_s_mva->Fill( mva_value );
  }

  for(int i = 0; i < bt->GetEntries(); i++){
    bt->GetEntry(i);
    hist_b->Fill(vals.at(0), vals.at(1));
    double mva_value = reader->EvaluateMVA("PDE");
    cout << vals.at(0) << " " << vals.at(1) << " " << mva_value << endl;
    hist_b_mva->Fill( mva_value );
  }

  TH2D * hist_pdf = new TH2D( "PDF",     "PDF",     100, 0, 2, 100, 0, 2 );
  for(int x = 0; x < 100; x++){
    for(int y = 0; y < 100; y++){
      vals.at(0) = 2. * x / 100.+0.05;
      vals.at(1) = 2. * y / 100.+0.05;
      hist_pdf->Fill(vals.at(0), vals.at(1), reader->EvaluateMVA("PDE"));
    }
  }
  TCanvas * canv3 = new TCanvas("PDF canv", "PDF canv", 640, 480);
  hist_pdf->Draw("col2");


  TCanvas * canv = new TCanvas("VAR_X@VAR_Y", "VAR_X@VAR_Y", 640, 480);
  hist_b->Draw();
  hist_s->Draw("same");
  hist_s->SetMarkerColor(kRed);

  TCanvas * canv2 = new TCanvas("MVA separation", "MVA separation", 640, 480);
  hist_s_mva->Draw();
  hist_s_mva->SetLineWidth(2);
  hist_s_mva->SetLineColor(4);

  hist_b_mva->Draw("same");
  hist_b_mva->SetLineWidth(2);
  hist_b_mva->SetLineColor(2);
}

int pde(){
  TMVA::Tools::Instance();

  make_toy_pde();
  train_pde();
  use_pde();

  return 0;
}









