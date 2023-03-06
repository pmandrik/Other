
#ifndef tqgamma_Event_hh
#define tqgamma_Event_hh 1

class tqgamma_Event {
  public:
    tqgamma_Event(){
      fChain_MuonLoose = nullptr;
      fChain_ElectronTight = nullptr;
      fChain_ElectronLoose = nullptr;
      fChain_Particle = nullptr;
      fChain_Photons = nullptr;
      fChain_Vertex = nullptr;
      fChain_GenJet = nullptr;
      fChain_MuonTight = nullptr;
      fChain_Selection = nullptr;
      fChain_JetPUPPI = nullptr;
      fChain_PuppiMissingET = nullptr;
      fChain_Event = nullptr;

    }

    void Init(){
      if(fChain_MuonLoose == nullptr){
        cout << "tqgamma_Event.Init(): fChain_MuonLoose == nullptr, return" << endl;
        return;
      };
      if(fChain_ElectronTight == nullptr){
        cout << "tqgamma_Event.Init(): fChain_ElectronTight == nullptr, return" << endl;
        return;
      };
      if(fChain_ElectronLoose == nullptr){
        cout << "tqgamma_Event.Init(): fChain_ElectronLoose == nullptr, return" << endl;
        return;
      };
      if(fChain_Particle == nullptr){
        cout << "tqgamma_Event.Init(): fChain_Particle == nullptr, return" << endl;
        return;
      };
      if(fChain_Photons == nullptr){
        cout << "tqgamma_Event.Init(): fChain_Photons == nullptr, return" << endl;
        return;
      };
      if(fChain_Vertex == nullptr){
        cout << "tqgamma_Event.Init(): fChain_Vertex == nullptr, return" << endl;
        return;
      };
      if(fChain_GenJet == nullptr){
        cout << "tqgamma_Event.Init(): fChain_GenJet == nullptr, return" << endl;
        return;
      };
      if(fChain_MuonTight == nullptr){
        cout << "tqgamma_Event.Init(): fChain_MuonTight == nullptr, return" << endl;
        return;
      };
      if(fChain_Selection == nullptr){
        cout << "tqgamma_Event.Init(): fChain_Selection == nullptr, return" << endl;
        return;
      };
      if(fChain_JetPUPPI == nullptr){
        cout << "tqgamma_Event.Init(): fChain_JetPUPPI == nullptr, return" << endl;
        return;
      };
      if(fChain_PuppiMissingET == nullptr){
        cout << "tqgamma_Event.Init(): fChain_PuppiMissingET == nullptr, return" << endl;
        return;
      };
      if(fChain_Event == nullptr){
        cout << "tqgamma_Event.Init(): fChain_Event == nullptr, return" << endl;
        return;
      };


      // Set branch addresses
      fCurrent = -1;

      fChain_Event->SetBranchAddress("Run", &Run);
      fChain_Event->SetBranchAddress("Event", &Event);
      fChain_Event->SetBranchAddress("Lumi", &Lumi);
      fChain_Particle->SetBranchAddress("Particle_size", &Particle_size);
      fChain_Vertex->SetBranchAddress("Vertex_size", &Vertex_size);
      fChain_GenJet->SetBranchAddress("GenJet_size", &GenJet_size);
      fChain_ElectronLoose->SetBranchAddress("ElectronLoose_size", &ElectronLoose_size);
      fChain_ElectronTight->SetBranchAddress("ElectronTight_size", &ElectronTight_size);
      fChain_MuonLoose->SetBranchAddress("MuonLoose_size", &MuonLoose_size);
      fChain_MuonTight->SetBranchAddress("MuonTight_size", &MuonTight_size);
      fChain_JetPUPPI->SetBranchAddress("JetPUPPI_size", &JetPUPPI_size);
      fChain_PuppiMissingET->SetBranchAddress("PuppiMissingET_size", &PuppiMissingET_size);
      fChain_Photons->SetBranchAddress("Photons_size", &Photons_size);
      fChain_Selection->SetBranchAddress("nFiltered", &nFiltered);


      fChain_Particle->SetBranchAddress("PID", Particle_PID);
      fChain_Particle->SetBranchAddress("Charge", Particle_Charge);
      fChain_Particle->SetBranchAddress("Status", Particle_Status);
      fChain_Particle->SetBranchAddress("P", Particle_P);
      fChain_Particle->SetBranchAddress("Px", Particle_Px);
      fChain_Particle->SetBranchAddress("Py", Particle_Py);
      fChain_Particle->SetBranchAddress("Pz", Particle_Pz);
      fChain_Particle->SetBranchAddress("E", Particle_E);
      fChain_Particle->SetBranchAddress("PT", Particle_PT);
      fChain_Particle->SetBranchAddress("Eta", Particle_Eta);
      fChain_Particle->SetBranchAddress("Phi", Particle_Phi);
      fChain_Particle->SetBranchAddress("Mass", Particle_Mass);
      fChain_Particle->SetBranchAddress("IsolationVar", Particle_IsolationVar);
      fChain_Vertex->SetBranchAddress("SumPT2", Vertex_SumPT2);
      fChain_GenJet->SetBranchAddress("PT", GenJet_PT);
      fChain_GenJet->SetBranchAddress("Eta", GenJet_Eta);
      fChain_GenJet->SetBranchAddress("Phi", GenJet_Phi);
      fChain_GenJet->SetBranchAddress("Mass", GenJet_Mass);
      fChain_ElectronLoose->SetBranchAddress("Charge", ElectronLoose_Charge);
      fChain_ElectronLoose->SetBranchAddress("Particle", ElectronLoose_Particle);
      fChain_ElectronLoose->SetBranchAddress("PT", ElectronLoose_PT);
      fChain_ElectronLoose->SetBranchAddress("Eta", ElectronLoose_Eta);
      fChain_ElectronLoose->SetBranchAddress("Phi", ElectronLoose_Phi);
      fChain_ElectronLoose->SetBranchAddress("Mass", ElectronLoose_Mass);
      fChain_ElectronLoose->SetBranchAddress("IsolationVar", ElectronLoose_IsolationVar);
      fChain_ElectronTight->SetBranchAddress("Charge", ElectronTight_Charge);
      fChain_ElectronTight->SetBranchAddress("Particle", ElectronTight_Particle);
      fChain_ElectronTight->SetBranchAddress("PT", ElectronTight_PT);
      fChain_ElectronTight->SetBranchAddress("Eta", ElectronTight_Eta);
      fChain_ElectronTight->SetBranchAddress("Phi", ElectronTight_Phi);
      fChain_ElectronTight->SetBranchAddress("Mass", ElectronTight_Mass);
      fChain_ElectronTight->SetBranchAddress("IsolationVar", ElectronTight_IsolationVar);
      fChain_MuonLoose->SetBranchAddress("Charge", MuonLoose_Charge);
      fChain_MuonLoose->SetBranchAddress("Particle", MuonLoose_Particle);
      fChain_MuonLoose->SetBranchAddress("PT", MuonLoose_PT);
      fChain_MuonLoose->SetBranchAddress("Eta", MuonLoose_Eta);
      fChain_MuonLoose->SetBranchAddress("Phi", MuonLoose_Phi);
      fChain_MuonLoose->SetBranchAddress("Mass", MuonLoose_Mass);
      fChain_MuonLoose->SetBranchAddress("IsolationVar", MuonLoose_IsolationVar);
      fChain_MuonTight->SetBranchAddress("Charge", MuonTight_Charge);
      fChain_MuonTight->SetBranchAddress("Particle", MuonTight_Particle);
      fChain_MuonTight->SetBranchAddress("PT", MuonTight_PT);
      fChain_MuonTight->SetBranchAddress("Eta", MuonTight_Eta);
      fChain_MuonTight->SetBranchAddress("Phi", MuonTight_Phi);
      fChain_MuonTight->SetBranchAddress("Mass", MuonTight_Mass);
      fChain_MuonTight->SetBranchAddress("IsolationVar", MuonTight_IsolationVar);
      fChain_JetPUPPI->SetBranchAddress("ID", JetPUPPI_ID);
      fChain_JetPUPPI->SetBranchAddress("GenJet", JetPUPPI_GenJet);
      fChain_JetPUPPI->SetBranchAddress("PT", JetPUPPI_PT);
      fChain_JetPUPPI->SetBranchAddress("Eta", JetPUPPI_Eta);
      fChain_JetPUPPI->SetBranchAddress("Phi", JetPUPPI_Phi);
      fChain_JetPUPPI->SetBranchAddress("Mass", JetPUPPI_Mass);
      fChain_JetPUPPI->SetBranchAddress("MVAv2", JetPUPPI_MVAv2);
      fChain_JetPUPPI->SetBranchAddress("DeepCSV", JetPUPPI_DeepCSV);
      fChain_JetPUPPI->SetBranchAddress("PartonFlavor", JetPUPPI_PartonFlavor);
      fChain_JetPUPPI->SetBranchAddress("HadronFlavor", JetPUPPI_HadronFlavor);
      fChain_JetPUPPI->SetBranchAddress("GenPartonPID", JetPUPPI_GenPartonPID);
      fChain_PuppiMissingET->SetBranchAddress("MET", PuppiMissingET_MET);
      fChain_PuppiMissingET->SetBranchAddress("Phi", PuppiMissingET_Phi);
      fChain_PuppiMissingET->SetBranchAddress("Eta", PuppiMissingET_Eta);
      fChain_Photons->SetBranchAddress("PT", Photons_PT);
      fChain_Photons->SetBranchAddress("Eta", Photons_Eta);
      fChain_Photons->SetBranchAddress("Phi", Photons_Phi);
      fChain_Photons->SetBranchAddress("Mass", Photons_Mass);
      fChain_Photons->SetBranchAddress("IsolationCharged", Photons_IsolationCharged);
      fChain_Photons->SetBranchAddress("IsolationNeutral", Photons_IsolationNeutral);
      fChain_Photons->SetBranchAddress("IsolationPhoton", Photons_IsolationPhoton);
   

      Notify();
    }

