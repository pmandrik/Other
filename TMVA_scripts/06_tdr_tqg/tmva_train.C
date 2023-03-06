

/*

                        : 
                         : Training Network
                         : 
^C                       : [>>>>>..........] (36%, time left: 5 sec) [<D^2> (train/test): -nan/-nan]  
Killed                   : [>>>>>>>>>>>>>..] (86%, time left: 1 sec) [<D^2> (train/test): -nan/-nan]  


*/

TTree * get_tree(string file_name, string tree_name){
  TFile * file = TFile::Open( file_name.c_str() );
  file->ls();
  auto tree = (TTree*) file->Get(tree_name.c_str());
  cout << tree << endl;
  return (TTree*) file->Get(tree_name.c_str());
}

string scalar_mult(string A, string B, bool only_definition = false){
  answer = "(" + A + "_p3, " + B + "_p3) := ";
  if(only_definition) answer = "";
  answer += A+"_Px * " + B + "_Px + ";
  answer += A+"_Py * " + B + "_Py + ";
  answer += A+"_Pz * " + B + "_Pz ";
  return answer;
}


void tmva_train(){
  int try_index = 4;

  // 0 nCuts=20:MaxDepth=3
  // 1 nCuts=100:MaxDepth=10:NTrees=50:SeparationType=CrossEntropy
  // 2 nCuts=100:MaxDepth=10:NTrees=120:SeparationType=GiniIndexWithLaplace
  // 3 nCuts=100:MaxDepth=10:NTrees=120:SeparationType=SDivSqrtSPlusB
  // 4 nCuts=50:MaxDepth=10:NTrees=120:BoostType=RealAdaBoost

  TFile* outputFile = TFile::Open( ("TMVA_MLP_v" + to_string(try_index) + ".root").c_str(), "RECREATE" );
  TMVA::Factory *factory = new TMVA::Factory( "TMVA", outputFile, "" );
  TMVA::DataLoader *dataloader=new TMVA::DataLoader( ("TMVA_MLP_v" + to_string(try_index)) );

  dataloader->SetWeightExpression("weight");
  double extra_weight = 1.;
/*
  dataloader->AddSignalTree    (get_tree("hzura_crab_dec_24_v2_10_ST_tch_14T_stic_v2-v2_part0.root"),   extra_weight, TMVA::Types::kTraining);
  dataloader->AddBackgroundTree(get_tree("hzura_crab_dec_24_v2_14_TT_TuneCUE_stic_v2-v3_part0.root"),   extra_weight, TMVA::Types::kTraining);
  dataloader->AddBackgroundTree(get_tree("hzura_crab_dec_24_v2_27_ST_tW_DR_1_v2_ext1-v1_part0.root"),   extra_weight, TMVA::Types::kTraining);
  dataloader->AddSignalTree    (get_tree("hzura_crab_dec_24_v2_10_ST_tch_14T_stic_v2-v2_part0.root"),   extra_weight, TMVA::Types::kTesting);
  dataloader->AddBackgroundTree(get_tree("hzura_crab_dec_24_v2_14_TT_TuneCUE_stic_v2-v3_part0.root"),   extra_weight, TMVA::Types::kTesting);
  dataloader->AddBackgroundTree(get_tree("hzura_crab_dec_24_v2_27_ST_tW_DR_1_v2_ext1-v1_part0.root"),   extra_weight, TMVA::Types::kTesting);
*/


  dataloader->AddSignalTree    (get_tree("samples/tcg.root",     "ttree;3"), extra_weight, TMVA::Types::kTraining);
//dataloader->AddBackgroundTree(get_tree("samples/Wjets.root",   "ttree;1"), extra_weight, TMVA::Types::kTraining);
//dataloader->AddBackgroundTree(get_tree("samples/tW.root",      "ttree;1"), extra_weight, TMVA::Types::kTraining);
  dataloader->AddBackgroundTree(get_tree("samples/TT.root",      "ttree;1"), extra_weight, TMVA::Types::kTraining);
//dataloader->AddBackgroundTree(get_tree("samples/ST.root",      "ttree;1"), extra_weight, TMVA::Types::kTraining);
//dataloader->AddBackgroundTree(get_tree("samples/QCD.root",     "ttree;1"), extra_weight, TMVA::Types::kTraining);
//dataloader->AddBackgroundTree(get_tree("samples/DY.root",      "ttree;1"), extra_weight, TMVA::Types::kTraining);
//dataloader->AddBackgroundTree(get_tree("samples/Diboson.root", "ttree;1"), extra_weight, TMVA::Types::kTraining);

  dataloader->AddSignalTree    (get_tree("samples/tcg.root",     "ttree;4"), extra_weight, TMVA::Types::kTesting);
//dataloader->AddBackgroundTree(get_tree("samples/Wjets.root",   "ttree;2"), extra_weight, TMVA::Types::kTesting);
//dataloader->AddBackgroundTree(get_tree("samples/tW.root",      "ttree;2"), extra_weight, TMVA::Types::kTesting);
  dataloader->AddBackgroundTree(get_tree("samples/TT.root",      "ttree;2"), extra_weight, TMVA::Types::kTesting);
//dataloader->AddBackgroundTree(get_tree("samples/ST.root",      "ttree;2"), extra_weight, TMVA::Types::kTesting);
//dataloader->AddBackgroundTree(get_tree("samples/QCD.root",     "ttree;2"), extra_weight, TMVA::Types::kTesting);
//dataloader->AddBackgroundTree(get_tree("samples/DY.root",      "ttree;2"), extra_weight, TMVA::Types::kTesting);
//dataloader->AddBackgroundTree(get_tree("samples/Diboson.root", "ttree;2"), extra_weight, TMVA::Types::kTesting);

  vector<string> vars_names = {"Sphericity", "Aplanarity", "MVA_bJetTop_dPhi", "LeadingJetSecondJet_Pt"
                               //"MVA_bJetTop_dPhi" // "MVA_bJetLepton_Mass", "MVA_bJetTop_dPhi"//, "MVA_bJetTop_dEta",
                               //"cos_lightJetLepton_tsystem", "cos_lightJetW_tsystem", "cos_leptonW_Wsystem",
                               //"Sphericity", "Aplanarity", //, "t_Pt", "bJet_Pt",
                               //"LeadingJetSecondJet_Pt", "Jets_M_without_bJet" // , "LeadingJetSecondJet_dR"
                                /* 
                                "Lepton_Type",
                                "JetsW_M", "Jets_SumPt_minus_bJet_Pt", "Sphericity", "MVA_bJetLepton_Pt",
                                "MET_Pt", "W_Pt", "W_Mt", "t_M", "LightJet_Pt", "LeadingJet_Pt", "SecondJet_Pt",
                                "MVA_bJetW_dPhi", "LeadingJetSecondJet_M", "Jets_SumPt", "Lepton_Pt"
                                */
                              };
  for(auto var : vars_names){
    dataloader->AddVariable( var.c_str(), 'F');
  }

  vector<string> objects = {"Lepton", "LeadingJet", "SecondJet"};
  for(int i = 0; i < objects.size(); i++){
    for(int j = i+1; j < objects.size(); j++){
      string objA = objects.at(i);
      string objB = objects.at(j);
      // dataloader->AddVariable( scalar_mult(objA, objB).c_str(), 'F');
    }
  }

  //dataloader->PrepareTrainingAndTestTree("", "nTrain_Signal=200:nTrain_Background=100" );
  factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT", "nCuts=50:MaxDepth=10:NTrees=120:BoostType=RealAdaBoost" );
  //factory->BookMethod( dataloader, TMVA::Types::kMLP, "MLP_ANN", "NCycles=200:EstimatorType=linear" );
  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();
}



