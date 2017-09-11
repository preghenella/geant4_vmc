#ifndef PLUGIN_MC_APPLICATION_H
#define PLUGIN_MC_APPLICATION_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex01MCApplication.h
/// \brief Definition of the Ex01MCApplication class 
///
/// Geant4 ExampleN01 adapted to Virtual Monte Carlo
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TVirtualMCApplication.h>
#include <TVirtualMCStack.h>

class TVirtualMagField;

/// \ingroup E01
/// \brief Implementation of the TVirtualMCApplication
///
/// \date 05/04/2002
/// \author I. Hrivnacova; IPN, Orsay

class PluginMCApplication : public TVirtualMCApplication
{
  public:
    PluginMCApplication(const char *name, const char *title);
    PluginMCApplication();
    virtual ~PluginMCApplication();
  
    // methods
    void InitMC(const char *setup);
    void RunMC(Int_t nofEvents);
    void FinishRun();
            
    virtual void ConstructGeometry();
    virtual void InitGeometry();
    virtual void GeneratePrimaries();
    virtual void BeginEvent();
    virtual void BeginPrimary();
    virtual void PreTrack();
    virtual void Stepping();
    virtual void PostTrack();
    virtual void FinishPrimary();
    virtual void FinishEvent();

  private:
  
    // data members
    TVirtualMCStack *fStack;       ///< The VMC stack
    TVirtualMagField *fMagField;    ///< The magnetic field 

  ClassDef(PluginMCApplication,1)  //Interface to MonteCarlo application
};

#endif //PLUGIN_MC_APPLICATION_H

