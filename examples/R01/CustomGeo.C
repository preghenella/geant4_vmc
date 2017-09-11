#include "TVirtualMC.h"

/******************************************************************************/

std::map<std::string, TGeoMaterial *> materials;
std::map<std::string, TGeoMedium *> media;

/******************************************************************************/

double magneticField = 5.; // kG

/******************************************************************************/

bool iLargeTank = false;
bool iBrickWall = true;
bool iSheetWall = false;
bool iSingleFoil = false;

/******************************************************************************/

std::string mWorld      = "Air";
std::string mLargeTank  = "Water";
std::string mBrickWall  = "Lead";
std::string mSheetWall  = "Silicon";
std::string mSingleFoil = "Silicon";
float pSingleFoil = -10.;

/******************************************************************************/

void CustomGeo();
void DrawGeometry();
void Transparency(char value);
void CreateGeoManager();
void CreateMaterials();
void CreateLargeTank(std::string &name);
void CreateBrickWall(std::string &name);
void CreateSheetWall(std::string &name);
void CreateSingleFoil(std::string &name, float pos);

/******************************************************************************/

void AnimateTracks();
void DrawTracks();
void DrawTrack(const TVirtualGeoTrack *track);
void DrawPoly(const TVirtualGeoTrack *track);
  
/******************************************************************************/

void
DrawGeometry()
{
  if (!gGeoManager) CustomGeo();
  gGeoManager->DefaultColors();
  gGeoManager->GetTopVolume()->Draw("ogl");
}

/******************************************************************************/

void
Transparency(char value = 50)
{
}

/******************************************************************************/

void
CustomGeo()
{

  if (!gGeoManager) CreateGeoManager();

  /** set magnetic field **/
  if (gMC) gMC->SetMagField(new TGeoUniformMagField(0., 0., magneticField));
  
  /** create materials **/
  CreateMaterials();
  
  /** create world **/
  auto world = gGeoManager->MakeBox("World", media[mWorld], 100., 100., 100.);
  gGeoManager->SetTopVolume(world);

  /** create structures **/
  if (iLargeTank) CreateLargeTank(mLargeTank);
  if (iBrickWall) CreateBrickWall(mBrickWall);
  if (iSheetWall) CreateSheetWall(mSheetWall);
  if (iSingleFoil) CreateSingleFoil(mSingleFoil, pSingleFoil);

  /** close geometry **/
  gGeoManager->CloseGeometry();
}

/******************************************************************************/

void
CreateGeoManager()
{
  /** create geometry manager **/
  new TGeoManager("HSSIP", "HSSIP geometry");
}

/******************************************************************************/

void
CreateMaterials()
{

  if (!gGeoManager) CreateGeoManager();
  auto table = gGeoManager->GetElementTable();
  
  Double_t a;        // Mass of a mole in g/mole   
  Double_t z;        // Atomic number
  Double_t density;  // Material density in g/cm3

  Double_t param[20];
  for (Int_t i = 0; i < 20; ++i) param[i] = 0.;
  param[1] = 2;     // ifield - User defined magnetic field
  param[2] = 50.;   // fieldm - Maximum field value (in kiloGauss)
  //  param[3] = 20.;  // tmaxfd - Maximum angle due to field deflection 
  //  param[4] = -0.01; // stemax - Maximum displacement for multiple scat 
  //  param[5] = -.3;   // deemax - Maximum fractional energy loss, DLS 
  //  param[6] = .001;  // epsil - Tracking precision
  //  param[7] = -.8;   // stmin

  
  /** Vacuum **/
  density = 0.;
  materials["Vacuum"] = new TGeoMaterial("Vacuum", table->FindElement("Vacuum"), density);
  media["Vacuum"] = new TGeoMedium("Vacuum", media.size(), materials["Vacuum"], param);
  
  /** Air **/
  density = 1.225e-3;
  auto mixture = new TGeoMixture("Air", 4, density);
  mixture->AddElement(table->FindElement("N"),  0.78084);
  mixture->AddElement(table->FindElement("O"),  0.20946);
  mixture->AddElement(table->FindElement("O"),  0.20946);
  mixture->AddElement(table->FindElement("Ar"), 0.00934);
  mixture->AddElement(table->FindElement("C"),  0.00040);
  mixture->AddElement(table->FindElement("O"),  0.00040);
  mixture->AddElement(table->FindElement("O"),  0.00040);
  materials["Air"] = mixture;
  media["Air"] = new TGeoMedium("Air", media.size(), materials["Air"], param);

  /** Water **/
  density = 1.;
  mixture = new TGeoMixture("Water", 2, density);
  mixture->AddElement(table->FindElement("H"),  2./3.);
  mixture->AddElement(table->FindElement("O"),  1./3.);
  materials["Water"] = mixture;
  media["Water"] = new TGeoMedium("Water", media.size(), materials["Water"], param);

  /** Silicon **/
  density = 2.3290;
  materials["Silicon"] = new TGeoMaterial("Silicon", table->FindElement("Si"), density);
  media["Silicon"] = new TGeoMedium("Silicon", media.size(), materials["Silicon"], param);
      
  /** Aluminum **/
  density = 2.70;
  materials["Aluminum"] = new TGeoMaterial("Aluminum", table->FindElement("Al"), density);
  media["Aluminum"] = new TGeoMedium("Aluminum", media.size(), materials["Aluminum"], param);
      
  /** Diamond **/
  density = 3.51;
  materials["Diamond"] = new TGeoMaterial("Diamond", table->FindElement("C"), density);
  media["Diamond"] = new TGeoMedium("Diamond", media.size(), materials["Diamond"], param);
      
  /** Gold **/
  density = 19.32;
  materials["Gold"] = new TGeoMaterial("Gold", table->FindElement("Au"), density);
  media["Gold"] = new TGeoMedium("Gold", media.size(), materials["Gold"], param);
      
  /** Lead **/
  density = 11.34;
  materials["Lead"] = new TGeoMaterial("Lead", table->FindElement("Pb"), density);
  media["Lead"] = new TGeoMedium("Lead", media.size(), materials["Lead"], param);
      
}

