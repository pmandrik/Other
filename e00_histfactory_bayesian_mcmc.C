
/*
  e00_histfactory_bayesian_mcmc.C

    C++ ROOT v6-06-00 script example of Bayesian analyse with HistFactory and RooStats tools:
      1. generate input MC histograms for signal and background processes, 
         produce data histogram as sum of templates
      2. setup statistical model with HistFactory witch include different complex
         normalization factors, statistical error and shape variation parameters
         and import it into RooWorkspace
      3. add custom priors and define list of nuisances and POIs,
         performe Bayesian analyse with Markov Chain Monte Carlo, save chain into TTree
      4. read chain from TTree and find upper limits of parameters
      5. produce toy histograms from statistical model
    See more information in related presentation.

  P.Mandrik, IHEP, 10.06.17
*/

//===================================== some global configurations ===================================== 
double RANGE_MIN             = 0;
double RANGE_MAX             = 100;
int    N_BACKGROUNDS         = 5;
int    NBINS                 = 50;
const char * INPUT_FILE_NAME = "e00_input_histograms_file.root";
const char * CHAIN_FILE_NAME = "e00_mcmc_chain_file.root";
const char * TOYS_FILE_NAME  = "e00_toys_file.root";
double SIGNAL_P              = 0.20;
double SIGNAL_P_alt          = 0.05;
int    MCMC_LENGTH           = 50000;
int    N_TOYS                = 100;
double STAT_ERROR_THRESHOLD  = 0.0001;

bool HISTFACTORY_USE_STAT_ERRORS              = true;
bool HISTFACTORY_USE_BACKGROUNDS_NORM_FACTORS = true;
bool HISTFACTORY_USE_PRIORS                   = true;
bool HISTFACTORY_USE_SHAPE_VARIATION          = true;

//=============================================================================================================== 
void s0_generate_input_histograms(){
  TH1::AddDirectory(kFALSE);
  TFile * out_file = new TFile(INPUT_FILE_NAME, "RECREATE");

  // signal mc template
  TF1 * f_signal = new TF1("signal","x*gaus(0)", RANGE_MIN, RANGE_MAX);
  f_signal->SetParameters(1., 50, 5);
  TH1D * s_template = new TH1D("signal", "signal", NBINS, RANGE_MIN, RANGE_MAX);
  s_template->FillRandom("signal", NBINS * 100);
  s_template->Write();

  TH1D * s_template_alt = new TH1D("signal_alt", "signal_alt", NBINS, RANGE_MIN, RANGE_MAX);
  f_signal->SetParameters(1., 75, 4);
  s_template_alt->FillRandom("signal", NBINS * 100);
  s_template_alt->Write();

  // background mc templates
  vector <TH1D *> backgrounds;
  for(int i = 0; i < N_BACKGROUNDS; i++){
    TF1 * f_background = new TF1(("background_" + to_string(i)).c_str(), "([0] + [3]*x) / ([1] + x)^[2]", RANGE_MIN, RANGE_MAX);
    f_background->SetParameters(5.+gRandom->Uniform(0, 5), 10. + gRandom->Uniform(0, 10), 0.05 + gRandom->Uniform(0, 5.75), gRandom->Uniform(-0.5, 2.5));

    TH1D * b_template = new TH1D(f_background->GetName(), f_background->GetName(), NBINS, RANGE_MIN, RANGE_MAX);
    b_template->FillRandom(f_background->GetName(), NBINS * 100);
    b_template->Write();
    backgrounds.push_back( b_template );
  }

  // data as a sum of templates with parameter
  TH1D * data = new TH1D("data", "data", NBINS, RANGE_MIN, RANGE_MAX);
  data->Sumw2(true);
  data->Add(s_template    , SIGNAL_P * SIGNAL_P / sqrt(pow(SIGNAL_P_alt,2) + pow(SIGNAL_P,2)) );
  data->Add(s_template_alt, SIGNAL_P_alt * SIGNAL_P_alt / sqrt(pow(SIGNAL_P_alt,2) + pow(SIGNAL_P,2)) );
  for(auto b_template : backgrounds) data->Add(b_template);
  data->Write();

  // add variation hists for background templates
  for(int i = 0; i < N_BACKGROUNDS; i++){
    auto temp = backgrounds.at(i);
    TString name_up = temp->GetName() + TString("_up");
    TString name_down = temp->GetName() + TString("_down");
    TH1D * template_up = new TH1D( name_up, name_up, NBINS, RANGE_MIN, RANGE_MAX);
    TH1D * template_down = new TH1D( name_down, name_down, NBINS, RANGE_MIN, RANGE_MAX);

    template_up->FillRandom(("background_" + to_string(i)).c_str(), NBINS * 110);
    template_down->FillRandom(("background_" + to_string(i)).c_str(), NBINS * 90);

    template_up->Write();
    template_down->Write();
  }

  out_file->Close();
  cout << "s0_generate_input_histograms() ... you can find generated input templates in " << INPUT_FILE_NAME << endl;
}

