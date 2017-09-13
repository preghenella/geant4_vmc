typedef std::vector<std::string> stringVector;

stringVector TRDVolumes = {"BFTRD", "BBTRD", "BTRD0", "BTRD1", "BTRD2", "BTRD3", "BTRD4", "BTRD5", "BTRD6", "BTRD7", "BTRD8", "BTRD9", "BTRD10", "BTRD11", "BTRD12", "BTRD13", "BTRD14", "BTRD15", "BTRD16", "BTRD17"};
stringVector TOFVolumes = {"FTOS", "BTOF0", "BTOF1", "BTOF2", "BTOF3", "BTOF4", "BTOF5", "BTOF6", "BTOF7", "BTOF8", "BTOF9", "BTOF10", "BTOF11", "BTOF12", "BTOF13", "BTOF14", "BTOF15", "BTOF16", "BTOF17"};
stringVector MCHVolumes = {"SC01", "SC02", "SC03", "SC04", "SC05I", "SC05O", "SC06I", "SC06O", "SC07I", "SC07O", "SC08I", "SC08O", "SC09I", "SC09O", "SC10I", "SC10O"};
stringVector MTRVolumes = {"SC11", "SC12", "SC13", "SC14"};
stringVector HMPIDVolumes = {"Hmp0", "Hmp1", "Hmp2", "Hmp3", "Hmp4", "Hmp5", "Hmp6"};

stringVector TRDTOFVolumes = {"BFTRD", "BBTRD", "BTRD0", "BTRD1", "BTRD2", "BTRD3", "BTRD4", "BTRD5", "BTRD6", "BTRD7", "BTRD8", "BTRD9", "BTRD10", "BTRD11", "BTRD12", "BTRD13", "BTRD14", "BTRD15", "BTRD16", "BTRD17", "FTOS", "BTOF0", "BTOF1", "BTOF2", "BTOF3", "BTOF4", "BTOF5", "BTOF6", "BTOF7", "BTOF8", "BTOF9", "BTOF10", "BTOF11", "BTOF12", "BTOF13", "BTOF14", "BTOF15", "BTOF16", "BTOF17"};

/** {name, {{toggle volumes}, {excluded volumes}} **/  
std::map<std::string, std::pair<stringVector, stringVector>> structures = {
  /** LHC **/
  {"Pipe", {{"CP", "RB24", "RB26Pipe"}, {"voRB24CuTubeA"}}},
  /** CENTRAL BARREL **/
  {"Solenoid", {{"L3MO"}, {}}},
  {"Doors", {{"L3MO"}, {"L3BM"}}},
  {"Frame", {{"B077", "BFMO", "BBMO", "Hcradle", "CalFrame"}, TRDTOFVolumes}},
  {"ITS", {{"ITSV"}, {}}},
  {"TPC", {{"TPC_M"}, {}}},
  {"TRD", {TRDVolumes, {}}},
  {"TOF", {TOFVolumes, {}}},
  {"HMPID", {HMPIDVolumes, {}}},
  {"PHOS", {{"PHOS"}, {}}},
  /** MUON SPECTROMETER **/
  {"Dipole", {{"Dipole"}, {}}},
  {"Absorber", {{"AFA", "YSAA", "YSAA1ExtraShield", "YCRE"}, {"AFaEndPlate", "AFass", "AFassAlRing", "YSAA3SteelPlate11", "YSAA3SteelPlate21", "YSAA3CCBlock"}}},
  {"Wall", {{"YMuonFilter"}, {}}},
  {"MCH", {MCHVolumes, {}}},
  {"MTR", {MTRVolumes, {}}},
  /** FORWARD DETECTORS **/
  {"FMD", {{"F1MT", "F2MT", "F3MT"}, {"FMD2_flange", "F3SU"}}},
  {"VZERO", {{"VERO"}, {}}} 
};

static std::map<std::string, bool> visibleFlag = {
  /** CENTRAL BARREL **/
  {"Solenoid", false},
  {"Doors", false},
  {"Frame", false},
  {"TRD",   false},
  {"TOF",   false},
  /** MUON SPECTROMETER **/
  {"Dipole", false},
  {"Absorber", true},
  {"Wall", false},
  {"MCH", true},
  {"MTR", false}
};

/******************************************************************************/

void init();
bool loadGeometry(const char *name = "geometry.root");
void cleanMaster();
bool drawVolume(const char *name);

void daughterVolumes(TGeoVolume *volume, std::set<std::string> &daughters);

void visibleVolume(const std::string &name, bool visible = true, const stringVector &exclude = stringVector());
void visibleVolume(TGeoVolume *volume, bool visible = true, const stringVector &exclude = stringVector());
void visibleStructure(const std::string &name, bool visible = true, bool excluded = false);
void printStructures();

/******************************************************************************/

void
printStructures()
{
  for (const auto &x : structures) std::cout << x.first << std::endl;
}

/******************************************************************************/

bool
loadGeometry(const char *name)
{
  auto fin = TFile::Open("geometry.root");
  auto geo = dynamic_cast<TGeoManager *>(fin->Get("ALICE"));
  //  geo->SetVisLevel(3);
  return true;
}

