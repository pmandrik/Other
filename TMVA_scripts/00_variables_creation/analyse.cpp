
#include "tqgamma_flat_ntuple_data.hh"

/*
  1. Pt фотона
  2. b-tag дискриминатор? o_O
  3. Pt b-jet
  4. Pt muon (tcy)
  5. cos(p_t, p_y)
  6. dR(b-jet, y)
  7. dR(muon, y)
  8. charge of lepton (tuy)
  9. jet multiplicity

*/

float get_dR(float eta_a, float phi_a, float eta_b, float phi_b){
  return sqrt( pow(eta_a - eta_b, 2) + pow(phi_a - phi_b, 2) );
}

int analyse(){

  string chanal_name = "wjets";

  // SETUP OUTPUT TTREE
  TFile * file_out = new TFile( (chanal_name + "_output.root").c_str(), "RECREATE");
  TTree * tree_out = new TTree("ttree", "ttree");
  float G_PT, BJ_BTAG, BJ_PT, MU_PT, COS_T_G, dR_BJ_G, dR_Mu_G, MU_CHARGE, J_MULT;
  tree_out->Branch("G_PT", &G_PT);
  tree_out->Branch("BJ_BTAG", &BJ_BTAG);
  tree_out->Branch("BJ_PT", &BJ_PT);
  tree_out->Branch("MU_PT", &MU_PT);
  tree_out->Branch("COS_T_G", &COS_T_G);
  tree_out->Branch("dR_BJ_G", &dR_BJ_G);
  tree_out->Branch("dR_Mu_G", &dR_Mu_G);
  tree_out->Branch("MU_CHARGE", &MU_CHARGE);
  tree_out->Branch("J_MULT", &J_MULT);

  // INPUT TTREE

  TFile * file = TFile::Open( (chanal_name + ".root").c_str() );
  file->cd("ntuple");

  tqgamma_Event event;
  event.fChain_MuonLoose          = (TTree*) gDirectory->Get("MuonLoose");
  event.fChain_ElectronTight      = (TTree*) gDirectory->Get("ElectronTight");
  event.fChain_ElectronLoose      = (TTree*) gDirectory->Get("ElectronLoose");
  event.fChain_Particle           = (TTree*) gDirectory->Get("Particle");
  event.fChain_Photons            = (TTree*) gDirectory->Get("Photons");
  event.fChain_Vertex             = (TTree*) gDirectory->Get("Vertex");
  event.fChain_GenJet             = (TTree*) gDirectory->Get("GenJet");
  event.fChain_MuonTight          = (TTree*) gDirectory->Get("MuonTight");
  event.fChain_Selection          = (TTree*) gDirectory->Get("Selection");
  event.fChain_JetPUPPI           = (TTree*) gDirectory->Get("JetPUPPI");
  event.fChain_PuppiMissingET     = (TTree*) gDirectory->Get("PuppiMissingET");
  event.fChain_Event              = (TTree*) gDirectory->Get("Event");
  

  event.Init();
  Long64_t max_events = 1000;

  for(Long64_t nevent = 0; nevent < max_events; nevent++){
    event.GetEntry( nevent );

    //   float G_PT, BJ_BTAG, BJ_PT, MU_PT, COS_T_G, dR_BJ_G, dR_Mu_G, MU_CHARGE, J_MULT;
    //   1. Pt фотона
    if(event.Photons_size){
      G_PT = event.Photons_PT[0];
    } else G_PT = -1;

    // 2. b-tag дискриминатор? o_O
    if(event.JetPUPPI_size){
      BJ_BTAG = event.JetPUPPI_MVAv2[0];
    } else BJ_BTAG = -1;

    // 3. Pt b-jet
    if(event.JetPUPPI_size){
      BJ_PT = event.JetPUPPI_PT[0];
    } else BJ_PT = -1;

    //  4. Pt muon (tcy)
    if(event.MuonTight_size){
      MU_PT = event.MuonTight_PT[0];
    } else MU_PT = -1;

    //  5. cos(p_t, p_y)
    COS_T_G = -1;

    //   6. dR(b-jet, y)
    if(event.JetPUPPI_size and event.Photons_size){
      dR_BJ_G = get_dR(event.JetPUPPI_Eta[0], event.JetPUPPI_Phi[0], event.Photons_Eta[0], event.Photons_Phi[0]);
    } else dR_BJ_G = -1;

    //   7. dR(muon, y)
    if(event.MuonTight_size and event.Photons_size){
      dR_Mu_G = get_dR(event.MuonTight_Eta[0], event.MuonTight_Phi[0], event.Photons_Eta[0], event.Photons_Phi[0]);
    } else dR_Mu_G = -1;

    //   8. charge of lepton (tuy)
    if(event.MuonTight_size){
      MU_CHARGE = event.MuonTight_Charge[0];
    } else MU_CHARGE = -10;

    //   9. jet multiplicity
    J_MULT = event.JetPUPPI_size;

    tree_out->Fill();
  }

  file_out->cd();
  tree_out->Write();

  return 0;
}