//=============================================================================================================== 
RooWorkspace * s1_define_histfactory_model(){
  // create histsfactory model
  cout << "create histsfactory model ... " << endl;
  RooStats::HistFactory::Measurement model("myModel", "myModel");

  model.SetLumi(1.0);
  model.AddConstantParam("Lumi"); // turn off default histfactory lumi parameter

  // define data chanal
  RooStats::HistFactory::Channel chanal("myData");
  chanal.SetData("data", INPUT_FILE_NAME, "");

  // define model parameters and add signals and background templates
  RooStats::HistFactory::NormFactor nf_w1; // <- this is a POI
  nf_w1.SetName( "W1" );

  RooStats::HistFactory::NormFactor nf_w2; // <- this is a POI
  nf_w2.SetName( "W2" );

  model.AddPreprocessFunction("W1", "SIGNAL_P * SIGNAL_P / sqrt(SIGNAL_P * SIGNAL_P + SIGNAL_P_alt * SIGNAL_P_alt)", "SIGNAL_P[1., 0., 2], SIGNAL_P_alt[0., 0., 2]");
  model.AddPreprocessFunction("W2", "SIGNAL_P_alt * SIGNAL_P_alt / sqrt(SIGNAL_P * SIGNAL_P + SIGNAL_P_alt * SIGNAL_P_alt)", "SIGNAL_P[1., 0., 2], SIGNAL_P_alt[0., 0., 2]");

  RooStats::HistFactory::Sample signal_sample("mySignal", "signal", INPUT_FILE_NAME, "");
  signal_sample.AddNormFactor( nf_w1 );

  RooStats::HistFactory::Sample signal_sample_alt("mySignalAlt", "signal_alt", INPUT_FILE_NAME, "");
  signal_sample_alt.AddNormFactor( nf_w2 );

  if(HISTFACTORY_USE_STAT_ERRORS){
    signal_sample.ActivateStatError();
    signal_sample_alt.ActivateStatError();
    chanal.SetStatErrorConfig( STAT_ERROR_THRESHOLD, "Poisson");
  }

  chanal.AddSample( signal_sample );
  chanal.AddSample( signal_sample_alt );

  // backgrounds
  for(int i = 0; i < N_BACKGROUNDS; i++){
    string name = "background_" + to_string(i);
    RooStats::HistFactory::Sample background_sample(name, name, INPUT_FILE_NAME, "");
  
    if(HISTFACTORY_USE_BACKGROUNDS_NORM_FACTORS){
      RooStats::HistFactory::NormFactor nf_b;
      nf_b.SetName( "nf_"+name );
      nf_b.SetVal  ( 1. );
      nf_b.SetLow  ( 0.0 );
      nf_b.SetHigh ( 2.0 );
      background_sample.AddNormFactor( nf_b );
    }

    if(HISTFACTORY_USE_SHAPE_VARIATION){
      background_sample.AddHistoSys ("sys", name + "_up", INPUT_FILE_NAME, "", name + "_down", INPUT_FILE_NAME, "" ) ;
    }

    if(HISTFACTORY_USE_STAT_ERRORS) background_sample.ActivateStatError();
    chanal.AddSample( background_sample );
  }

  model.AddChannel(chanal);

  // build histsfactory model
  cout << "build histsfactory model ... " << endl;
  model.CollectHistograms();
  model.PrintTree();
  RooStats::HistFactory::HistoToWorkspaceFactoryFast factory;
  RooWorkspace * workspace = factory.MakeCombinedModel( model );

    // setup bayesian analyses
  cout << "setup bayesian analyses ... " << endl;

  // define list of pois
  RooArgList pois_list("pois_list");
  pois_list.add(*workspace->factory( "SIGNAL_P" ) );
  pois_list.add(*workspace->factory( "SIGNAL_P_alt" ) );
  pois_list.add(*workspace->factory( "alpha_sys" ) );

  if(HISTFACTORY_USE_BACKGROUNDS_NORM_FACTORS){
    for(int i = 0; i < N_BACKGROUNDS; i++){
      string name = "nf_background_" + to_string(i);
      pois_list.add( *workspace->factory( name.c_str() ) );
    }
  }

  if(HISTFACTORY_USE_STAT_ERRORS){
    for(int bin = 0; bin < NBINS; bin++){
      pois_list.add( *workspace->factory( ("gamma_stat_myData_bin_" + to_string(bin)).c_str() ) );
    }
  }

  // define list of nuisances
  RooArgList nuis_list("nuis_list"); // there is no difference between pois and nuis in pure bayesian analyses

  // define list of priors
  RooArgList priors_list("priors_list"); // flat priors at the moment, so, empty

  if(HISTFACTORY_USE_BACKGROUNDS_NORM_FACTORS and HISTFACTORY_USE_PRIORS){
    for(int i = 0; i < N_BACKGROUNDS; i++){
      string name = "nf_background_" + to_string(i);
      RooAbsPdf * prior = (RooAbsPdf *)workspace->factory( ("Gaussian::prior_" + name + "(" + name + ", 1.0, 0.1)").c_str() );
      priors_list.add( *prior );
    }
  }

  // make total prior as production of prior list members
  RooProdPdf prior_pdf("prior_pdf", "prior_pdf", priors_list);

  // define list of priors
  RooStats::ModelConfig* mconf = (RooStats::ModelConfig*)workspace->obj("ModelConfig"); // get histfactory modelconfig
  mconf->SetParametersOfInterest( pois_list );
  mconf->SetNuisanceParameters( nuis_list );
  mconf->SetPriorPdf( prior_pdf );

  return workspace;
}

