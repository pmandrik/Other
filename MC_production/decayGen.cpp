// IHEP, Petr Mandrik, a long time ago ... 
// g++ decayGen.cpp `root-config --libs --cflags` -o foo

//----------------------------------------------------------------//
// P0   P1   P2
// E0   E1   E2
// M0   M1   M2     M_{n-1} Mn=m_n
// => 0 => 0 => 0  ... => 0 ->
//     \    \    \         \
//     m1   m2   m3        m_{n-1}
//     Em1  Em2  Em3        E_{n-1}
//     Pm1  Pm2  Pm3
//----------------------------------------------------------------//
// m_{i+1} + m_{i+2} + ... + m_n <= Mi <= M_{i-1} - m_i
//----------------------------------------------------------------//
// Ai = m_{i+1} + m_{i+2} + ... + m_n
// B = M0 - m_1 - m_2 - ... - m_n
// Ri = random(0., 1.)
// Mi = Ai + Ri * B
//----------------------------------------------------------------//
// A_{i-1} - Ai = m_i
// Mi <= M_{i-1} - m_i -> Ri <= R_{i-1}
//----------------------------------------------------------------//
// Ei = ( M_{i-1}^2 - Mi^2 + m_i^2 ) / ( 2*M_{i-1} )
//----------------------------------------------------------------//

#include <stdio.h>
#include <math.h>
#include <algorithm> 
#include <numeric>
#include <ctime>
#include "TRandom.h"
#include "TLorentzVector.h"
#include "TGenPhaseSpace.h"
#include "TH1D.h"
#include "TCanvas.h"

TRandom * randGenerator = new TRandom();
const int MaxParticleNumber = 100;


class DecayGenerator{
  public:
  DecayGenerator(){}
  ~DecayGenerator(){}

  bool SetDecay(TLorentzVector P0, int N, double * masses){
    if(N < 2 or N >= MaxParticleNumber){
      return false;
    }

    this->P0 = P0;
    Bpar = P0.Mag();
    Npar = N;

    for(int i = 0; i < Npar; i++){
      m[i]  = masses[i];
      Bpar -= masses[i];
    }
    if(Bpar < 0) return false;

    return true;
  }

  double GenDecay(){
    // generate random numbers
    double random_numbers[MaxParticleNumber];
    random_numbers[0]   = 1.;    // M0
    random_numbers[Npar-1] = 0.; // Mn = m_n
    for(int i = 1; i < Npar-1; i++){
      random_numbers[i] = randGenerator->Uniform(0., 1.);
    }
  	std::sort(random_numbers+1, random_numbers+Npar-1, std::greater<float>());

    // generate invariant masses M
    double sumMasses = 0;
    double M[ MaxParticleNumber ];
    for(int i = Npar-1; i>=0; i--){
      sumMasses += m[i];
      M[i] = sumMasses + random_numbers[i] * Bpar;
    }

    // get energy from masses
    // rotate and boost into labor
    double Emi, cosTheta, Phi, p, pt, weight = 1.;
    TLorentzVector PM = P0;
    for(int i = 0; i < Npar-1; i++){
      Emi      = 0.5*(M[i]*M[i] - M[i+1]*M[i+1] + m[i]*m[i] ) / M[i];
      Phi      = randGenerator->Uniform(0., 2*M_PI);
      cosTheta = randGenerator->Uniform(-1., 1.);
      p        = std::sqrt(Emi*Emi - m[i]*m[i]);
      pt       = p * std::sqrt(1. - cosTheta*cosTheta);
      weight  *= p;

      Pm[i].SetXYZT( pt * cos(Phi),  pt * sin(Phi),  p * cosTheta, Emi );
      Pm[i].Boost( PM.BoostVector() );
      PM -= Pm[i];
    }
    Pm[Npar-1] = PM;

    return weight;
  }

  TLorentzVector * GetLorentzVector(int i){ return Pm+i;}

  double Bpar, m[ MaxParticleNumber ];
  int Npar;
  TLorentzVector P0, Pm[ MaxParticleNumber ];
};

int decayGen(){
  
  double m0 = 1.;
  double mass[100];

  int N = 25;
  for(int i = 0; i < 14; i++){
    mass[i] = 0.1;
  }

  TLorentzVector P0 = TLorentzVector(0,0,1.,sqrt(1.*1. + m0*m0));

  DecayGenerator * Generator1 = new DecayGenerator();
  Generator1->SetDecay(P0, N, mass);
  TH1D *h1 = new TH1D("h1","h1", 100, -0.5, 1.);
  h1->Sumw2();

  TGenPhaseSpace * Generator2 = new TGenPhaseSpace();
  Generator2->SetDecay(P0, N, mass);
  TH1D *h2 = new TH1D("h2","h2", 100, -0.5, 1.);
  h2->Sumw2();

  TLorentzVector *P[100];
  double weight;

  clock_t begin_time, end_time;
  double elapsed_secs;

  begin_time = clock();
  for(int i = 0; i < 10000000; i++){
    weight = Generator1->GenDecay();
    for(int j = 0; j < N; j++){
      P[j] = Generator1->GetLorentzVector(j);
    }
    h1->Fill(P[0]->Pt(), weight);
  }
  end_time = clock();
  elapsed_secs = double(end_time - begin_time) / CLOCKS_PER_SEC;
  printf("Generator1 time = %.2f \n", elapsed_secs);

  begin_time = clock();
  for(int i = 0; i < 10000000; i++){
    weight = Generator2->Generate();
    for(int j = 0; j < N; j++){
      P[j] = Generator2->GetDecay(j);
    }
    h2->Fill(P[0]->Pt(), weight);
  }
  end_time = clock();
  elapsed_secs = double(end_time - begin_time) / CLOCKS_PER_SEC;
  printf("Generator2 time = %.2f \n", elapsed_secs);

  TCanvas * c1 = new TCanvas("c1", "c1", 600, 480);
  h1->Draw("E");
  c1->Print("c1.png");

  TCanvas * c2 = new TCanvas("c2", "c2", 600, 480);
  h2->Draw("E");
  c2->Print("c2.png");

  TCanvas * c3 = new TCanvas("c3", "c3", 600, 480);
  TH1D * h3 = new TH1D(*h1);
  h3->Sumw2();
  h3->Divide(h2);
  h3->Draw("E");
  c3->Print("c3.png");

  return 0;
}

