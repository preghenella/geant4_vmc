#include "TVirtualMC.h"

void
CustomGen()
{

  Float_t momentum = 1.; // [GeV/c]
  
  // Particle
  Int_t pdgcode = 22;
  auto database = TDatabasePDG::Instance();
  auto particle = database->GetParticle(pdgcode);
  auto m = particle->Mass();
  
  // Momentum 
  Double_t px, py, pz, e; // [GeV/c]
  px = momentum;
  py = 0.; 
  pz = 0.; 
  e  = sqrt(px * px + py * py + pz * pz + m * m);
  
  // Position
  Double_t vx, vy, vz; // [cm]
  vx = -100.;
  vy = -22.5; 
  vz = -22.5; 
  
  // Add particle to stack 
  Int_t ntr;  
  std::cout << "Injecting " << particle->GetName() << " with momentum " << momentum << " GeV/c" << std::endl;
  gMC->GetStack()->PushTrack(true, -1, pdgcode, px, py, pz, e, vx, vy, vz, 0., 0., 0., 0., kPPrimary, ntr, 1., 0);

}
