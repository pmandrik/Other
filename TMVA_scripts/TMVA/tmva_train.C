

TTree * get_tree(string path, string file_name, string tree_name){
  TFile * file = TFile::Open( (path + "/" + file_name).c_str() );
  file->ls();
  auto tree = (TTree*) file->Get(tree_name.c_str());
  cout << tree << endl;
  return (TTree*) file->Get(tree_name.c_str());
}

string scalar_mult(string A, string B, bool only_definition = false){
  string answer = "(" + A + "_P, " + B + "_P) := ";
  if(only_definition) answer = "";
  answer += "log(abs(";
  answer += A+"_E *  " + B + "_E  - ";
  answer += A+"_Px * " + B + "_Px - ";
  answer += A+"_Py * " + B + "_Py - ";
  answer += A+"_Pz * " + B + "_Pz " + "))";
  return answer;
}

vector<string> get_variables(bool for_use=false){
  vector<string> answer = {"cos_lightJetLepton_tsystem", "cos_lightJetW_tsystem", "cos_leptonW_Wsystem",
                           "Sphericity", "Planarity", "LeadingJetSecondJet_dR",
                           "log(t_M)", "log(bJet_Pt)", "log(t_Pt)", "log(Lepton_Pt)", "log(LightJet_Pt)", "log(W_Pt)", "log(Jets_SumPt)",
                           "log(LeadingJetSecondJet_Pt)", "log(Jets_M_without_bJet)", "log(MVA_bJetLepton_Mass)",
                           "MVA_bJetTop_dPhi", "MVA_bJetTop_dEta","Lepton_Type"

                              //"MVA_bJetTop_dPhi", "MVA_bJetTop_dEta",//,
                               //"cos_lightJetLepton_tsystem", "cos_lightJetW_tsystem", "cos_leptonW_Wsystem",
                               //"Sphericity", "Planarity", "t_Pt", "bJet_Pt", "t_M",
                               //"LeadingJetSecondJet_Pt", "LeadingJetSecondJet_dR", "Jets_M_without_bJet"

                                /* 
                                "Lepton_Type",
                                "JetsW_M", "Jets_SumPt_minus_bJet_Pt", "Sphericity", "MVA_bJetLepton_Pt",
                                "MET_Pt", "W_Pt", "W_Mt", "t_M", "LightJet_Pt", "LeadingJet_Pt", "SecondJet_Pt",
                                "MVA_bJetW_dPhi", "LeadingJetSecondJet_M", "Jets_SumPt", "Lepton_Pt"
                                */
                              };

  vector<string> objects = {"Lepton", "LeadingJet", "SecondJet"};
  for(int i = 0; i < objects.size(); i++){
    for(int j = i+1; j < objects.size(); j++){
      string objA = objects.at(i);
      string objB = objects.at(j);
      answer.push_back(scalar_mult(objA, objB, for_use).c_str());
    }
  }

  return answer;
}

