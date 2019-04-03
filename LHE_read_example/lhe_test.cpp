
#include "LHEF.h"

void lhe_test() {

  std::ifstream lhe_file ("unweighted_events.lhe");

  LHEF::Reader * reader = new LHEF::Reader( lhe_file );

  TFile * ofile = new TFile("out_file.root", "RECREATE");
  TTree * tree_cg = new TTree("ttree_cg", "ttree_cg");
  TTree * tree_ug = new TTree("ttree_ug", "ttree_ug");


  TLorentzVector Higgs, cquark;
  tree_cg->Branch("Higgs",  &Higgs);
  tree_cg->Branch("cquark", &cquark);
  tree_ug->Branch("Higgs", &Higgs);
  tree_ug->Branch("cquark", &cquark);

  while( reader->readEvent() ){
    
    // run information
    reader->heprup;

    // event information
    reader->hepeup;

    Higgs.SetPxPyPzE( 0, 0, 0, 0 );
    cquark.SetPxPyPzE( 0, 0, 0, 0 );

    string channel = "";
    for(int i = 0, NUP = reader->hepeup.NUP; i < NUP; i++){
      int IDUP = reader->hepeup.IDUP.at(i);
      int ISTUP = reader->hepeup.ISTUP.at(i);
      std::pair<int,int> MOTHUP = reader->hepeup.MOTHUP.at(i);
      std::vector<double> PUP   = reader->hepeup.PUP.at(i);  // (Px, Py, Pz, E and M in GeV)

      if( IDUP == 25 and MOTHUP.first == 1 ) 
        Higgs.SetPxPyPzE( PUP[0], PUP[1], PUP[2], PUP[3] );

      if( abs(IDUP) == 4 and MOTHUP.first == 1 ) 
        cquark.SetPxPyPzE( PUP[0], PUP[1], PUP[2], PUP[3] );

      if( abs(IDUP) == 2 and MOTHUP.first == 0 ) channel = "ug->Hc";
      if( abs(IDUP) == 4 and MOTHUP.first == 0 ) channel = "cg->Hc";
    }

    if(channel == "ug->Hc") tree_ug->Fill();
    if(channel == "cg->Hc") tree_cg->Fill();
  }

  ofile->Write();
  ofile->Close();
}











