

#ifndef mRootTreeToHist_hh
#define mRootTreeToHist_hh 1

namespace mRoot{

  class EventsExcluder {
    public:
    EventsExcluder(string fname){
      original_fname = fname;
      exclude_events = parce_train_events( fname );
    }

    void Print(){
      cout << "EventsExcluder( " << original_fname << " )"<< endl;
      for(auto iter : *exclude_events){
        cout << " " << iter.first << " " << iter.second->size() << endl;
      }
    }

    void SetExcludedEventsFile(string fname){
      for(auto iter : *exclude_events)
        if(iter.first == fname){ 
          active_events = iter.second;
          active_event_number = active_events->at(0);
          active_event_counter = 0;
          return;
        }
      active_events = nullptr;
    }

    bool IsExcluded(int event_number){
      if(active_events == nullptr) return false;
      if(event_number != active_event_number) return false;
        
      active_event_counter++;
      if(active_event_counter < active_events->size())
        active_event_number  = active_events->at( active_event_counter );
      else active_events = nullptr;
      return true;
    }

    vector<pair<string, vector<int>*>> *parce_train_events(string fname = "bnn_sm7_trainEvents.txt"){
      vector<pair<string, vector<int>*>> * answer = new vector<pair<string, vector<int>*>>;
      vector<int> * current_events;
      string current_name = "";

      regex name_pattern( ".+\.root" );

      regex sep(" ", regex_constants::egrep);
      smatch sm;

      cout << "parce_train_events() ... " << endl;
      int i = 0;
      std::ifstream infile(fname.c_str());
      for( std::string line; std::getline( infile, line ); ){
        if( regex_match(line, name_pattern) ){ // find new name line
          if(current_name != "") answer->push_back( make_pair(current_name, current_events) );
          current_events = new vector<int>();
          current_name = line;
          cout << "add " << current_name << endl;
          continue;
        }

        auto str = split_string(line, " ");
        if(str.size() > 8){
          for(auto s : str) current_events->push_back( atoi(s.c_str()) );
        }
      }
      answer->push_back( make_pair(current_name, current_events) );

      cout << "parce_train_events() ... done" << endl;
      return answer;
    }

    string original_fname;
    vector<int>* active_events;
    int active_event_number;
    int active_event_counter;
    vector<pair<string, vector<int>*>> * exclude_events;
  };


  // multiple files with same ttree and value_rule -> single hist -> save in file
  // fill_hist(hist_name, nbins, rmax, rmin, output_file, prefix, input_file_names, tree_name, value_rule, weight_rule, event_excluder){
  void fill_hist(string hist_name, // input TH1D name
    int nbins,                 // number of bins
    double rmin,               // x-axis range min
    double rmax,               // x-axis range max
    TFile * output_file,       // output file
    string prefix,             // path to input files
    vector<string> input_file_names, // vector of names of input files
    string tree_name,                // name of tree
    string value_rule,               // formula of value to evaluate
    string weight_rule,              // formula of weight to evaluate
    EventsExcluder * event_excluder = nullptr  // contain list of events per file
  ){
    cout << " fill_hist(): process " << hist_name << " with value rule = " << "\"" << value_rule << "\"" << ", weight rule = " << "\"" << weight_rule << "\"" << endl;
    // open file
    output_file->cd();
    TH1D * hist = new TH1D(hist_name.c_str(), hist_name.c_str(), nbins, rmin, rmax);
    hist->Sumw2();
    int prev_integral = 0;

    double totl_weight = 0;
    double excl_weight = 0;
    double weight = 0;
    int event_index = 0;
    int print_n_entries = 20;

    for(auto name : input_file_names){
      TFile * file = TFile::Open( (prefix + name).c_str() );
      if(!file or file->IsZombie()){
        cerr << __FUNCTION__ << ": can't open file name \"" << name << "\", skip ... " << endl;
        continue;
      }

      TTreeReader * reader = new TTreeReader(tree_name.c_str(), file);
      if(!reader->GetTree()){
        cerr << __FUNCTION__ << ": can't get ttree \"" << tree_name << "\" in file \"" << file->GetName() << "\", skip ... " << endl;
        continue;
      }

      // reader->GetTree()->Print();

      TTreeFormula value_f(value_rule.c_str(), value_rule.c_str(), reader->GetTree());
      TTreeFormula weight_f(weight_rule.c_str(), weight_rule.c_str(), reader->GetTree());

      if( value_f.GetNdim()==0 or weight_f.GetNdim()==0){
        reader->GetTree()->Print();
        return;
      }

      if(event_excluder != nullptr) event_excluder->SetExcludedEventsFile( name );
      event_index = -1;

      while(reader->Next()){
        event_index++;
        weight = weight_f.EvalInstance();
        totl_weight += weight;
        if(event_excluder != nullptr and event_excluder->IsExcluded( event_index )){
          excl_weight += weight;
          continue;
        }
        // if(weight < 0.) cout << weight << endl;
        hist->Fill(value_f.EvalInstance(), weight);
        if(print_n_entries > 0 and weight > 0){ 
          msg(value_f.EvalInstance(), weight);
          print_n_entries--;
        };
      }

      if((int)hist->Integral() == prev_integral){
        cerr << __FUNCTION__ << ": fill no events from file " << name << " to hist " << hist_name << ", continue ... " << endl;
        continue;
      }
      prev_integral += (int)hist->Integral();
    }

    // now we need to reweight hists because of excluded events
    if( totl_weight <= excl_weight){
      cerr << __FUNCTION__ << ": something wrong with events weights, can't rescale " << hist_name << " - " << totl_weight << ", " << excl_weight << ", continue ... " << endl;
    } else {
      // cout << __FUNCTION__ << ": reweight factor for " << hist_name << " = " << totl_weight / (totl_weight - excl_weight) << endl;
      hist->Scale( totl_weight / (totl_weight - excl_weight) );
    }
    
    // save, exit
    output_file->cd();
    hist->Write();
  }

