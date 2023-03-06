
//#include "/afs/cern.ch/user/p/pmandrik/public/global_cfg/mRoot.cpp"
//#include "/afs/cern.ch/user/p/pmandrik/public/global_cfg/mRootStackDrawer.cpp"

#include "global_cfg/mRoot.cpp"
#include "global_cfg/mRootStackDrawer.cpp"
#include "tmva_train.C"

using namespace mRoot;

TH1D * tmva_use_dataset(string path, string hist_name, string fname, string tname, string dname, string tmva_name, double extra_weight){
  TFile * file = TFile::Open( (path + "/" + fname).c_str(), "READ" );
  TTreeReader * ttree_reader = new TTreeReader(tname.c_str(), file);
  TMVA::Reader* tmva_reader = new TMVA::Reader( "<options>" );

  vector<TTreeFormula*> values;
  vector<float*> values_f;
  TTreeFormula weight("weight", "weight", ttree_reader->GetTree());

  auto vars = get_variables(true);
  for(auto vname : vars){
    values.push_back( new TTreeFormula(vname.c_str(), vname.c_str(), ttree_reader->GetTree()) );
    values_f.push_back( new float );
    tmva_reader->AddVariable( vname.c_str(), (values_f.at( values.size() - 1 )) );
  }

  //                                                                    /weights/TMVA_    TMlp_ANN     .weights.xml
  auto tmva_method = tmva_reader->BookMVA( tmva_name.c_str(), (dname + "/weights/TMVA_" + tmva_name + ".weights.xml").c_str() );
  cout << tmva_method << endl;
  cout << tmva_method->GetMvaValue() << endl;
  cout << tmva_reader->GetMVAError() << endl;

  TH1D * hist = new TH1D(hist_name.c_str(), hist_name.c_str(), 100, 0, 1);
  int event_number_max = 10;
  while(ttree_reader->Next()){
    for(int i = 0; i < values_f.size(); i++) *(values_f[i]) = values.at(i)->EvalInstance();

    hist->Fill( tmva_method->GetMvaValue(), weight.EvalInstance() * extra_weight );

    // cout << tmva_method->GetMvaValue() << endl;
    // if(not event_number_max) break;
    // event_number_max--;
  }
  hist->Print();
  return hist;
} 

void tmva_use(){
  string out_path = "/afs/cern.ch/work/p/pmandrik/tdr_tqgamma/mensura/hzura_mva_jan_02";
         out_path = "hzura_mva_jan_02";

  vector<TH1D*> hists;
  string path = "/afs/cern.ch/work/p/pmandrik/tdr_tqgamma/mensura/hzura_mva_jan_02";
         path = "hzura_mva_jan_02";
  string run_mode    = "TMVA_v13";
  string tmva_method = "MLP";
  // tmva_method = "BDT";
  string tree_name   = "data";

  hists.push_back( tmva_use_dataset( path, "tcg",     "tcg.root",     tree_name, run_mode, tmva_method, 10 ) );
  hists.push_back( tmva_use_dataset( path, "TT",      "TT.root",      tree_name, run_mode, tmva_method, 1 ) );
  hists.push_back( tmva_use_dataset( path, "Wjets",   "Wjets.root",   tree_name, run_mode, tmva_method, 1 ) );
  hists.push_back( tmva_use_dataset( path, "ST",      "ST.root",      tree_name, run_mode, tmva_method, 1 ) );
  hists.push_back( tmva_use_dataset( path, "tW",      "tW.root",      tree_name, run_mode, tmva_method, 1 ) );
  hists.push_back( tmva_use_dataset( path, "DY",      "DY.root",      tree_name, run_mode, tmva_method, 1 ) );
//hists.push_back( tmva_use_dataset( path, "QCD",     "QCD.root",     tree_name, run_mode, tmva_method, 1 ) );
  hists.push_back( tmva_use_dataset( path, "Diboson", "Diboson.root", tree_name, run_mode, tmva_method, 1 ) );

  StackDrawer * drawer = new StackDrawer();
  for(int i = 0; i < hists.size(); i++){
    TH1D* hist = hists.at(i);
    if( string(hist->GetName()) != "tcg" ) drawer->stack_hists.push_back( hist );
    else                                   drawer->signal_hists.push_back( hist );
  }
  // auto canv = drawer->GetCanvas("BDT", 640, 640);
  auto canv = drawer->GetCanvas("MLP", 640, 640);
  canv->Print( (out_path + "/" + run_mode + "_" + tmva_method + ".png").c_str() );
}




