
TRandom3 _random_engine;

inline Color_t FindFreeCustomColorIndex(Color_t start = 1000){
   for (Color_t i = std::max(start, Color_t(1000)), e = 10000; i < e; ++i)
      if (!gROOT->GetColor(i))
         return i;
   return -1;
}

int get_colors(int N, int Nmax){
  double r = (1.-float(N)/Nmax);
  double g = float(N)/Nmax;
  double b = 0.4;
  int index = FindFreeCustomColorIndex();
  auto color = new TColor(index, r, g, b);
  return index;
}

struct tmva_file{
  TFile *file_train, *file_test, *file_data;
  TTree *tree_train, *tree_test, *tree_data;

  void make_files(string name){
    file_train = new TFile( (name + "_train.root").c_str(), "RECREATE");
    tree_train = new TTree("ttree", "ttree");

    file_test = new TFile( (name + "_test.root").c_str(), "RECREATE");
    tree_test = new TTree("ttree", "ttree");

    file_data = new TFile( (name + "_data.root").c_str(), "RECREATE");
    tree_data = new TTree("ttree", "ttree");
  }

  void save(){
    file_train->cd();
    tree_train->Write();
    file_test->cd();
    tree_test->Write();
    file_data->cd();
    tree_data->Write();
  }

  template<typename T> void add_var(const char * name, T & var){
    tree_train->Branch(name, &var);
    tree_test ->Branch(name, &var);
    tree_data ->Branch(name, &var);
  }
};

void make_toy(){
  auto background  = tmva_file();
  auto signal      = tmva_file();

  background.make_files("background");
  signal.make_files("signal");

  float VAR_X, VAR_Y;
  background.add_var("VAR_X", VAR_X);
  background.add_var("VAR_Y", VAR_Y);

  signal.add_var("VAR_X", VAR_X);
  signal.add_var("VAR_Y", VAR_Y);

  for(int i = 0; i < 5000; i++){
    VAR_X = _random_engine.Uniform(0, 2);
    VAR_Y = _random_engine.Uniform(0, 2);
    background.tree_train->Fill();

    VAR_X = 1. + 0.1 * _random_engine.Gaus();
    VAR_Y = 1. + 0.3 * _random_engine.Gaus();
    signal.tree_train->Fill();
  }

  for(int i = 0; i < 5000; i++){
    VAR_X = _random_engine.Uniform(0, 2);
    VAR_Y = _random_engine.Uniform(0, 2);
    background.tree_test->Fill();

    VAR_X = 1. + 0.1 * _random_engine.Gaus();
    VAR_Y = 1. + 0.3 * _random_engine.Gaus();
    signal.tree_test->Fill();
  }

  for(int i = 0; i < 15000; i++){
    VAR_X = _random_engine.Uniform(0, 2);
    VAR_Y = _random_engine.Uniform(0, 2);
    background.tree_data->Fill();

    VAR_X = 1. + 0.1 * _random_engine.Gaus();
    VAR_Y = 1. + 0.3 * _random_engine.Gaus();
    signal.tree_data->Fill();
  }

  background.save();
  signal.save();
}

TTree * get_tree(string name){
  TFile * file = TFile::Open( name.c_str() );
  return (TTree*) file->Get("ttree");
}

TTree * get_tree(string name, vector<float> * vals, vector<string> names){
  TFile * file = TFile::Open( name.c_str() );
  TTree* theTree = (TTree*) file->Get("ttree");
  for(int i = 0; i < names.size(); i++){
    theTree->SetBranchAddress( names.at(i).c_str(), &(vals->at(i)) );
  }
  return theTree;
}

void train_cut(){
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

  factory->BookMethod( dataloader, TMVA::Types::kCuts, "Cuts", "" );

  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();
}

void use_cut(){
  TMVA::Reader* reader = new TMVA::Reader( "<options>" );
  vector<string> vars_names = {"VAR_X", "VAR_Y"};
  vector<float> vals;
  for(auto var : vars_names){
    vals.push_back(0);
    reader->AddVariable( var.c_str(), &(vals.at( vals.size()-1 )));
  }

  reader->BookMVA( "Cuts", "dataset/weights/TMVA_Cuts.weights.xml" );
  
  TH2D * hist_s = new TH2D( "signal@VAR_X@VAR_Y",     "signal_VAR_X@VAR_Y",     100, 0, 2, 100, 0, 2 );
  TH2D * hist_b = new TH2D( "background@VAR_X@VAR_Y", "background_VAR_X@VAR_Y", 100, 0, 2, 100, 0, 2 );

  auto st = get_tree("signal_data.root", &vals, vars_names);
  auto bt = get_tree("background_data.root", &vals, vars_names);

  for(int i = 0; i < st->GetEntries(); i++){
    st->GetEntry(i);
    hist_s->Fill(vals.at(0), vals.at(1));
  }

  for(int i = 0; i < bt->GetEntries(); i++){
    bt->GetEntry(i);
    hist_b->Fill(vals.at(0), vals.at(1));
  }

  hist_s->SetMarkerColor(kRed);

  TCanvas * canv = new TCanvas("VAR_X@VAR_Y", "VAR_X@VAR_Y", 640, 480);
  hist_b->Draw();
  hist_s->Draw("same");

  TMVA::MethodCuts* tmva_cuts = reader->FindCutsMVA("Cuts");
  vector<Double_t> cutMin, cutMax;

  
  int nmax = 10;
  for(int i = 1; i < nmax; i++){ 
    tmva_cuts->GetCuts(1. * i / nmax, cutMin, cutMax );
    tmva_cuts->PrintCuts(1. * i / nmax);

    double min_x = cutMin.at(0), max_x = cutMax.at(0);
    double min_y = cutMin.at(1), max_y = cutMax.at(1);

    auto box = new TBox(min_x, min_y, max_x, max_y);
    box->SetFillStyle(0);
    box->SetLineWidth(4.0);
    box->SetLineColor( get_colors(i, nmax) );
    box->Draw();
  }
}

int rectangular_cut(){
  TMVA::Tools::Instance();

  // make_toy();
  // train_cut();
  use_cut();

  return 0;
}