/******************************************************************************/

void
CreateLargeTank(std::string &name)
{
  /** create large tank **/
  auto tank = gGeoManager->MakeBox("Tank", media[name], 20., 50., 50.);
  gGeoManager->GetTopVolume()->AddNode(tank, 1, new TGeoTranslation(30., 0., 0.));
    
}

/******************************************************************************/

void
CreateBrickWall(std::string &name)
{
  /** create lead brick **/
  auto brick = gGeoManager->MakeBox("Brick", media[name], 4.5, 2.25, 2.25);
  /** create brick layer **/
  auto brickLayer = gGeoManager->MakeBox("BrickLayer", media["Vacuum"], 5., 50, 2.5);
  for (int ix = 0; ix < 20; ix++) brickLayer->AddNode(brick, 1 + ix, new TGeoTranslation(0., -50. + 2.25 + ix * 5., 0.));
  /** add brick layers **/
  for (int iy = 0; iy < 20; iy++) gGeoManager->GetTopVolume()->AddNode(brickLayer, 1 + iy, new TGeoTranslation(0., 0., -50. + 2.25 + iy * 5.));
    
}

/******************************************************************************/

void
CreateSheetWall(std::string &name)
{
  /** create lead sheet **/
  auto sheet = gGeoManager->MakeBox("Sheet", media[name], 0.5, 50., 50.);
  /** add sheets **/
  for (int ix = 0; ix < 9; ix++) gGeoManager->GetTopVolume()->AddNode(sheet, 1 + ix, new TGeoTranslation(-10. - 5. * ix, 0., 0.));
    
}

/******************************************************************************/

void
CreateSingleFoil(std::string &name, float pos)
{
  /** create lead sheet **/
  auto foil = gGeoManager->MakeBox("Foil", media[name], 1., 50., 50.);
  gGeoManager->GetTopVolume()->AddNode(foil, 1, new TGeoTranslation(pos, 0., 0.));
    
}

/******************************************************************************/

void
AnimateTracks()
{
  gGeoManager->AnimateTracks(0., 1.e-8, 200, "/*/S");
}

/******************************************************************************/

void
DrawTracks()
{
  if (!gGeoManager) return;
  int nTracks = gGeoManager->GetNtracks();
  for (int iTrack = 0; iTrack < nTracks; iTrack++)
    DrawTrack(gGeoManager->GetTrack(iTrack));
}

/******************************************************************************/

void
DrawTrack(const TVirtualGeoTrack *track)
{
  DrawPoly(track);
  int nDaugh = track->GetNdaughters();
  for (int iDaugh = 0; iDaugh < nDaugh; iDaugh++)
    DrawTrack(track->GetDaughter(iDaugh));
}

/******************************************************************************/

void
DrawPoly(const TVirtualGeoTrack *track)
{
  auto nPoints = track->GetNpoints();
  auto poly = new TPolyLine3D(nPoints);
  for (int iPoint = 0; iPoint < nPoints; iPoint++) {
    auto xyzt = track->GetPoint(iPoint);
    poly->SetPoint(iPoint, xyzt[0], xyzt[1], xyzt[2]);
  }
  poly->SetLineColor(track->GetLineColor());
  poly->SetLineWidth(track->GetLineWidth());
  poly->SetLineStyle(track->GetLineStyle());
  if (track->GetParentId() < 0) {
    poly->SetLineColor(1);
    poly->SetLineWidth(5);
  }
  poly->Draw("same,c");
}
