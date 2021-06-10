#include <iostream>
#include <string>
#include <fstream>

#include <vector>
#include "HistoManager.hh"
#include "PersonalizedSteppingAction.hh"
#include "PersonalizedRunAction.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SteppingManager.hh"
#include "globals.hh"
#include "G4UserSteppingAction.hh"
#include "G4SystemOfUnits.hh"

PersonalizedSteppingAction::PersonalizedSteppingAction()
        : G4UserSteppingAction()
{;}
PersonalizedSteppingAction::~PersonalizedSteppingAction() {;}

void PersonalizedSteppingAction::UserSteppingAction(const G4Step * theStep) {
    G4Track *theTrack = theStep->GetTrack();

//Variable Definitions

auto DecayPosition = theStep->GetPostStepPoint()->GetPosition();
Z_Value = theStep->GetPostStepPoint()->GetPosition().getX();
X_Value = theStep->GetPostStepPoint()->GetPosition().getY();
Y_Value =-(theStep->GetPostStepPoint()->GetPosition().getZ())+80*m;
auto ParticleType=theStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding();

auto PVector = theStep->GetPostStepPoint()->GetMomentum();
double P_Z=PVector.getX();
double P_X=PVector.getY();
double P_Y=-PVector.getZ();
double P_Magnitude=sqrt(pow(P_Z,2)+pow(P_Y,2)+pow(P_X,2));
 
//if the particle is a Muon
	if(ParticleType == 13 || ParticleType ==-13){

  if (theTrack->GetTrackStatus() == fStopAndKill) {
G4cout<<"Momentum value: "<<PVector<<" Magnitude: "<<P_Magnitude<<" fStopAndKill"<<G4endl;

}else{

G4cout<<"Momentum value: "<<PVector<<" Magnitude: "<<P_Magnitude<<G4endl;
}; 

//     <<" Track Status "<<theTrack->GetTrackStatus()
  //       <<" Particle Definition "<<theStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding()<<G4endl;

 return;
}
}




