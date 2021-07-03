
//
//-----------------------------------------------------------------
//
// This code is a implementation of G4 example GB01
//
//-----------------------------------------------------------------
//

#ifndef MultiParticleChangeCrossSection_hh
#define MultiParticleChangeCrossSection_hh 1

#include "G4VBiasingOperator.hh"
class ChangeCrossSection;
class G4ParticleDefinition;

#include <map>

class MultiParticleChangeCrossSection : public G4VBiasingOperator {
public:
  MultiParticleChangeCrossSection();
  virtual ~MultiParticleChangeCrossSection() {}

  // ---------------------------------
  // -- Method specific to this class:
  // ---------------------------------
  // -- Each particle type for which its name is passed will be biased; *provided*
  // -- that the proper calls to biasingPhysics->Bias(particleName) have been done
  // -- in the main program.
  void AddParticle( G4String particleName );


private:
  // -----------------------------
  // -- Mandatory from base class:
  // -----------------------------
  // -- This method returns a biasing operation that will bias the physics process occurence:
  virtual G4VBiasingOperation*
  ProposeOccurenceBiasingOperation(const G4Track*                            track,
                                   const G4BiasingProcessInterface* callingProcess);
  // -- Methods not used:
  virtual G4VBiasingOperation*
  ProposeFinalStateBiasingOperation(const G4Track*, const G4BiasingProcessInterface*)
  {return 0;}
  virtual G4VBiasingOperation*
  ProposeNonPhysicsBiasingOperation(const G4Track*, const G4BiasingProcessInterface*)
  {return 0;}


private:
  // -- ("using" is to avoid compiler complaining against (false) method shadowing.)
    using G4VBiasingOperator::OperationApplied;

  // -- Optionnal base class method implementation.
  // -- This method is called to inform the operator that a proposed operation has been applied.
  // -- In the present case, it means that a physical interaction occured (interaction at
  // -- PostStepDoIt level):
  virtual void OperationApplied( const G4BiasingProcessInterface*                callingProcess,
                                 G4BiasingAppliedCase                               biasingCase,
                                 G4VBiasingOperation*                 occurenceOperationApplied,
                                 G4double                         weightForOccurenceInteraction,
                                 G4VBiasingOperation*                finalStateOperationApplied,
                                 const G4VParticleChange*                particleChangeProduced );

public:
  // -- Optionnal base class method. It is called at the time a tracking of a particle starts:
  void StartTracking( const G4Track* track );

private:
  // -- List of associations between particle types and biasing operators:
  std::map < const G4ParticleDefinition*, ChangeCrossSection* >    fBOptrForParticle;
  std::vector < const G4ParticleDefinition* >   fParticlesToBias;
  ChangeCrossSection*                  fCurrentOperator;

  // -- count number of biased interations for current track:
  G4int fnInteractions;
};

#endif
