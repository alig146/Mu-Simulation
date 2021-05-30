//
// Created by grace on 5/14/2021.
//

#ifndef PERSONALIZEDSTEPPINGACTION_PERSONALIZEDSTEPPINGACTION_HH
#define PERSONALIZEDSTEPPINGACTION_PERSONALIZEDSTEPPINGACTION_HH
#include "globals.hh"
#include "G4UserSteppingAction.hh"


class PersonalizedRunAction;
class HistoManager;

class PersonalizedSteppingAction : public G4UserSteppingAction
{
public:
    PersonalizedSteppingAction();
    virtual ~PersonalizedSteppingAction();

    virtual void UserSteppingAction(const G4Step*);
private:
G4double X_Value;
G4double Y_Value;
G4double Z_Value;

};
#endif //PERSONALIZEDSTEPPINGACTION_PERSONALIZEDSTEPPINGACTION_HH
