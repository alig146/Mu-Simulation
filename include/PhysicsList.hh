#ifndef PhysicsList_h
#define PhysicsList_h 1

#include <G4VUserPhysicsList.hh>
#include "globals.hh"



class PhysicsList: public G4VUserPhysicsList
{
public:
    PhysicsList();
    ~PhysicsList();

protected:
  virtual void ConstructParticle();
  virtual void ConstructProcess();
  virtual void SetCuts();


protected:
  void ConstructEM();

};






#endif 
