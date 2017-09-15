#include "TVirtualMC.h"

/** manual generator **/
Float_t momentum = 10; // [GeV/c]
Int_t pdgcode = 2212;
Int_t nInjects = 100;

/** pythia generator **/
bool usePythia = false;
float pythiaEnergy = 100; // GeV
const char *pythiaProjectile = "e+";
const char *pythiaTarget = "e-";
int nPythiaEvents = 1;
bool isPythiaInitialised = false;
void initPythia();

void
CustomGen()
{

  /** use particles from PYTHIA event generator **/
  if (usePythia) {

    /** load pythia libraries **/
    gSystem->Load("libpythia6");
    if (!isPythiaInitialised) initPythia();

    /** generate pythia events **/
    TPythia6 *pythia = TPythia6::Instance();
    TClonesArray *particles = new TClonesArray("TParticle");
    for (int iev = 0; iev < nPythiaEvents; iev++) {
      pythia->GenerateEvent();
      
      /** import particles from pythia **/
      pythia->ImportParticles(particles, "Final");
      
      /** inject particles into simulation **/
      int nParticles = particles->GetEntries();
      int ntr;
      for (int ipart = 0; ipart < nParticles; ipart++) {
	auto particle = (TParticle *)particles->At(ipart);
	auto pdgcode = particle->GetPdgCode();
	auto px = particle->Px();
	auto py = particle->Py();
	auto pz = particle->Pz();
	auto e  = particle->Energy();
	auto vx = particle->Vx();
	auto vy = particle->Vy();
	auto vz = particle->Vz();
	auto tof = particle->T();
	
	gMC->GetStack()->PushTrack(true, -1, pdgcode, px, py, pz, e, vx, vy, vz, tof, 0., 0., 0., kPPrimary, ntr, 1., 0);

      }
    }
    
   return;
  }
  
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
  vx = -2.;
  vy = 0.; 
  vz = 0.; 
  
  // Add particle to stack 
  Int_t ntr;  
  std::cout << "Injecting " << nInjects << " " << particle->GetName() << " with momentum " << momentum << " GeV/c" << std::endl;
  for (Int_t i = 0; i < nInjects; i++)
    gMC->GetStack()->PushTrack(true, -1, pdgcode, px, py, pz, e, vx, vy, vz, 0., 0., 0., 0., kPPrimary, ntr, 1., 0);

}

/******************************************************************************/

void
initPythia()
{

  /** get pythia instance **/
  TPythia6 *pythia = TPythia6::Instance();

  /** configure particle decays **/
  /** only particles with a decay within a volume limited by 
      rho = sqrt(x^2 + y^2) < xyMax and |z| < zMax are decayed **/
  pythia->Pygive("MSTJ(22) = 4"); 
  pythia->Pygive("PARJ(73) = 1");  // xyMax [mm]
  pythia->Pygive("MSTJ(22) = 10"); // zMax [mm]

  /** configure physics tune **/
  /** Perugia 2011 physics tune **/
  pythia->Pygive("MSTP(5) = 350");
  
  /** configure physics process **/
  pythia->Pygive("MSEL = 2");
  //  pythia->Pygive("CKIN(3) = 100.");
  //  pythia->Pygive("CKIN(4) = 150.");
  //  pythia->Pygive("CKIN(7) = -1.");
  //  pythia->Pygive("CKIN(8) = 1.");
  
  /** configure random seed **/
  pythia->Pygive("MRPY(1) = 123456789"); 
  
  /** initialise pythia **/
  pythia->Initialize("CMS", pythiaProjectile, pythiaTarget, pythiaEnergy);
  
}
