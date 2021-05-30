#include "PhysicsList.hh"

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4ParticleTypes.hh>
#include <G4ParticleTable.hh>
#include <G4SystemOfUnits.hh>
#include <G4RegionStore.hh>
#include <G4ProductionCuts.hh>
#include "G4PhysicsListHelper.hh"

#include <G4ComptonScattering.hh>
#include <G4GammaConversion.hh>
#include <G4PhotoElectricEffect.hh>

#include <G4eMultipleScattering.hh>
#include <G4eIonisation.hh>
#include <G4eBremsstrahlung.hh>
#include <G4eplusAnnihilation.hh>

#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"

// Bosons
#include "G4ChargedGeantino.hh"
#include "G4Geantino.hh"
#include "G4Gamma.hh"

// leptons
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"

// Mesons
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"

#include "G4KaonPlus.hh"
#include "G4KaonMinus.hh"

// Baryons
#include "G4Proton.hh"
#include "G4AntiProton.hh"
#include "G4Neutron.hh"
#include "G4AntiNeutron.hh"

// Nuclei
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4Alpha.hh"
#include "G4GenericIon.hh"




PhysicsList::PhysicsList()
: G4VUserPhysicsList()
{
    defaultCutValue = 1.0*mm;
    SetVerboseLevel(1);
}

PhysicsList::~PhysicsList()
{ }

////////////////////////////////////////////////
void PhysicsList::ConstructParticle()
{

// gamma
  G4Gamma::GammaDefinition();

// leptons
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();

// mesons
  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
  G4KaonPlus::KaonPlusDefinition();
  G4KaonMinus::KaonMinusDefinition();

// baryons
  G4Proton::ProtonDefinition();
  G4AntiProton::AntiProtonDefinition();
  G4Neutron::NeutronDefinition();
  G4AntiNeutron::AntiNeutronDefinition();

// ions
  G4Deuteron::DeuteronDefinition();
  G4Triton::TritonDefinition();
  G4Alpha::AlphaDefinition();
  G4GenericIon::GenericIonDefinition();
}


//////////////////////////////////////////////
void PhysicsList::ConstructProcess()
{
    AddTransportation();
    ConstructEM();
}

void PhysicsList::ConstructEM()
{
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

  auto particleIterator=GetParticleIterator();
  particleIterator->reset();
  while( (*particleIterator)() ){
    G4ParticleDefinition* particle = particleIterator->value();
    G4String particleName = particle->GetParticleName();

    if (particleName == "gamma") {
      ph->RegisterProcess(new G4PhotoElectricEffect, particle);
      ph->RegisterProcess(new G4ComptonScattering,   particle);
      ph->RegisterProcess(new G4GammaConversion,     particle);

    } else if (particleName == "e-") {
      ph->RegisterProcess(new G4eMultipleScattering, particle);
      ph->RegisterProcess(new G4eIonisation,         particle);
      ph->RegisterProcess(new G4eBremsstrahlung,     particle);

    } else if (particleName == "e+") {
      ph->RegisterProcess(new G4eMultipleScattering, particle);
      ph->RegisterProcess(new G4eIonisation,         particle);
      ph->RegisterProcess(new G4eBremsstrahlung,     particle);
      ph->RegisterProcess(new G4eplusAnnihilation,   particle);
    } else if (particleName == "mu-" || particleName == "mu+") {
        ph->RegisterProcess(new G4MuMultipleScattering, particle);
        ph->RegisterProcess(new G4MuIonisation,         particle);
        ph->RegisterProcess(new G4MuBremsstrahlung,     particle);
        ph->RegisterProcess(new G4MuPairProduction,     particle);
    } else if (particleName == "proton") {
      ph->RegisterProcess(new G4hMultipleScattering, particle);
      ph->RegisterProcess(new G4hIonisation,         particle);
      ph->RegisterProcess(new G4hBremsstrahlung,     particle);
      ph->RegisterProcess(new G4hPairProduction,   particle);
    }
  }
}

////////////////////////////////////////////////////////////////////////////
void PhysicsList::SetCuts() {
    // default production thresholds for the world volume
    SetCutsWithDefault();

    // Production thresholds for earth regions
    G4Region* region;
    G4String regName;
    G4ProductionCuts* cuts;

    regName = "Earth_Cut_Region";
    region = G4RegionStore::GetInstance()->GetRegion(regName);
    cuts = new G4ProductionCuts;
    cuts->SetProductionCut(1.0*mm,G4ProductionCuts::GetIndex("gamma"));
    cuts->SetProductionCut(1.0*mm,G4ProductionCuts::GetIndex("e-"));
    cuts->SetProductionCut(1.0*mm,G4ProductionCuts::GetIndex("e+"));
    cuts->SetProductionCut(1.0*mm,G4ProductionCuts::GetIndex("proton"));
    region->SetProductionCuts(cuts);
}
