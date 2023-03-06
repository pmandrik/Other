


TTree * get_tree(string name){
  TFile * file = TFile::Open( name.c_str() );
  return (TTree*) file->Get("ttree");
}

int train_tmva(){

  TTree* s_ttree    = get_tree("ww_output.root");
  TTree* b_ttree_a  = get_tree("wjets_output.root");
  TTree* b_ttree_b  = get_tree("qcd_output.root");
  
  TMVA::Tools::Instance();

  TFile* outputFile = TFile::Open( "TMVA.root", "RECREATE" );
  TMVA::Factory* factory = new TMVA::Factory( "TMVA", outputFile, "" );

  factory->AddSignalTree(s_ttree, 1.0, TMVA::Types::kTraining);
  factory->AddBackgroundTree(b_ttree_a, 1.0, TMVA::Types::kTraining);
  factory->AddBackgroundTree(b_ttree_b, 1.0, TMVA::Types::kTraining);

  factory->AddSignalTree(s_ttree, 1.0, TMVA::Types::kTesting);
  factory->AddBackgroundTree(b_ttree_a, 1.0, TMVA::Types::kTesting);
  factory->AddBackgroundTree(b_ttree_b, 1.0, TMVA::Types::kTesting);

  // factory->SetWeightExpression( "<YourWeightExpression>" );

  // vector<string> vars_names = {"G_PT", "BJ_BTAG", "BJ_PT", "MU_PT", "COS_T_G", "dR_BJ_G", "dR_Mu_G", "MU_CHARGE", "J_MULT"};
  vector<string> vars_names = {"BJ_PT", "J_MULT"};
  for(auto var : vars_names){
    factory->AddVariable( var.c_str(), 'F');
  }

  // factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodD", "" );
  //   factory->PrintHelpMessage("LikelihoodD");
  factory->BookMethod( TMVA::Types::kCuts, "Cuts", "" );

  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();

  return 0;
}
