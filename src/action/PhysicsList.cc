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
//
/// \file field/field04/src/F04PhysicsList.cc
/// \brief Implementation of the F04PhysicsList class
//

#include "PhysicsList.hh"

#include "G4StepLimiterPhysics.hh"
#include "G4OpticalPhysics.hh"

#include "G4LossTableManager.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

//#include "G4PhysListFactory.hh"
#include "FTFP_BERT.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"

//#include "StepMax.hh"

#include "G4ProcessTable.hh"

#include "G4DecayTable.hh"
#include "FiveBodyMuonDecayChannel.hh"

#include "G4MuonMinusCapture.hh"
#include "G4MuMinusCapturePrecompound.hh"

#include "MuonDataController.hh"
#include "G4SystemOfUnits.hh"
#include "G4AutoDelete.hh"
namespace MATHUSLA { namespace MU {
//G4ThreadLocal StepMax* PhysicsList::fStepMaxProcess = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList() : G4VModularPhysicsList(),fData()
{
    SetVerboseLevel(1);

//    G4PhysListFactory factory;
    G4VModularPhysicsList* phys = NULL;
       phys = new FTFP_BERT;

    if (!phys) G4Exception("F04PhysicsList::F04PhysicsList","InvalidSetup",
                              FatalException,"PhysicsList does not exist");

    for (G4int i = 0; ; ++i) {
       G4VPhysicsConstructor* elem =
                  const_cast<G4VPhysicsConstructor*> (phys->GetPhysics(i));
       if (elem == NULL) break;
       G4cout << "RegisterPhysics: " << elem->GetPhysicsName() << G4endl;
       RegisterPhysics(elem);
    }

    RegisterPhysics(new G4StepLimiterPhysics());
fData = new MuonDataController();
fData->setRandom(true);


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructParticle()
{

    G4VModularPhysicsList::ConstructParticle();

    G4GenericIon::GenericIonDefinition();

    G4DecayTable* muonPlusDecayTable = new G4DecayTable();
    muonPlusDecayTable -> Insert(new
                           FiveBodyMuonDecayChannel("mu+",1.00,fData));
    G4MuonPlus::MuonPlusDefinition() -> SetDecayTable(muonPlusDecayTable);

    G4DecayTable* muonMinusDecayTable = new G4DecayTable();
    muonMinusDecayTable -> Insert(new
                            FiveBodyMuonDecayChannel("mu-",1.00,fData));
    G4MuonMinus::MuonMinusDefinition() -> SetDecayTable(muonMinusDecayTable);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 

}}