    void Notify(){
      b_Run = fChain_Event->GetBranch("Run");
      b_Event = fChain_Event->GetBranch("Event");
      b_Lumi = fChain_Event->GetBranch("Lumi");
      b_Particle_size = fChain_Particle->GetBranch("Particle_size");
      b_Vertex_size = fChain_Vertex->GetBranch("Vertex_size");
      b_GenJet_size = fChain_GenJet->GetBranch("GenJet_size");
      b_ElectronLoose_size = fChain_ElectronLoose->GetBranch("ElectronLoose_size");
      b_ElectronTight_size = fChain_ElectronTight->GetBranch("ElectronTight_size");
      b_MuonLoose_size = fChain_MuonLoose->GetBranch("MuonLoose_size");
      b_MuonTight_size = fChain_MuonTight->GetBranch("MuonTight_size");
      b_JetPUPPI_size = fChain_JetPUPPI->GetBranch("JetPUPPI_size");
      b_PuppiMissingET_size = fChain_PuppiMissingET->GetBranch("PuppiMissingET_size");
      b_Photons_size = fChain_Photons->GetBranch("Photons_size");
      b_nFiltered = fChain_Selection->GetBranch("nFiltered");


      b_Particle_PID = fChain_Particle->GetBranch("PID");
      b_Particle_Charge = fChain_Particle->GetBranch("Charge");
      b_Particle_Status = fChain_Particle->GetBranch("Status");
      b_Particle_P = fChain_Particle->GetBranch("P");
      b_Particle_Px = fChain_Particle->GetBranch("Px");
      b_Particle_Py = fChain_Particle->GetBranch("Py");
      b_Particle_Pz = fChain_Particle->GetBranch("Pz");
      b_Particle_E = fChain_Particle->GetBranch("E");
      b_Particle_PT = fChain_Particle->GetBranch("PT");
      b_Particle_Eta = fChain_Particle->GetBranch("Eta");
      b_Particle_Phi = fChain_Particle->GetBranch("Phi");
      b_Particle_Mass = fChain_Particle->GetBranch("Mass");
      b_Particle_IsolationVar = fChain_Particle->GetBranch("IsolationVar");
      b_Vertex_SumPT2 = fChain_Vertex->GetBranch("SumPT2");
      b_GenJet_PT = fChain_GenJet->GetBranch("PT");
      b_GenJet_Eta = fChain_GenJet->GetBranch("Eta");
      b_GenJet_Phi = fChain_GenJet->GetBranch("Phi");
      b_GenJet_Mass = fChain_GenJet->GetBranch("Mass");
      b_ElectronLoose_Charge = fChain_ElectronLoose->GetBranch("Charge");
      b_ElectronLoose_Particle = fChain_ElectronLoose->GetBranch("Particle");
      b_ElectronLoose_PT = fChain_ElectronLoose->GetBranch("PT");
      b_ElectronLoose_Eta = fChain_ElectronLoose->GetBranch("Eta");
      b_ElectronLoose_Phi = fChain_ElectronLoose->GetBranch("Phi");
      b_ElectronLoose_Mass = fChain_ElectronLoose->GetBranch("Mass");
      b_ElectronLoose_IsolationVar = fChain_ElectronLoose->GetBranch("IsolationVar");
      b_ElectronTight_Charge = fChain_ElectronTight->GetBranch("Charge");
      b_ElectronTight_Particle = fChain_ElectronTight->GetBranch("Particle");
      b_ElectronTight_PT = fChain_ElectronTight->GetBranch("PT");
      b_ElectronTight_Eta = fChain_ElectronTight->GetBranch("Eta");
      b_ElectronTight_Phi = fChain_ElectronTight->GetBranch("Phi");
      b_ElectronTight_Mass = fChain_ElectronTight->GetBranch("Mass");
      b_ElectronTight_IsolationVar = fChain_ElectronTight->GetBranch("IsolationVar");
      b_MuonLoose_Charge = fChain_MuonLoose->GetBranch("Charge");
      b_MuonLoose_Particle = fChain_MuonLoose->GetBranch("Particle");
      b_MuonLoose_PT = fChain_MuonLoose->GetBranch("PT");
      b_MuonLoose_Eta = fChain_MuonLoose->GetBranch("Eta");
      b_MuonLoose_Phi = fChain_MuonLoose->GetBranch("Phi");
      b_MuonLoose_Mass = fChain_MuonLoose->GetBranch("Mass");
      b_MuonLoose_IsolationVar = fChain_MuonLoose->GetBranch("IsolationVar");
      b_MuonTight_Charge = fChain_MuonTight->GetBranch("Charge");
      b_MuonTight_Particle = fChain_MuonTight->GetBranch("Particle");
      b_MuonTight_PT = fChain_MuonTight->GetBranch("PT");
      b_MuonTight_Eta = fChain_MuonTight->GetBranch("Eta");
      b_MuonTight_Phi = fChain_MuonTight->GetBranch("Phi");
      b_MuonTight_Mass = fChain_MuonTight->GetBranch("Mass");
      b_MuonTight_IsolationVar = fChain_MuonTight->GetBranch("IsolationVar");
      b_JetPUPPI_ID = fChain_JetPUPPI->GetBranch("ID");
      b_JetPUPPI_GenJet = fChain_JetPUPPI->GetBranch("GenJet");
      b_JetPUPPI_PT = fChain_JetPUPPI->GetBranch("PT");
      b_JetPUPPI_Eta = fChain_JetPUPPI->GetBranch("Eta");
      b_JetPUPPI_Phi = fChain_JetPUPPI->GetBranch("Phi");
      b_JetPUPPI_Mass = fChain_JetPUPPI->GetBranch("Mass");
      b_JetPUPPI_MVAv2 = fChain_JetPUPPI->GetBranch("MVAv2");
      b_JetPUPPI_DeepCSV = fChain_JetPUPPI->GetBranch("DeepCSV");
      b_JetPUPPI_PartonFlavor = fChain_JetPUPPI->GetBranch("PartonFlavor");
      b_JetPUPPI_HadronFlavor = fChain_JetPUPPI->GetBranch("HadronFlavor");
      b_JetPUPPI_GenPartonPID = fChain_JetPUPPI->GetBranch("GenPartonPID");
      b_PuppiMissingET_MET = fChain_PuppiMissingET->GetBranch("MET");
      b_PuppiMissingET_Phi = fChain_PuppiMissingET->GetBranch("Phi");
      b_PuppiMissingET_Eta = fChain_PuppiMissingET->GetBranch("Eta");
      b_Photons_PT = fChain_Photons->GetBranch("PT");
      b_Photons_Eta = fChain_Photons->GetBranch("Eta");
      b_Photons_Phi = fChain_Photons->GetBranch("Phi");
      b_Photons_Mass = fChain_Photons->GetBranch("Mass");
      b_Photons_IsolationCharged = fChain_Photons->GetBranch("IsolationCharged");
      b_Photons_IsolationNeutral = fChain_Photons->GetBranch("IsolationNeutral");
      b_Photons_IsolationPhoton = fChain_Photons->GetBranch("IsolationPhoton");

    }

