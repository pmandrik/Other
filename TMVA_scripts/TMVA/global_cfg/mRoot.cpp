
#ifndef mRoot_hh
#define mRoot_hh 1

// MROOT
#include "msg.hh"

namespace mRoot{

  //=================================== LAMBDAS LIST MANIPULATION =========================================================================================================
  template<typename R, typename F, typename T> vector<R> list_compr(F lambda, vector<T> inp){ 
    vector<R> answer;
    for(auto it : inp) answer.push_back( lambda( it ) );
    return answer;
  }

  template<typename R, typename F, typename T, typename P> vector<R> list_compr(F lambda, vector<T> inp_a, vector<P> inp_b){ 
    vector<R> answer;
    for(int i = 0; i < inp_a.size(); i++)
      answer.push_back( lambda( inp_a.at(i), inp_b.at(i) ) );
    return answer;
  }

  template<typename R, typename F> R test(F lambda){
    return R();
  }

  template <typename T> vector <T> vector_sum(vector<T> v1, vector<T> v2){
    v1.insert(v1.begin(), v2.begin(), v2.end());
    return v1;
  }

  template <typename T, typename... Args> vector <T> vector_sum(vector<T> v1, vector<T> v2, Args... args){
    v1.insert(v1.begin(), v2.begin(), v2.end());
    return vector_sum(v1, args...);
  }

  //=================================== ROOT BASIC MANIPULATION =========================================================================================================
  std::string get_date(){
    std::time_t result = std::time(nullptr);
    std::string name = std::string(std::asctime(std::localtime(&result)));
    name.pop_back();
    return name;
  }

  static int name_id_counter = 0;
  std::string get_name(){
    return "#" + std::to_string(name_id_counter++) + " " + get_date();
  }

  TCanvas * get_canvas (int width=640, int height=480){
    std::string name = get_name();
    return new TCanvas(name.c_str(), name.c_str(), width, height);
  }

  string get_name_without_extension_and_path(string name){
    TString basename = gSystem->BaseName( name.c_str() );
    return basename.ReplaceAll(".root", "").Data();
  }

  //=================================== HISTOGRAMS MANIPULATION =========================================================================================================

  void tune_hist(TH1D* hist){
    // set axis range
    double left_entry  = hist->GetBinCenter(hist->FindFirstBinAbove(0));
    double right_entry = hist->GetBinCenter(hist->FindLastBinAbove(0));

    int nbins =     hist->GetXaxis()->GetNbins();
    double bwidth = 0.5*(right_entry - left_entry) / pow(hist->GetEntries(), 1./3.);
    int nbins_new = (hist->GetXaxis()->GetXmax() - hist->GetXaxis()->GetXmin()) / bwidth;

    for( ;nbins % nbins_new; nbins_new--){}

    hist->Rebin(nbins/nbins_new);
    hist->GetXaxis()->SetRangeUser(left_entry, right_entry);
  }

  TCanvas * plotCorrelationHist(TH2D* hist, bool round = true){
    TCanvas * canv = get_canvas(640, 640);
    canv->SetLeftMargin(0.17);
    canv->SetRightMargin(0.17);
    canv->SetTopMargin(0.17);
    canv->SetBottomMargin(0.17);

    hist->Draw("COLZ");
    hist->SetStats(0);
    hist->GetXaxis()->LabelsOption("v");

    TH2D* hist_copy = new TH2D(*hist);
    if(round)
      for(int x = 1; x <= hist->GetXaxis()->GetNbins(); x++)
        for(int y = 1; y <= hist->GetYaxis()->GetNbins(); y++)
          hist_copy->SetBinContent( x, y, int(hist_copy->GetBinContent(x, y)) );
    hist_copy->Draw("TEXT SAME");
    hist_copy->SetMarkerColor(kRed);
    hist_copy->SetMarkerSize(1.2);
    canv->Update();
    return canv;
  }

  TH1D *  copy_tree_without_bins(TH1D * hist, vector<int> bins){
    int new_nbins = hist->GetXaxis()->GetNbins() - bins.size();
    string new_name = hist->GetName();
    string new_title = hist->GetTitle() + string("@WithoutBins");
    
    for(int i = 0; i < bins.size(); i++){
      // new_name+= "@" + to_string(i);
      new_title += "@" + to_string( bins.at(i) );
    }

    double xmin = hist->GetXaxis()->GetXmin();
    double xmax = hist->GetXaxis()->GetXmax() - bins.size()*hist->GetBinWidth(1);

    TH1D * new_hist = new TH1D(new_name.c_str(), new_title.c_str(), new_nbins, xmin, xmax);

    for(int i = 1, j = 1; i <= hist->GetXaxis()->GetNbins(); i++){
      auto it = std::find_if(bins.begin(), bins.end(),  [i](const int & bin) { return i == bin; });
      if(it != bins.end()) continue;  
      new_hist->SetBinContent(j, hist->GetBinContent(i));
      new_hist->SetBinError(j, hist->GetBinError(i));
      j++;
    }
    return new_hist;
  }