//=============================================================================================================== 
void s2_run_bayesian_analyse(RooWorkspace * workspace){
  // setup bayesian analyses
  cout << "setup bayesian analyses ... " << endl;

  RooStats::ModelConfig* mconf = (RooStats::ModelConfig*)workspace->obj("ModelConfig"); // get histfactory modelconfig
  mconf->Print();

  RooStats::SequentialProposal proposal(10.5);
  // setup MCMC calculator
  RooStats::MCMCCalculator mc(*workspace->data("obsData"), *mconf);
  mc.SetNumBurnInSteps( 0 ); // better to apply custom cut in future
  mc.SetNumIters(MCMC_LENGTH);
  mc.SetProposalFunction( proposal );

  mc.SetConfidenceLevel(0.5);      // \*
  mc.SetLeftSideTailFraction(0.5); // we dont use this options but should set them anyway because RooStat is mad

  mc.Print();
    
  // produce chains
  cout << "produce chains ... " << endl;
  RooStats::MCMCInterval * mcInt = mc.GetInterval();
  const RooStats::MarkovChain* chain = mcInt->GetChain();
  
  // write chain as a ROOT tree
  TFile * out_file = new TFile(CHAIN_FILE_NAME, "RECREATE");
  TTree * chain_tree = new TTree("chain_tree", "chain_tree");
  vector<double*> vals;
  
  int chain_size = chain->Size();
  const RooArgSet * argset = chain->Get( 0 );

  auto iter = argset->createIterator();
  auto var = iter->Next();
  while( var ){
    cout << "add branch " << var->GetName() << endl;
    vals.push_back(new double);
    chain_tree->Branch( var->GetName(), vals.at(vals.size() - 1) );
    var = iter->Next();
  }

  vals.push_back(new double);
  chain_tree->Branch("weight", vals.at(vals.size() - 1) );

  for(int i = 0; i < chain_size; i++){
    const RooArgSet * argset = chain->Get( i );
    auto niter = argset->createIterator();
    auto nvar = niter->Next();
    int index = 0;
    while( nvar ){
      *vals.at(index) = argset->getRealValue( nvar->GetName() );
      index++;
      nvar = niter->Next();
    }
    *vals.at(vals.size() - 1) = chain->Weight(i);
    chain_tree->Fill();
  }

  chain_tree->Write();
  out_file->Close();

  cout << "s2_run_bayesian_analyse() ... you can find output chain in " << CHAIN_FILE_NAME << endl;
}

//=============================================================================================================== 
double get_qv(TH1D* histe, double qfrac){
  int Nbins = histe->GetXaxis()->GetNbins();
  double tot = histe->Integral();
  double sum = 0.0;
  int upe=1;
  for (int i=1; i <=Nbins; ++i){
    sum += histe->GetBinContent(i);
    if(sum < qfrac * tot) upe = i;
    else break;
  }
  return histe->GetBinCenter(upe);
}