  void fill_hist_sys(string hist_name, // input TH1D name
    int nbins,                 // number of bins
    double rmin,               // x-axis range min
    double rmax,               // x-axis range max
    TFile * output_file,       // output file
    string prefix,             // path to input files
    vector<string> input_file_names, // vector of names of input files
    string tree_name,                // name of tree
    string value_rule,               // formula of value to evaluate
    string weight_rule_up,                // formula of weight to evaluate
    string weight_rule_down,              // formula of weight to evaluate
    EventsExcluder * event_excluder = nullptr  // contain list of events per file
  ){
    // cout << weight_rule_up << " " << weight_rule_down << endl;
    fill_hist(hist_name+"Up",   nbins, rmin, rmax, output_file, prefix, input_file_names, tree_name, value_rule, weight_rule_up, event_excluder);
    fill_hist(hist_name+"Down", nbins, rmin, rmax, output_file, prefix, input_file_names, tree_name, value_rule, weight_rule_down, event_excluder);
  }







  // ===============
  TH1D* fill_hist(string hist_name, // input TH1D name
    int nbins,                 // number of bins
    double rmin,               // x-axis range min
    double rmax,               // x-axis range max
    string prefix,             // path to input files
    vector<string> input_file_names, // vector of names of input files
    string tree_name,                // name of tree
    string value_rule,               // formula of value to evaluate
    string weight_rule,              // formula of weight to evaluate
    EventsExcluder * event_excluder = nullptr  // contain list of events per file
  ){
    cout << " fill_hist(): process " << hist_name << " with value rule = " << "\"" << value_rule << "\"" << ", weight rule = " << "\"" << weight_rule << "\"" << endl;
    // open file
    TH1D * hist = new TH1D(hist_name.c_str(), hist_name.c_str(), nbins, rmin, rmax);
    hist->Sumw2();
    int prev_integral = 0;

    double totl_weight = 0;
    double excl_weight = 0;
    double weight = 0;
    int event_index = 0;
    int print_n_entries = 1;

    for(auto name : input_file_names){
      cout << "process " <<  name << endl;
      TFile * file = TFile::Open( (prefix + name).c_str() );
      if(!file or file->IsZombie()){
        cerr << __FUNCTION__ << ": can't open file name \"" << name << "\", skip ... " << endl;
        continue;
      }

      TTreeReader * reader = new TTreeReader(tree_name.c_str(), file);
      if(!reader->GetTree()){
        cerr << __FUNCTION__ << ": can't get ttree \"" << tree_name << "\" in file \"" << file->GetName() << "\", skip ... " << endl;
        continue;
      }

      // reader->GetTree()->Print();

      TTreeFormula value_f(value_rule.c_str(), value_rule.c_str(), reader->GetTree());
      TTreeFormula weight_f(weight_rule.c_str(), weight_rule.c_str(), reader->GetTree());

      if( value_f.GetNdim()==0 or weight_f.GetNdim()==0){
        reader->GetTree()->Print();
        continue;
      }

      if(event_excluder != nullptr) event_excluder->SetExcludedEventsFile( name );
      event_index = -1;

      while(reader->Next()){
        event_index++;
        weight = weight_f.EvalInstance();
        totl_weight += weight;
        if(event_excluder != nullptr and event_excluder->IsExcluded( event_index )){
          excl_weight += weight;
          continue;
        }
        // if(weight < 0.) cout << weight << endl;
        hist->Fill(value_f.EvalInstance(), weight);
        if(print_n_entries > 0 and weight > 0){ 
          msg(value_f.EvalInstance(), weight);
          print_n_entries--;
        };
      }

      if((int)hist->Integral() == prev_integral){
        cerr << __FUNCTION__ << ": fill no events from file " << name << " to hist " << hist_name << ", continue ... " << endl;
        continue;
      }
      prev_integral += (int)hist->Integral();
      file->Close();
    }

    // now we need to reweight hists because of excluded events
    if( totl_weight <= excl_weight){
      cerr << __FUNCTION__ << ": something wrong with events weights, can't rescale " << hist_name << " - " << totl_weight << ", " << excl_weight << ", continue ... " << endl;
    } else {
      // cout << __FUNCTION__ << ": reweight factor for " << hist_name << " = " << totl_weight / (totl_weight - excl_weight) << endl;
      hist->Scale( totl_weight / (totl_weight - excl_weight) );
    }
    
    // save, exit
    return hist;
  }
};

#endif










