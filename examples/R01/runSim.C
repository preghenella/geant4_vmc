//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E01
/// \file E01/run_g3.C
/// \brief Macro for running Example01 with Geant3 

#include "TVirtualMC.h"

void runSim(const TString& configMacro = "g3tgeoConfig.C") 
{
/// Macro function for running Example01 with Geant3 from
/// Root interactive session.
/// Note that since Root 6 the libraries have to be loaded first
/// via load_g3.C.
/// \param configMacro configuration macro name, default \ref E01/g3Config.C 

  // MC application
  PluginMCApplication* appl 
    = new PluginMCApplication("Example01", "The example01 MC application");

  // Initialize MC
  appl->InitMC(configMacro);
  gMC->SetCollectTracks(kTRUE);
  
  // Run MC
  appl->RunMC(1);

  // Draw 
  gGeoManager->DefaultColors();
  gGeoManager->SetVisOption(0);
  gGeoManager->DrawTracks("/*");
  
  delete appl;
}  