void s3_find_limits(const char * input_file){
  TFile * inp_file = new TFile(input_file, "OPEN");
  TTree * chain_tree = (TTree*) inp_file->Get( "chain_tree" );

  double weight;
  auto bweight = chain_tree->GetBranch("weight");
  bweight->SetAddress(&weight);

  auto list = chain_tree->GetListOfBranches();
  //list->Print();
  TIter next( list );
  TBranch *branch;
  
  double CL = 0.95;
  cout << "s3_find_limits() ... parameter upper limit list, CL = " << CL << endl;
  while((branch = (TBranch *)next())){

    if( string(branch->GetName()) == "weight" ) continue;
    if( string(branch->GetName()) == "nll_MarkovChain_local_" ) continue;

    double var;
    branch->SetAddress(&var);

    auto nevent = chain_tree->GetEntries();
    TH1D * hist = new TH1D(branch->GetName(), branch->GetName(), 100000, -10, 10);
    for(Int_t i=0; i<nevent; i++){
      chain_tree->GetEvent(i);
      hist->Fill(var, weight);
    }

    cout << branch->GetName() << " " << get_qv(hist, CL) << endl;
  }
  
}

//=============================================================================================================== 
void s4_produce_toys(RooWorkspace * workspace){
  RooStats::ModelConfig* mconf = (RooStats::ModelConfig*)workspace->obj("ModelConfig"); // get histfactory modelconfig
  mconf->Print();

  // define PDFs and list of variables to generate in prior
  RooAbsPdf *model_pdf = mconf->GetPdf();
  model_pdf->Print();

  RooArgList priors_list("priors_list");
  RooArgList prior_par_list("pois_list");

  if(HISTFACTORY_USE_SHAPE_VARIATION){
    priors_list.add( *workspace->factory("alpha_sysConstraint") );
    prior_par_list.add( *workspace->factory("alpha_sys") );
  }

  if(HISTFACTORY_USE_STAT_ERRORS){
    for(int i = 0; i < NBINS; i++){
      priors_list.add( *workspace->factory( ("gamma_stat_myData_bin_" + to_string(i) + "_constraint").c_str() ) );
      prior_par_list.add( *workspace->factory( ("gamma_stat_myData_bin_" + to_string(i)).c_str() ) );
    }
  }

  if(HISTFACTORY_USE_BACKGROUNDS_NORM_FACTORS){
    priors_list.add( *mconf->GetPriorPdf() );
    for(int i = 0; i < N_BACKGROUNDS; i++){
      string name = "nf_background_" + to_string(i);
      prior_par_list.add( *workspace->factory( name.c_str() ) );
    }
  }

  RooProdPdf tot_pdf("tot_prior_pdf", "tot_prior_pdf", priors_list);
  tot_pdf.Print();
 
  RooDataSet * prior_pdf_events = tot_pdf.generate( prior_par_list, N_TOYS );
  prior_pdf_events->Print();

  TFile * out_file = new TFile(TOYS_FILE_NAME, "RECREATE");

  map<string, TH1D*> vars_hist;
  const RooArgSet * argset = prior_pdf_events->get(0);
  auto iter = argset->createIterator();
  auto var = iter->Next();
  while( var ){
    auto roovar = (RooRealVar*)workspace->factory( var->GetName() );
    vars_hist[ var->GetName() ] = new TH1D(var->GetName(), var->GetName(), 100, roovar->getMin(), roovar->getMax());
    var = iter->Next();
  }

  for(int i = 0; i < N_TOYS; i++){
    // read variables from generated prior set
    const RooArgSet * argset = prior_pdf_events->get(i);

    auto iter = argset->createIterator();
    auto var = iter->Next();
    while( var ){
      ((RooRealVar*)workspace->factory( var->GetName() ))->setVal( argset->getRealValue(var->GetName()) );

      auto item = vars_hist.find( var->GetName() );
      item->second->Fill( argset->getRealValue(var->GetName()) );

      var = iter->Next();
    }

    // generate histograms from model pdf
    RooDataHist * model_pdf_event = model_pdf->generateBinned( *workspace->factory("obs_x_myData") );

    TH1D * toy_hist = new TH1D( ("toy_" + to_string(i)).c_str(), ("toy_" + to_string(i)).c_str(), NBINS, RANGE_MIN, RANGE_MAX);
    for(int i = 0; i < NBINS; i++){
      model_pdf_event->get(i);
      toy_hist->SetBinContent(i+1, model_pdf_event->weight());
    }
    toy_hist->Write();
  }

  for(auto iter : vars_hist) iter.second->Write();
  out_file->Close();
}

//=============================================================================================================== 
void e00_histfactory_bayesian_mcmc(){
  // generate input data and mc histograms
  s0_generate_input_histograms();

  // setup statistical model with histfactory
  RooWorkspace * workspace = s1_define_histfactory_model();

  // run bayesian analyses
  s2_run_bayesian_analyse( workspace );

  // find limits
  s3_find_limits(CHAIN_FILE_NAME);

  // produce toys from model
  s4_produce_toys( workspace );
}