    Int_t GetEntry(Long64_t entry){
      // Read contents of entry.
      fChain_MuonLoose->GetEntry(entry);
      fChain_ElectronTight->GetEntry(entry);
      fChain_ElectronLoose->GetEntry(entry);
      fChain_Particle->GetEntry(entry);
      fChain_Photons->GetEntry(entry);
      fChain_Vertex->GetEntry(entry);
      fChain_GenJet->GetEntry(entry);
      fChain_MuonTight->GetEntry(entry);
      fChain_Selection->GetEntry(entry);
      fChain_JetPUPPI->GetEntry(entry);
      fChain_PuppiMissingET->GetEntry(entry);
      fChain_Event->GetEntry(entry);


      return 0;
    }

    TTree *fChain_MuonLoose;
    TTree *fChain_ElectronTight;
    TTree *fChain_ElectronLoose;
    TTree *fChain_Particle;
    TTree *fChain_Photons;
    TTree *fChain_Vertex;
    TTree *fChain_GenJet;
    TTree *fChain_MuonTight;
    TTree *fChain_Selection;
    TTree *fChain_JetPUPPI;
    TTree *fChain_PuppiMissingET;
    TTree *fChain_Event;

    Int_t fCurrent; //!current Tree number in a TChain

     // Declaration of leave types
    Int_t Run;
    Int_t Event;
    Int_t Lumi;
    Int_t Particle_size;
    Int_t Vertex_size;
    Int_t GenJet_size;
    Int_t ElectronLoose_size;
    Int_t ElectronTight_size;
    Int_t MuonLoose_size;
    Int_t MuonTight_size;
    Int_t JetPUPPI_size;
    Int_t PuppiMissingET_size;
    Int_t Photons_size;
    Int_t nFiltered;