void tmva_train(){
  int try_index = 14;
  map<int, string> options_map;
  // options_map[0] = "NCycles=50:HiddenLayers=N-1,N+10,3:V:TestRate=5";
  options_map[0] = "NCycles=100:TestRate=25:LearningRate=0.025:HiddenLayers=N,N+10,N,";
  options_map[1] = "NCycles=300:TestRate=25:LearningRate=0.025:HiddenLayers=N,N+10,N,";
  options_map[2] = "NCycles=150:TestRate=25:LearningRate=0.025:HiddenLayers=N,N+20,N:EstimatorType=tanh";
  options_map[3] = "NCycles=100:TestRate=25:LearningRate=0.025:HiddenLayers=N,N-1";
  options_map[4] = "NCycles=100:TestRate=25:LearningRate=0.025:HiddenLayers=N,N-1"; // only electron channel
  options_map[5] = "NCycles=100:TestRate=25:LearningRate=0.025:HiddenLayers=N,N-1"; // only electron channel, more data
  options_map[6] = "NCycles=100:TestRate=25:LearningRate=0.025:HiddenLayers=N+10,N-1"; // only muon channel, more data, more background

  options_map[7] = "NCycles=200:TestRate=25:LearningRate=0.025:HiddenLayers=N+10,N-1:EstimatorType=CE";
  options_map[8] = "NCycles=200:TestRate=25:LearningRate=0.025:HiddenLayers=N+10,N-1";
  options_map[9] = "NCycles=1000:TestRate=25:LearningRate=0.025:HiddenLayers=N+10,N-1";
  options_map[10] = "NCycles=1000:TestRate=25:LearningRate=0.025:HiddenLayers=N+10,N+10";
  options_map[11] = "NCycles=1000:TestRate=25:LearningRate=0.025:HiddenLayers=N+20,N+10";
  options_map[12] = "NCycles=1000:TestRate=25:LearningRate=0.025:HiddenLayers=N+26,N+16";
  options_map[13] = "NCycles=1000:TestRate=100:LearningRate=0.02:HiddenLayers=N+25,N+20";
  options_map[14] = "NCycles=2750:TestRate=500:LearningRate=0.02:HiddenLayers=N+50,N+50";
 

  TFile* outputFile = TFile::Open( ("TMVA_v" + to_string(try_index) + ".root").c_str(), "RECREATE" );
  TMVA::Factory *factory = new TMVA::Factory( "TMVA", outputFile, "" );
  TMVA::DataLoader *dataloader=new TMVA::DataLoader( ("TMVA_v" + to_string(try_index)) );

  dataloader->SetWeightExpression("weight");
  double extra_weight = 1;

  string path = "/afs/cern.ch/work/p/pmandrik/tdr_tqgamma/mensura/hzura_mva_jan_02";
  path = "hzura_mva_jan_02";
  dataloader->AddSignalTree    (get_tree(path, "tcg.root",     "train"), extra_weight, TMVA::Types::kTraining);
  dataloader->AddSignalTree    (get_tree(path, "tcg.root",     "test"),  extra_weight, TMVA::Types::kTraining);

  dataloader->AddBackgroundTree(get_tree(path, "Wjets.root",   "train"), extra_weight, TMVA::Types::kTraining);
  dataloader->AddBackgroundTree(get_tree(path, "tW.root",      "train"), extra_weight, TMVA::Types::kTraining);
  dataloader->AddBackgroundTree(get_tree(path, "TT.root",      "train"), extra_weight, TMVA::Types::kTraining);
  dataloader->AddBackgroundTree(get_tree(path, "ST.root",      "train"), extra_weight, TMVA::Types::kTraining);
//dataloader->AddBackgroundTree(get_tree(path, "QCD.root",     "train"), extra_weight, TMVA::Types::kTraining);
  dataloader->AddBackgroundTree(get_tree(path, "DY.root",      "train"), extra_weight, TMVA::Types::kTraining);
  dataloader->AddBackgroundTree(get_tree(path, "Diboson.root", "train"), extra_weight, TMVA::Types::kTraining);

  dataloader->AddBackgroundTree(get_tree(path, "tW.root",      "test"), extra_weight, TMVA::Types::kTraining);
  dataloader->AddBackgroundTree(get_tree(path, "TT.root",      "test"), extra_weight, TMVA::Types::kTraining);
  dataloader->AddBackgroundTree(get_tree(path, "ST.root",      "test"), extra_weight, TMVA::Types::kTraining);
//dataloader->AddBackgroundTree(get_tree(path, "QCD.root",     "test"), extra_weight, TMVA::Types::kTraining);
  dataloader->AddBackgroundTree(get_tree(path, "DY.root",      "test"), extra_weight, TMVA::Types::kTraining);
  dataloader->AddBackgroundTree(get_tree(path, "Diboson.root", "test"), extra_weight, TMVA::Types::kTraining);

  dataloader->AddSignalTree    (get_tree(path, "tcg.root",     "data"), extra_weight, TMVA::Types::kTesting);
  dataloader->AddBackgroundTree(get_tree(path, "Wjets.root",   "data"), extra_weight, TMVA::Types::kTesting);
  dataloader->AddBackgroundTree(get_tree(path, "tW.root",      "data"), extra_weight, TMVA::Types::kTesting);
  dataloader->AddBackgroundTree(get_tree(path, "TT.root",      "data"), extra_weight, TMVA::Types::kTesting);
  dataloader->AddBackgroundTree(get_tree(path, "ST.root",      "data"), extra_weight, TMVA::Types::kTesting);
//dataloader->AddBackgroundTree(get_tree(path, "QCD.root",     "data"), extra_weight, TMVA::Types::kTesting);
  dataloader->AddBackgroundTree(get_tree(path, "DY.root",      "data"), extra_weight, TMVA::Types::kTesting);
  dataloader->AddBackgroundTree(get_tree(path, "Diboson.root", "data"), extra_weight, TMVA::Types::kTesting);

  auto variables = get_variables();
  for(auto var : variables){
    dataloader->AddVariable( var.c_str(), 'F' );
  }

//dataloader->PrepareTrainingAndTestTree("", "NormMode=NumEvents:nTrain_Signal=5000:nTrain_Background=5000" );
//dataloader->PrepareTrainingAndTestTree("", "NormMode=None" );
//dataloader->PrepareTrainingAndTestTree("Lepton_Type==1", "NormMode=EqualNumEvents:nTrain_Signal=20000:nTrain_Background=20000:nTest_Signal=10000" );
  dataloader->PrepareTrainingAndTestTree("", "NormMode=EqualNumEvents" );
  factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT", "nCuts=50:MaxDepth=3:NTrees=50" );
  factory->BookMethod( dataloader, TMVA::Types::kMLP, "MLP", options_map.find( try_index )->second.c_str() );
//factory->BookMethod( dataloader, TMVA::Types::kTMlpANN, "TMlp_ANN", options_map.find( try_index )->second.c_str() );
  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();
}



