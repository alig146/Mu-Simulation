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
// G4MuonDecayChannel class implementation
//
// Author: H.Kurashige, 30 May 1997
// Contributions:
// - 2005 - M.Melissas, J.Brunner CPPM/IN2P3
//   Added V-A fluxes for neutrinos using a new algorithm, 2005
// --------------------------------------------------------------------

#include "G4ParticleDefinition.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4DecayProducts.hh"
#include "G4VDecayChannel.hh"
#include "FiveBodyMuonDecayChannel.hh"
#include "G4LorentzVector.hh"
#include "G4LorentzRotation.hh"
#include "action.hh"
namespace MATHUSLA { namespace MU {

FiveBodyMuonDecayChannel::FiveBodyMuonDecayChannel()
  : G4VDecayChannel(), fData()
{
}

FiveBodyMuonDecayChannel::FiveBodyMuonDecayChannel(const G4String& theParentName, 
                                             G4double theBR, MuonDataController* dataController)
  : G4VDecayChannel("Muon Decay", 1),fData()
{
fData = dataController;

  // set names for daughter particles
  if (theParentName == "mu+")
  {
    SetBR(theBR);
    SetParent("mu+");
    SetNumberOfDaughters(3);
    SetDaughter(0, "e-");
    SetDaughter(1, "e+");
    SetDaughter(2, "e+");
  }
  else if (theParentName == "mu-")
  {
    SetBR(theBR);
    SetParent("mu-");
    SetNumberOfDaughters(3);
    SetDaughter(0, "e+");
    SetDaughter(1, "e-");
    SetDaughter(2, "e-");
  }
  else
  {
#ifdef G4VERBOSE
    if (GetVerboseLevel()>0)
    {
      G4cout << "FiveBodyMuonDecayChannel:: constructor :";
      G4cout << " parent particle is not muon but ";
      G4cout << theParentName << G4endl;
    }
#endif
  }
}

FiveBodyMuonDecayChannel::FiveBodyMuonDecayChannel(const FiveBodyMuonDecayChannel& right)
  : G4VDecayChannel(right)
{
}

FiveBodyMuonDecayChannel::~FiveBodyMuonDecayChannel()
{
}

FiveBodyMuonDecayChannel&
FiveBodyMuonDecayChannel::operator=(const FiveBodyMuonDecayChannel& right)
{
  if (this != &right)
  { 
    kinematics_name = right.kinematics_name;
    verboseLevel = right.verboseLevel;
    rbranch = right.rbranch;

    // copy parent name
    parent_name = new G4String(*right.parent_name);

    // clear daughters_name array
    ClearDaughtersName();

    // recreate array
    numberOfDaughters = right.numberOfDaughters;
    if ( numberOfDaughters >0 )
    {
      if (daughters_name !=0) ClearDaughtersName();
      daughters_name = new G4String*[numberOfDaughters];
      // copy daughters name
      for (G4int index=0; index < numberOfDaughters; ++index)
      {
        daughters_name[index] = new G4String(*right.daughters_name[index]);
      }
    }
  }
  return *this;
}

G4DecayProducts* FiveBodyMuonDecayChannel::DecayIt(G4double) 
{
  // this version neglects muon polarization,and electron mass  
  //              assumes the pure V-A coupling
  //              the Neutrinos are correctly V-A

#ifdef G4VERBOSE
  if (GetVerboseLevel()>1) G4cout << "FiveBodyMuonDecayChannel::DecayIt ";
#endif

  CheckAndFillParent();
  CheckAndFillDaughters();
 
  // parent mass
  G4double parentmass = G4MT_parent->GetPDGMass();
  const G4int N_DAUGHTER=3;

  // daughters'mass
  G4double daughtermass[N_DAUGHTER]; 
  G4double sumofdaughtermass = 0.0;
  for (G4int index=0; index<N_DAUGHTER; ++index)
  {
    daughtermass[index] = G4MT_daughters[index]->GetPDGMass();
    sumofdaughtermass += daughtermass[index];
  }

  // create parent G4DynamicParticle at rest
  G4ThreeVector dummy;
  G4DynamicParticle* parentparticle
    = new G4DynamicParticle( G4MT_parent, dummy, 0.0);
  // create G4Decayproducts
  G4DecayProducts *products = new G4DecayProducts(*parentparticle);
  delete parentparticle;

  // calculate daughter momentum
  G4ThreeVector daughtermomentum[N_DAUGHTER];
  // calculate electron energy
  
//Define parser

G4double insertParticles[9];
G4bool random = fData->getRandom();
G4int muonDecays = fData->getMuonDecays();

if(random){
fData->getRandomParticles(insertParticles);
}else{
fData->getParticles(muonDecays,insertParticles);
}
fData->incrementMuonDecays();


  
  // Positron 0

  daughtermomentum[0]=G4ThreeVector(insertParticles[0],insertParticles[1],insertParticles[2]);

  G4DynamicParticle* daughterparticle
    = new G4DynamicParticle (G4MT_daughters[0], daughtermomentum[0]);

  products->PushProducts(daughterparticle);
  
  // Electron  1

  daughtermomentum[1] =G4ThreeVector(insertParticles[3],insertParticles[4],insertParticles[5]);

  G4DynamicParticle* daughterparticle1
    = new G4DynamicParticle (G4MT_daughters[1], daughtermomentum[1]);
  products->PushProducts(daughterparticle1);

  // Electron 2
  
  daughtermomentum[2] =G4ThreeVector(insertParticles[6],insertParticles[7],insertParticles[8]);

  G4DynamicParticle* daughterparticle2
    = new G4DynamicParticle (G4MT_daughters[2], daughtermomentum[2]);
  products->PushProducts(daughterparticle2);

  // output message
#ifdef G4VERBOSE
  if (GetVerboseLevel()>1)
  {
    G4cout << "FiveBodyMuonDecayChannel::DecayIt()";
    G4cout << " create decay products in rest frame " << G4endl;
    products->DumpInfo();
  }
#endif
  return products;
}


}}