    Int_t Particle_PID[100];
    Int_t Particle_Charge[100];
    Int_t Particle_Status[100];
    Float_t Particle_P[100];
    Float_t Particle_Px[100];
    Float_t Particle_Py[100];
    Float_t Particle_Pz[100];
    Float_t Particle_E[100];
    Float_t Particle_PT[100];
    Float_t Particle_Eta[100];
    Float_t Particle_Phi[100];
    Float_t Particle_Mass[100];
    Float_t Particle_IsolationVar[100];
    Float_t Vertex_SumPT2[100];
    Float_t GenJet_PT[100];
    Float_t GenJet_Eta[100];
    Float_t GenJet_Phi[100];
    Float_t GenJet_Mass[100];
    Int_t ElectronLoose_Charge[100];
    Int_t ElectronLoose_Particle[100];
    Float_t ElectronLoose_PT[100];
    Float_t ElectronLoose_Eta[100];
    Float_t ElectronLoose_Phi[100];
    Float_t ElectronLoose_Mass[100];
    Float_t ElectronLoose_IsolationVar[100];
    Int_t ElectronTight_Charge[100];
    Int_t ElectronTight_Particle[100];
    Float_t ElectronTight_PT[100];
    Float_t ElectronTight_Eta[100];
    Float_t ElectronTight_Phi[100];
    Float_t ElectronTight_Mass[100];
    Float_t ElectronTight_IsolationVar[100];
    Int_t MuonLoose_Charge[100];
    Int_t MuonLoose_Particle[100];
    Float_t MuonLoose_PT[100];
    Float_t MuonLoose_Eta[100];
    Float_t MuonLoose_Phi[100];
    Float_t MuonLoose_Mass[100];
    Float_t MuonLoose_IsolationVar[100];
    Int_t MuonTight_Charge[100];
    Int_t MuonTight_Particle[100];
    Float_t MuonTight_PT[100];
    Float_t MuonTight_Eta[100];
    Float_t MuonTight_Phi[100];
    Float_t MuonTight_Mass[100];
    Float_t MuonTight_IsolationVar[100];
    Int_t JetPUPPI_ID[100];
    Int_t JetPUPPI_GenJet[100];
    Float_t JetPUPPI_PT[100];
    Float_t JetPUPPI_Eta[100];
    Float_t JetPUPPI_Phi[100];
    Float_t JetPUPPI_Mass[100];
    Int_t JetPUPPI_MVAv2[100];
    Int_t JetPUPPI_DeepCSV[100];
    Int_t JetPUPPI_PartonFlavor[100];
    Int_t JetPUPPI_HadronFlavor[100];
    Int_t JetPUPPI_GenPartonPID[100];
    Float_t PuppiMissingET_MET[100];
    Float_t PuppiMissingET_Phi[100];
    Float_t PuppiMissingET_Eta[100];
    Float_t Photons_PT[100];
    Float_t Photons_Eta[100];
    Float_t Photons_Phi[100];
    Float_t Photons_Mass[100];
    Float_t Photons_IsolationCharged[100];
    Float_t Photons_IsolationNeutral[100];
    Float_t Photons_IsolationPhoton[100];


