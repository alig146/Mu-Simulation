//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file TrackingAction.cc
/// \brief Implementation of the TrackingAction class
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "MuonTrackingAction.hh"

#include "HistoManager.hh"

#include "G4Track.hh"
#include "G4ParticleTypes.hh"
#include "G4RunManager.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include <cmath>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuonTrackingAction::MuonTrackingAction()
:G4UserTrackingAction()
 
{
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuonTrackingAction::~MuonTrackingAction()
{
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuonTrackingAction::PostUserTrackingAction(const G4Track* track)
{  

auto CurrentPosition = track->GetPosition();
double Z_Value = CurrentPosition.getX();
double X_Value = CurrentPosition.getY();
double Y_Value =-(CurrentPosition.getZ())+80*m;
auto ParticleType=track->GetParticleDefinition()->GetPDGEncoding();
auto PVector=track->GetMomentum();
double P_Z=PVector.getX();
double P_X=PVector.getY();
double P_Y=-PVector.getZ();
double P_Magnitude=sqrt(pow(P_Z,2)+pow(P_Y,2)+pow(P_X,2));

if(ParticleType == 13 || ParticleType ==-13){     
 
   const std::vector<const G4Track*>* secondaries 
                              = track->GetStep()->GetSecondaryInCurrentStep();
   size_t nbtrk = (*secondaries).size();
   if (nbtrk) {
   	//there are secondaries --> it is a decay
 
   	//fill decay location plots
   	//1D
   	G4AnalysisManager::Instance()->FillH1(0, X_Value); //X-Values
   	G4AnalysisManager::Instance()->FillH1(1, Y_Value); //Y- Values
   	G4AnalysisManager::Instance()->FillH1(2, Z_Value); //Z- Values
   	//3D
   	G4AnalysisManager::Instance()->FillH3(0, X_Value, Y_Value, Z_Value); //3D decay
   	//2D
   	G4AnalysisManager::Instance()->FillH2(0, X_Value, Y_Value);
   	G4AnalysisManager::Instance()->FillH2(1,Y_Value,Z_Value);
    	G4AnalysisManager::Instance()->FillH2(2,Z_Value,X_Value);   

   	//Momentum Plots
   
   	G4AnalysisManager::Instance()->FillH1(3, P_Magnitude); //All decay momentum values
   
   	if (P_Magnitude != 0){
       		//1D Non-Zero Momentum
       		G4AnalysisManager::Instance()->FillH1(4, P_Magnitude);
       		//2D Decay Locations
       		G4AnalysisManager::Instance()->FillH2(3, X_Value, Y_Value);
       		G4AnalysisManager::Instance()->FillH2(4,Y_Value,Z_Value);
       		G4AnalysisManager::Instance()->FillH2(5,Z_Value,X_Value);
    
  
       		if((Z_Value > (7000*cm) && Z_Value < (16900*cm)) &&
       		(X_Value < (4950*cm) && X_Value > (-4950*cm)) &&
       		(Y_Value < (8917*cm)  && Y_Value > (6001*cm)))
       		{
          		//message if muon decays inside the detector
          		G4cout<< ParticleType <<" Non-zero momentum muon decays within detector "<< CurrentPosition <<G4endl;
       		}else{
          		//message if muon decays outside the detector
          		G4cout<< ParticleType <<" Non-zero momentum muon decays outside detector"<< CurrentPosition <<G4endl;
       		}

   	}else{
       		//2D Decay Locations Zero Momentum
       		G4AnalysisManager::Instance()->FillH2(6, X_Value, Y_Value);
       		G4AnalysisManager::Instance()->FillH2(7,Y_Value,Z_Value);
       		G4AnalysisManager::Instance()->FillH2(8,Z_Value,X_Value);
 
       		if((Z_Value > (7000*cm) && Z_Value < (16900*cm)) &&
       		(X_Value < (4950*cm) && X_Value > (-4950*cm)) &&
       		(Y_Value < (8917*cm)  && Y_Value > (6001*cm)))
       		{
          		//message if muon decays inside the detector
          		G4cout<< ParticleType <<" Zero momentum muon decays within detector "<< CurrentPosition <<G4endl;
       		}else{
          		//message if muon decays outside the detector
          		G4cout<< ParticleType <<" Zero momentum muon decays outside detector"<< CurrentPosition <<G4endl;
      	 	}	  
   	}   


 }

}  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