/******************************************************************************/

bool
drawVolume(const char *name)
{
  if (!gGeoManager) loadGeometry();
  auto volume = gGeoManager->GetVolume(name);
  if (!volume) return false;
  volume->Draw("ogl");
  return true;
}

/******************************************************************************/

void
daughterVolumes(TGeoVolume *volume, std::set<std::string> &daughters)
{
  for (int inode = 0; inode < volume->GetNdaughters(); inode++){
    auto node = volume->GetNode(inode);
    auto volume = node->GetVolume();
    daughters.insert(volume->GetName());
    daughterVolumes(volume, daughters);
  }
}

/******************************************************************************/

void
visibleVolume(const std::string &name, bool visible, const stringVector &exclude)
{
  if (!gGeoManager) loadGeometry();
  auto volume = gGeoManager->GetVolume(name.c_str());
  if (!volume) {
    std::cout << "Invalid volume: " << name << std::endl;
    return;
  }
  visibleVolume(volume, visible, exclude);
}

void
visibleVolume(TGeoVolume *volume, bool visible, const stringVector &exclude)
{
  volume->SetVisibility(visible);
  for (int inode = 0; inode < volume->GetNdaughters(); inode++){
    auto node = volume->GetNode(inode);
    auto daughter = node->GetVolume();
    if (std::find(exclude.begin(), exclude.end(), daughter->GetName()) != exclude.end()) continue;
    visibleVolume(daughter, visible, exclude);
  }
}

/******************************************************************************/

void
visibleStructure(const std::string &name, bool visible, bool excluded)
{
  if (structures.count(name) == 0) {
    std::cout << "Invalid structure: " << name << std::endl;
    return;
  }
  for (const auto &x : excluded ? structures.at(name).second : structures.at(name).first)
    visibleVolume(x, visible, excluded ? stringVector() : structures.at(name).second);
}

/******************************************************************************/

void
cleanMaster()
{
  if (!gGeoManager) loadGeometry();
  auto volumes = gGeoManager->GetListOfVolumes();
  auto nVolumes = volumes->GetEntries();
  for (Int_t ivol = 0; ivol < nVolumes; ivol++) {
    auto volume = (TGeoVolume *)volumes->At(ivol);
    volume->SetVisibility(false);
  }
  for (Int_t ivol = 0; ivol < nVolumes; ivol++) {
    auto volume = (TGeoVolume *)volumes->At(ivol);
    volume->SetVisDaughters(true);
  }
}

/******************************************************************************/

void
init()
{
  cleanMaster();
  visibleStructure("Solenoid");
  visibleStructure("Doors", false);
  visibleStructure("Pipe");
  visibleStructure("Dipole");
  visibleStructure("Absorber");
  //  visibleStructure("Frame");
}

/******************************************************************************/

bool
drawMaster()
{
  if (!gGeoManager) loadGeometry();
  
  /** master **/
  auto master = gGeoManager->GetMasterVolume();

  /** solenoid **/
  for (auto x : {"L3MO"})
    visibleVolume(x, visibleFlag["Solenoid"]);

  /** solenoid doors **/
  for (auto x : {"L3DoorL", "L3DoorR", "L3PlugSPL", "L3PlugSPR"})
    visibleVolume(x, visibleFlag["Doors"]);

  /** inner frame **/
  for (auto x : {"B077", "BRS1", "BRS4", "BFMO", "BBMO"})
    visibleVolume(x, visibleFlag["Frame"]);
  
  /** TRD **/
  for (auto x : {"BFTRD", "BBTRD"})
    visibleVolume(x, visibleFlag["TRD"]);
  for (int isector = 0; isector < 18; isector++) {
    auto volume = gGeoManager->GetVolume(Form("BTRD%d", isector));
    volume->SetVisibility(visibleFlag["TRD"]);
    volume->SetVisDaughters(visibleFlag["TRD"]);
  }
  
  /** TOF **/
  for (auto x : {"FTOS"})
    visibleVolume(x, visibleFlag["TOF"]);
  for (int isector = 0; isector < 18; isector++) {
    auto volume = gGeoManager->GetVolume(Form("BTOF%d", isector));
    volume->SetVisibility(visibleFlag["TOF"]);
    volume->SetVisDaughters(visibleFlag["TOF"]);
  }

  /** MUON SPECTROMETER **/

  /** dipole **/
  for (auto x : {"Dipole"})
    visibleVolume(x, visibleFlag["Dipole"]);

  /** front absorber **/
  for (auto x : {"AFA", "YSAA"})
    visibleVolume(x, visibleFlag["Absorber"]);
  
  /** muon chambers **/
  for (auto x : {"YOUT1", "YOUT2"})
    visibleVolume(x, visibleFlag["MCH"]);

  /** muon wall **/
  for (auto x : {"YMuonFilterH"})
    visibleVolume(x, visibleFlag["Wall"]);
  
  
  /** draw master **/
  master->Draw("ogl");

  return true;
}
