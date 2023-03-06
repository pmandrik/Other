#include "/afs/cern.ch/user/p/pmandrik/public/global_cfg/mRoot.cpp"

void slimm_data(){
	vector<string> names = {"Data", "QCD_data", "DY", "ttbar", "WW", "WZ", "ZZ", "s-channel", "t-channel", "tW-channel", "FCNC_tug", "FCNC_tcg"};

	string path     = "/afs/cern.ch/work/g/gvorotni/public/samples/13TeV/17-10-31_PUPPI/tuples_merged/Central/";

	for(auto item : names){
		mRoot::slim_tree( path + item + ".root", item + ".root", "Vars", 10000 );
	}
}
