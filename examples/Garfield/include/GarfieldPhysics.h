#ifndef GARFIELDMODELCONFIG_HH_
#define GARFIELDMODELCONFIG_HH_

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  Garfield/include/GarfieldPhysics.h
/// \brief Definition of the GarfieldPhysics class 
///
/// Garfield garfieldpp example adapted to Virtual Monte Carlo.
/// This class is imported from garfieldpp example with no modification
/// in the code.
///
/// \date 28/10/2015
/// \author D. Pheiffer, CERN

/*
 * GarfieldPhysics.hh
 *
 *  Created on: Jul 17, 2014
 *      Author: dpfeiffe
 */

#include <map>
#include <vector>
#include <iostream>

#include "Sensor.hh"
#include "AvalancheMC.hh"
#include "AvalancheMicroscopic.hh"
#include "ComponentAnalyticField.hh"
#include "TrackHeed.hh"
#include "TrackSimple.hh"
#include "MediumMagboltz.hh"
#include "GeometryRoot.hh"
#include "GeometrySimple.hh"
#include "SolidTube.hh"

typedef std::pair<double, double> EnergyRange_keV;
typedef std::map< const std::string, EnergyRange_keV> MapParticlesEnergy;

class GarfieldElectron{
public:
	GarfieldElectron( double ekin_eV,double time, double x_cm,double y_cm,double z_cm, double dx,double dy,double dz):fEkin_MeV(ekin_eV/1000000), fTime(time), fx_mm(10*x_cm),fy_mm(10*y_cm), fz_mm(10*z_cm),  fdx(dx), fdy(dy), fdz(dz){}
	~GarfieldElectron(){};

	double getX_mm() {return fx_mm;}
	double getY_mm(){return fy_mm;}
	double getZ_mm(){return fz_mm;}
	double getEkin_MeV(){return fEkin_MeV;}
	double getTime(){return fTime;}
	double getDX(){return fdx;}
	double getDY(){return fdy;}
	double getDZ(){return fdz;}


private:
	double fEkin_MeV, fTime, fx_mm,fy_mm,fz_mm,fdx,fdy,fdz;

};

class GarfieldPhysics {
public:
	static GarfieldPhysics* GetInstance();
	static void Dispose();

	void InitializePhysics();
	void CreateGeometry();

	void DoIt(std::string particleName, double ekin_keV,double time,
			double x_cm, double y_cm, double z_cm, double dx, double dy, double dz);

	void AddParticleName(const std::string particleName, double ekin_min_keV, double ekin_max_keV);
	bool FindParticleName(const std::string name);
	bool FindParticleNameEnergy(std::string name, double ekin_keV);
	std::vector<GarfieldElectron*>* GetSecondaryElectrons();
	void DeleteSecondaryElectrons();
	inline void EnableCreateSecondariesInGeant4() {createSecondariesInGeant4 = true;};
	inline void DisableCreateSecondariesInGeant4() {createSecondariesInGeant4 = false;};
	inline bool GetCreateSecondariesInGeant4() {return createSecondariesInGeant4;};
	inline double GetEnergyDeposit_MeV() {return fEnergyDeposit/1000000;};
	inline double GetAvalancheSize() {return fAvalancheSize;};
	inline double GetGain() {return fGain;};
private:
	GarfieldPhysics();
	~GarfieldPhysics();


	static GarfieldPhysics* fGarfieldPhysics;
	MapParticlesEnergy* fMapParticlesEnergy;
	TGeoManager* fGeoManager;
	Garfield::MediumMagboltz* fMediumMagboltz;
	Garfield::Sensor* fSensor;
	Garfield::AvalancheMC* fDrift;
	Garfield::AvalancheMicroscopic* fAvalanche;
	Garfield::TrackHeed* fTrackHeed;
	Garfield::GeometryRoot* fGeometryRoot;
	Garfield::GeometrySimple* fGeometrySimple;
	Garfield::ComponentAnalyticField* fComponentAnalyticField;
	Garfield::SolidTube* fTube;

	std::vector<GarfieldElectron*>* fSecondaryElectrons;

	bool createSecondariesInGeant4;
	double fEnergyDeposit;
	double fAvalancheSize;
	double fGain;

};
#endif /* GARFIELDMODELCONFIG_HH_ */