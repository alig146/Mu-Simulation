/*
 * include/action.hh
 *
 * Copyright 2018 Brandon Gomes
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MU__ACTION_HH
#define MU__ACTION_HH
#pragma once

#include <G4VUserActionInitialization.hh>
#include <G4UserEventAction.hh>
#include <G4UserRunAction.hh>
#include <G4UserSteppingAction.hh>
#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4Event.hh>
#include <G4Run.hh>
#include "TROOT.h"
#include "TTree.h"

#include "physics/Generator.hh"
#include "ui.hh"

namespace MATHUSLA { namespace MU {

//__Geant4 Action Initializer___________________________________________________________________
class ActionInitialization : public G4VUserActionInitialization {
public:
  ActionInitialization(const std::string& generator="",
                       const std::string& data_dir="");
  void BuildForMaster() const;
  void Build() const;
  static bool Debug;
};
//----------------------------------------------------------------------------------------------

//__Event Action Manager________________________________________________________________________
class EventAction : public G4UserEventAction {
public:
  EventAction(const size_t print_modulo);
  void BeginOfEventAction(const G4Event* event);
  void EndOfEventAction(const G4Event* event);
  static const G4Event* GetEvent();
  static size_t EventID();
};
//----------------------------------------------------------------------------------------------

//__Run Action Manager__________________________________________________________________________
class RunAction : public G4UserRunAction {
public:
  RunAction(const std::string& data_dir="");
  void BeginOfRunAction(const G4Run* run);
  void EndOfRunAction(const G4Run*);
  static const G4Run* GetRun();
  static size_t RunID();
  static size_t EventCount();
};
//----------------------------------------------------------------------------------------------

//__Generator Action Manager____________________________________________________________________
class GeneratorAction : public G4VUserPrimaryGeneratorAction, public G4UImessenger {
public:
  GeneratorAction(const std::string& generator="");
  void GeneratePrimaries(G4Event* event);
  void SetNewValue(G4UIcommand* command, G4String value);
  static const Physics::Generator* GetGenerator();
  static Physics::GenParticleVector GetLastEvent();
  static void SetGenerator(const std::string& generator);

private:
  Command::NoArg*     _list;
  Command::NoArg*     _current;
  Command::StringArg* _select;
};
//----------------------------------------------------------------------------------------------

class StepAction : public G4UserSteppingAction {
public:

  StepAction();
  void UserSteppingAction( const G4Step* aStep);
  static void WriteTree(int);
  static TTree* _step_data;

};

class StepDataStore {
public:
  static double _step_x;
  static double _step_y;
  static double _step_z;
  static double _step_px;
  static double _step_py;
  static double _step_pz;
  static double _energy_loss;
  static double _deposit;
  static int _pdg;
  static int _material_index;

  static void Initialize(int id);

};

} } /* namespace MATHUSLA::MU */

#endif /* MU__ACTION_HH */
