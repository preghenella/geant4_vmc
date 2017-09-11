//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex01MCApplication.cxx
/// \brief Implementation of the Ex01MCApplication class 
///
/// Geant4 ExampleN01 adapted to Virtual Monte Carlo \n
///
/// \date 05/04/2002
/// \author I. Hrivnacova; IPN, Orsay

#include "PluginMCApplication.h"
#include "Ex01MCStack.h"
#include "Ex01DetectorConstructionOld.h"

#include <TROOT.h>
#include <Riostream.h>
#include <TInterpreter.h>
#include <TVirtualMC.h>
#include <TLorentzVector.h>
#include <TArrayD.h>
#include <TGeoManager.h>
#include <TGeoMatrix.h>
#include <TGeoMaterial.h>
#include <TThread.h>

using namespace std;

/// \cond CLASSIMP
ClassImp(PluginMCApplication)
/// \endcond

//_____________________________________________________________________________
PluginMCApplication::PluginMCApplication(const char *name, const char *title) 
  : TVirtualMCApplication(name,title),
    fStack(0),
    fMagField(0)
{
/// Standard constructor
/// \param name   The MC application name 
/// \param title  The MC application description

  // create a user stack
  fStack = new Ex01MCStack(100);  
  
  // create magnetic field (with zero value)
  fMagField = new TGeoUniformMagField();
}

//_____________________________________________________________________________
PluginMCApplication::PluginMCApplication()
  : TVirtualMCApplication(),
    fStack(0),
    fMagField(0)
{    
/// Default constructor
}

//_____________________________________________________________________________
PluginMCApplication::~PluginMCApplication() 
{
/// Destructor  

  delete fStack;
  delete fMagField;
  delete gMC;
}

//
// public methods
//

//_____________________________________________________________________________
void PluginMCApplication::InitMC(const char* setup)
{    
/// Initialize MC.
/// The selection of the concrete MC is done in the macro.
/// \param setup The name of the configuration macro 

  if ( TString(setup) != "" ) {
    gROOT->LoadMacro(setup);
    gInterpreter->ProcessLine("Config()");
    if ( ! gMC ) {
      Fatal("InitMC",
            "Processing Config() has failed. (No MC is instantiated.)");
    }
  }

  gROOT->LoadMacro("CustomGen.C");
  
  gMC->SetStack(fStack);
  gMC->SetMagField(fMagField);
  gMC->Init();
  gMC->BuildPhysics();  
}

//__________________________________________________________________________
void PluginMCApplication::RunMC(Int_t nofEvents)
{    
/// Run MC.
/// \param nofEvents Number of events to be processed

  gMC->ProcessRun(nofEvents);
  FinishRun();
}

//_____________________________________________________________________________
void PluginMCApplication::FinishRun()
{    
/// Finish MC run.
}

//_____________________________________________________________________________
void PluginMCApplication::ConstructGeometry()
{    
/// Construct geometry using TGeo functions or
/// TVirtualMC functions (if oldGeometry is selected)

  gROOT->LoadMacro("CustomGeo.C");
  gInterpreter->ProcessLine("CustomGeo()");
  gGeoManager->CloseGeometry();
  gMC->SetRootGeometry();
}

//_____________________________________________________________________________
void PluginMCApplication::InitGeometry()
{    
/// Initialize geometry.
}

//_____________________________________________________________________________
void PluginMCApplication::GeneratePrimaries()
{    
/// Fill the user stack (derived from TVirtualMCStack) with primary particles.
  
  gInterpreter->ProcessLine("CustomGen()");
}

//_____________________________________________________________________________
void PluginMCApplication::BeginEvent()
{    
/// User actions at beginning of event.
/// Nothing to be done this example
}

//_____________________________________________________________________________
void PluginMCApplication::BeginPrimary()
{    
/// User actions at beginning of a primary track.
/// Nothing to be done this example
}

//_____________________________________________________________________________
void PluginMCApplication::PreTrack()
{    
/// User actions at beginning of each track.
/// Print info message.
}

//_____________________________________________________________________________
void PluginMCApplication::Stepping()
{    
/// User actions at each step.
/// Print track position, the current volume and current medium names.
}

//_____________________________________________________________________________
void PluginMCApplication::PostTrack()
{    
/// User actions after finishing of each track
/// Nothing to be done this example
}

//_____________________________________________________________________________
void PluginMCApplication::FinishPrimary()
{    
/// User actions after finishing of a primary track.
/// Nothing to be done this example
}

//_____________________________________________________________________________
void PluginMCApplication::FinishEvent()
{    
/// User actions after finishing of an event
/// Nothing to be done this example
} 
