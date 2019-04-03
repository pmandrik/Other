
// Include files

#include "HepMC/IO_GenEvent.h"
#include "HepMC/GenEvent.h"

void hepmc_test(){
  gSystem->Load("/home/pmandrik/soft/hepmc2.07-beta00/lib/libHepMC.so.4.0.0");

  HepMC::IO_GenEvent ascii_in("associated_gu_to_Hc.hepmc",std::ios::in);
  HepMC::GenEvent* evt = ascii_in.read_next_event();

  int event_counter = 0;
  while( evt ) {
    cout << event_counter << endl;

    HepMC::GenParticle * higgs;
    for( HepMC::GenEvent::particle_iterator it = evt->particles_begin(); it != evt->particles_end(); it++ ){
      HepMC::GenParticle * particle  = *it;
      cout << particle->pdg_id() << endl;

      if( particle->pdg_id() == 25 )
        higgs = particle;
    }

    delete evt;
    ascii_in >> evt;

    event_counter++;
    if(event_counter > 10) break;
  }
}

