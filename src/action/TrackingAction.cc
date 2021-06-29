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

#include "action.hh"


namespace MATHUSLA { namespace MU {

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::TrackingAction()
:G4UserTrackingAction()
 
{
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::~TrackingAction()
{
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PostUserTrackingAction(const G4Track* track)
{

   //Make sure 5-body muon decays are turned on
   if(ActionInitialization::Five != true){return;}
   auto ParticleType=track->GetParticleDefinition()->GetPDGEncoding();
   
   //Determine that the track is a muon or anti-muon
   if((ParticleType !=13) && (ParticleType != -13)){return;} 

   const std::vector<const G4Track*>* secondariesInCurrentStep
                              = track->GetStep()->GetSecondaryInCurrentStep();
   size_t nbtrkCurrent = (*secondariesInCurrentStep).size();

   //Determine that a three body decay has occured
   if(nbtrkCurrent !=3){return;}
   
   //kill secondaries from 3 body decay  
   for(int i=0; i<nbtrkCurrent; i++){
   const G4Track* strack = secondariesInCurrentStep->at(i);
   auto sname = strack->GetDefinition()->GetParticleName();
  
   G4Track* str = (G4Track*) strack;
   str->SetTrackStatus(fStopAndKill);}
   
   //Information for inserting 5-Body Decay
   auto muonMomentum = track->GetMomentum();
   auto XMuonMomentum = muonMomentum.getY();
   auto YMuonMomentum = -(muonMomentum.getZ())+80*m;
   auto ZMuonMomentum = muonMomentum.getX();
   auto muonEnergy = track->GetTotalEnergy();
   
   //anti will flip secondaries based on muon or anti-muon
   G4int anti = 1;
   if(ParticleType == 13){anti = -1;}
   
   //make 4-vector for muon
   CLHEP::HepLorentzVector muon4Momentum = CLHEP::HepLorentzVector(muonMomentum,muonEnergy);
   
   //Get secondary particles data

   //Make secondary particle 4 momentum vectors
      
   //Rotate secondary particles along correct axis
    
   //Boost secondary particles to correct frame

   //Create tracks with this data
   

}
 


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
}}