    // List of branches
    TBranch *b_Run; //!
    TBranch *b_Event; //!
    TBranch *b_Lumi; //!
    TBranch *b_Particle_size; //!
    TBranch *b_Vertex_size; //!
    TBranch *b_GenJet_size; //!
    TBranch *b_ElectronLoose_size; //!
    TBranch *b_ElectronTight_size; //!
    TBranch *b_MuonLoose_size; //!
    TBranch *b_MuonTight_size; //!
    TBranch *b_JetPUPPI_size; //!
    TBranch *b_PuppiMissingET_size; //!
    TBranch *b_Photons_size; //!
    TBranch *b_nFiltered; //!


    TBranch *b_Particle_PID; //!
    TBranch *b_Particle_Charge; //!
    TBranch *b_Particle_Status; //!
    TBranch *b_Particle_P; //!
    TBranch *b_Particle_Px; //!
    TBranch *b_Particle_Py; //!
    TBranch *b_Particle_Pz; //!
    TBranch *b_Particle_E; //!
    TBranch *b_Particle_PT; //!
    TBranch *b_Particle_Eta; //!
    TBranch *b_Particle_Phi; //!
    TBranch *b_Particle_Mass; //!
    TBranch *b_Particle_IsolationVar; //!
    TBranch *b_Vertex_SumPT2; //!
    TBranch *b_GenJet_PT; //!
    TBranch *b_GenJet_Eta; //!
    TBranch *b_GenJet_Phi; //!
    TBranch *b_GenJet_Mass; //!
    TBranch *b_ElectronLoose_Charge; //!
    TBranch *b_ElectronLoose_Particle; //!
    TBranch *b_ElectronLoose_PT; //!
    TBranch *b_ElectronLoose_Eta; //!
    TBranch *b_ElectronLoose_Phi; //!
    TBranch *b_ElectronLoose_Mass; //!
    TBranch *b_ElectronLoose_IsolationVar; //!
    TBranch *b_ElectronTight_Charge; //!
    TBranch *b_ElectronTight_Particle; //!
    TBranch *b_ElectronTight_PT; //!
    TBranch *b_ElectronTight_Eta; //!
    TBranch *b_ElectronTight_Phi; //!
    TBranch *b_ElectronTight_Mass; //!
    TBranch *b_ElectronTight_IsolationVar; //!
    TBranch *b_MuonLoose_Charge; //!
    TBranch *b_MuonLoose_Particle; //!
    TBranch *b_MuonLoose_PT; //!
    TBranch *b_MuonLoose_Eta; //!
    TBranch *b_MuonLoose_Phi; //!
    TBranch *b_MuonLoose_Mass; //!
    TBranch *b_MuonLoose_IsolationVar; //!
    TBranch *b_MuonTight_Charge; //!
    TBranch *b_MuonTight_Particle; //!
    TBranch *b_MuonTight_PT; //!
    TBranch *b_MuonTight_Eta; //!
    TBranch *b_MuonTight_Phi; //!
    TBranch *b_MuonTight_Mass; //!
    TBranch *b_MuonTight_IsolationVar; //!
    TBranch *b_JetPUPPI_ID; //!
    TBranch *b_JetPUPPI_GenJet; //!
    TBranch *b_JetPUPPI_PT; //!
    TBranch *b_JetPUPPI_Eta; //!
    TBranch *b_JetPUPPI_Phi; //!
    TBranch *b_JetPUPPI_Mass; //!
    TBranch *b_JetPUPPI_MVAv2; //!
    TBranch *b_JetPUPPI_DeepCSV; //!
    TBranch *b_JetPUPPI_PartonFlavor; //!
    TBranch *b_JetPUPPI_HadronFlavor; //!
    TBranch *b_JetPUPPI_GenPartonPID; //!
    TBranch *b_PuppiMissingET_MET; //!
    TBranch *b_PuppiMissingET_Phi; //!
    TBranch *b_PuppiMissingET_Eta; //!
    TBranch *b_Photons_PT; //!
    TBranch *b_Photons_Eta; //!
    TBranch *b_Photons_Phi; //!
    TBranch *b_Photons_Mass; //!
    TBranch *b_Photons_IsolationCharged; //!
    TBranch *b_Photons_IsolationNeutral; //!
    TBranch *b_Photons_IsolationPhoton; //!

};

#endif
