
#ifndef FiveBodyDataController_h
#define FiveBodyDataController_h 1

#include <iostream>
#include <action.hh>
#include <stdlib.h>
#include "globals.hh"


namespace MATHUSLA { namespace MU {

class MuonDataController
{
public:
    MuonDataController();
    virtual ~MuonDataController();
    
    static MuonDataController* getMuonDataController();
    virtual void setDecayInEvent(G4bool);
    virtual G4bool getDecayInEvent();   

    std::vector<G4double> p1x, p1y, p1z, p2x, p2y, p2z, p3x, p3y, p3z;
    void getParticles(G4int, G4double*);
    void getRandomParticles(G4double*);
    void incrementMuonDecays();
    virtual G4bool getRandom();
    virtual G4int getMuonDecays();
    virtual void setRandom(G4bool);
    virtual void setMuonDecays(G4int);        
private:
   EventAction* eventAction;
   static MuonDataController* sController;
   G4int i =0;
   G4bool Random;
   G4int MuonDecays =0;
   G4String e1x, e1y, e1z, e2x, e2y, e2z, e3x, e3y, e3z;
   G4bool DecayInEvent;

};
}}
#endif