  TH1D * get_residial(TH1D * hist_a, TH1D * hist_b, bool fraction = true){
    TH1D * rhist = new TH1D(*hist_a);
    rhist->Sumw2();
    rhist->Add(hist_b, -1.);
    if(fraction) rhist->Divide(hist_a);
    return rhist;
  }

  //=================================== TSYSTEM MANIPULATION =========================================================================================================
  vector<string> get_files(string dirname="", string ext=".root"){
    vector<string> answer;
    TSystemDirectory dir(dirname.c_str(), dirname.c_str());
    TList *files = dir.GetListOfFiles();
    if (files) {
      TSystemFile *file;
      TString fname;
      TIter next(files);
      while ((file=(TSystemFile*)next())) {
         fname = file->GetName();
         if(not file->IsDirectory() and fname.EndsWith(ext.c_str()))
            answer.push_back( fname.Data() );
      }
    }
    return answer;
  }

  vector<string> get_directories(string dirname=".", string ext=""){ // FIXME
    vector<string> answer;
    TSystemDirectory dir(dirname.c_str(), dirname.c_str());
    TList *files = dir.GetListOfFiles();
    if (files) {
      TSystemFile *file;
      TString fname;
      TIter next(files);
      while ((file=(TSystemFile*)next())) {
        fname = file->GetName();
        if(fname==".") continue;
        if(fname=="..") continue;
        if(not file->IsDirectory()) continue;
        answer.push_back( fname.Data() );
      }
    }
    return answer;
  }

  //=================================== TFILE DIRECTOY MANIPULATION =========================================================================================================
  TObject * find_in_dir(TDirectory *dir, string name) {
    TIter next (dir->GetListOfKeys());
    TKey *key;
    while ((key = (TKey*)next())) {
      if(key->GetName() == name) {
        return key->ReadObj();
      }
      if(strcmp(key->GetClassName(),"TDirectory")) {
        dir->cd(key->GetName());
        TDirectory *subdir = gDirectory;
        TObject * answer = find_in_dir(subdir, name);
        if(answer != nullptr) return answer;
        dir->cd();
      }
    }
    return nullptr;
  }

  vector <TObject*> * get_all_from_folder(TDirectory * dir, string include_rexp=".+", string exclude_rexp=""){
    vector <TObject*> * objs = new vector <TObject*>;

    regex reg_incl( include_rexp );
    regex reg_excl( exclude_rexp );

    auto keys = dir->GetListOfKeys();

    for(int i = 0; i < keys->GetSize(); i++){
      auto obj =  ((TKey*) keys->At(i))->ReadObj();
      string name = obj->GetName();

      if( !regex_match(name, reg_incl) ) continue;
      if( regex_match(name, reg_excl) ) continue;

      objs->push_back( obj );
    }

    return objs;
  }

  //=================================== TTREE MANIPULATION =========================================================================================================
  void slim_tree(string inp_name, string out_name, string tree_name, long long int n_events){
    TFile * file = TFile::Open( inp_name.c_str() );
    if(!file or file->IsZombie()){
      cerr << __FUNCTION__ << ": can't open file name \"" << inp_name << "\", skip ... " << endl;
      return;
    }

    TTree * oldtree = (TTree*) find_in_dir(file, tree_name);

    TFile *newfile = new TFile(out_name.c_str(), "RECREATE");
    TTree *newtree = oldtree->CloneTree( n_events );

    newtree->Print();
    newfile->Write();
  }

  //=================================== STRING MANIPULATION =========================================================================================================
  static void ltrim(std::string &s) {
    if(not s.size()) return;
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
  }

  static void rtrim(std::string &s) {
    if(not s.size()) return;
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
  }

  std::string strip(std::string s) {
    if(not s.size()) return s;
    ltrim(s);
    rtrim(s);
    return s;
  }

  vector<string> split_string(string str, const string & sep = " "){
    int sep_size = sep.size();
    vector<string> answer;

    for(int i = 0; i + sep_size < str.size(); i++){
      if( sep == str.substr(i, sep_size) ){
        answer.push_back( str.substr(0, i) ); 
        str = str.substr(i + sep_size, str.size() - i - sep_size);
        i = 0;
      }
    }

    answer.push_back(str);
    return answer;
  }

  vector<string> split_and_strip_string(string str, const string & sep = " "){
    vector<string> answer = split_string(str, sep);
    for(int i = 0; i < answer.size(); i++)
      answer[i] = strip( answer.at(i) );
    return answer;
  }

  //=================================== EXTRA GRAPHIC PLOT MANIPULATION =========================================================================================================
  class FillGraph{
    public:
    TGraph *GetGraph(){
      TGraph *graph   = new TGraph( 2 * xs.size() );
      for(int i = 0; i < xs.size(); i++){
        graph->SetPoint( i,               xs.at(i), ys_up.at(i) );
        graph->SetPoint( 2*xs.size()-1-i, xs.at(i), ys_down.at(i) );
      }
      return graph;
    }

    void AddPoint(double x, double y1, double y2){
      xs.push_back(x);
      ys_up.push_back(y1);
      ys_down.push_back(y2);
    }
    
    vector<double> xs, ys_up, ys_down;
  };
};

#include "mRootTreeToHist.cpp"

#endif





