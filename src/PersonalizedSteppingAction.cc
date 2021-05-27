#include <iostream>
#include <string>
#include <fstream>

#include <vector>
#include "HistoManager.hh"
#include "PersonalizedSteppingAction.hh"
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
G4double X_Value = theStep->GetPostStepPoint()->GetPosition().getX();
G4double Y_Value = theStep->GetPostStepPoint()->GetPosition().getY();
G4double Z_Value = theStep->GetPostStepPoint()->GetPosition().getZ();
auto ParticleType=theStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding();

//if the track is about to decay
    if (theTrack->GetTrackStatus() == fStopAndKill) { 
//if the particle is a Muon
	if(ParticleType == 13 || ParticleType ==-13){
//message if a muon decays at all
G4cout<< ParticleType <<" Muon decays--PostStepPoint "<< DecayPosition <<G4endl;

G4AnalysisManager::Instance()->FillH1(0, X_Value);
G4AnalysisManager::Instance()->FillH1(1, Y_Value);
G4AnalysisManager::Instance()->FillH1(2, Z_Value);

//if the decay is inside the detector
		if((Z_Value > (7000*cm) && Z_Value < (16900*cm)) &&
(X_Value < (4950*cm) && X_Value > (-4950*cm)) &&
(Y_Value < (8917*cm)  && Y_Value > (6001*cm))) {

//message if muon decays inside the detector
G4cout<< ParticleType <<" Muon decays within detector--PostStepPoint "<< DecayPosition <<G4endl;  


//write decay position vectors to a vector array
//std::vector<double> decayLocation {X_Value,Y_Value,Z_Value};
//decayVectors.push_back(decayLocation);
 

//     <<" Track Status "<<theTrack->GetTrackStatus()
  //       <<" Particle Definition "<<theStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding()<<G4endl;
	}
 return;
    }
}
}




