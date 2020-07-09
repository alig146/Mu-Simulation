/*
 * src/physics/Generator.cc
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

#include "physics/Generator.hh"

#include <cmath>
#include <limits>
#include <ostream>

#include <Randomize.hh>
#include <G4ParticleTable.hh>

#include "physics/Units.hh"
#include "tracking.hh"

#include "util/string.hh"

namespace MATHUSLA { namespace MU {

    namespace Physics { ////////////////////////////////////////////////////////////////////////////


//__Check if String has Number__________________________________________________________________
bool _has_digit(const std::string& s) {
  return std::any_of(s.begin(), s.end(), ::isdigit);
}
//----------------------------------------------------------------------------------------------


//__Generator Messenger Directory Path__________________________________________________________
const std::string Generator::MessengerDirectory = "/gen/";
//----------------------------------------------------------------------------------------------

//__Generator Simulation Setting Prefix_________________________________________________________
const std::string Generator::SimSettingPrefix = "GEN";
//----------------------------------------------------------------------------------------------

//__Generator Constructor_______________________________________________________________________
Generator::Generator(const std::string& name,
                     const std::string& description,
                     const Particle& particle)
    : G4UImessenger(MessengerDirectory + name + '/', description),
      _name(name), _description(description), _particle(particle) {
  GenerateCommands();
}
//----------------------------------------------------------------------------------------------

//__Generator Constructor_______________________________________________________________________
Generator::Generator(const std::string& name,
                     const std::string& description)
    : Generator(name, description, {}) {}
//----------------------------------------------------------------------------------------------

//__Generate UI Commands________________________________________________________________________
void Generator::GenerateCommands() {
  _ui_id = CreateCommand<Command::IntegerArg>("id", "Set Particle Id.");
  _ui_id->SetParameterName("id", false);
  _ui_id->AvailableForStates(G4State_PreInit, G4State_Idle);

  _ui_pT = CreateCommand<Command::DoubleUnitArg>("pT", "Set Transverse Momentum.");
  _ui_pT->SetParameterName("pT", false, false);
  _ui_pT->SetRange("pT >= 0");
  _ui_pT->SetDefaultUnit("GeV/c");
  _ui_pT->SetUnitCandidates("eV/c keV/c MeV/c GeV/c");
  _ui_pT->AvailableForStates(G4State_PreInit, G4State_Idle);

  _ui_eta = CreateCommand<Command::DoubleArg>("eta", "Set Pseudorapidity.");
  _ui_eta->SetParameterName("eta", false);
  _ui_eta->AvailableForStates(G4State_PreInit, G4State_Idle);

  _ui_phi = CreateCommand<Command::DoubleUnitArg>("phi", "Set Semi-Opening Angle.");
  _ui_phi->SetParameterName("phi", false, false);
  _ui_phi->SetDefaultUnit("deg");
  _ui_phi->SetUnitCandidates("degree deg radian rad milliradian mrad");
  _ui_phi->AvailableForStates(G4State_PreInit, G4State_Idle);

  _ui_ke = CreateCommand<Command::DoubleUnitArg>("ke", "Set Kinetic Energy.");
  _ui_ke->SetParameterName("ke", false, false);
  _ui_ke->SetRange("ke >= 0");
  _ui_ke->SetDefaultUnit("GeV");
  _ui_ke->SetUnitCandidates("eV keV MeV GeV");
  _ui_ke->AvailableForStates(G4State_PreInit, G4State_Idle);

  _ui_p = CreateCommand<Command::ThreeVectorUnitArg>("p", "Set Momentum.");
  _ui_p->SetParameterName("px", "py", "pz", false, false);
  _ui_p->SetDefaultUnit("GeV/c");
  _ui_p->SetUnitCandidates("eV/c keV/c MeV/c GeV/c");
  _ui_p->AvailableForStates(G4State_PreInit, G4State_Idle);

  _ui_p_unit = CreateCommand<Command::ThreeVectorArg>("p_unit", "Set Momentum Direction.");
  _ui_p_unit->SetParameterName("px", "py", "pz", false, false);
  _ui_p_unit->AvailableForStates(G4State_PreInit, G4State_Idle);

  _ui_p_mag = CreateCommand<Command::DoubleUnitArg>("p_mag", "Set Momentum Magnitude.");
  _ui_p_mag->SetParameterName("p_mag", false, false);
  _ui_p_mag->SetRange("p_mag >= 0");
  _ui_p_mag->SetDefaultUnit("GeV/c");
  _ui_p_mag->SetUnitCandidates("eV/c keV/c MeV/c GeV/c");
  _ui_p_mag->AvailableForStates(G4State_PreInit, G4State_Idle);

  _ui_t0 = CreateCommand<Command::DoubleUnitArg>("t0", "Set Initial Time");
  _ui_t0->SetParameterName("t0", false, false);
  _ui_t0->SetDefaultUnit("ns");
  _ui_t0->AvailableForStates(G4State_PreInit, G4State_Idle);

  _ui_vertex = CreateCommand<Command::ThreeVectorUnitArg>("vertex", "Set Initial Vertex Position");
  _ui_vertex->SetParameterName("x0", "y0", "z0", false, false);
  _ui_vertex->SetDefaultUnit("m");
  _ui_vertex->AvailableForStates(G4State_PreInit, G4State_Idle);
}
//----------------------------------------------------------------------------------------------

//__Generate Initial Particles__________________________________________________________________
void Generator::GeneratePrimaryVertex(G4Event* event) {
  AddParticle(_particle, *event);
}
//----------------------------------------------------------------------------------------------

//__Get Last Event Data_________________________________________________________________________
ParticleVector Generator::GetLastEvent() const {
  return ParticleVector{_particle};
}
//----------------------------------------------------------------------------------------------

//__Generator Messenger Set Value_______________________________________________________________
void Generator::SetNewValue(G4UIcommand* command,
                            G4String value) {
  if (command == _ui_id) {
    _particle.id = _ui_id->GetNewIntValue(value);
  } else if (command == _ui_pT) {
    _particle.set_pT(_ui_pT->GetNewDoubleValue(value));
  } else if (command == _ui_eta) {
    _particle.set_eta(_ui_eta->GetNewDoubleValue(value));
  } else if (command == _ui_phi) {
    _particle.set_phi(_ui_phi->GetNewDoubleValue(value));
  } else if (command == _ui_ke) {
    _particle.set_ke(_ui_ke->GetNewDoubleValue(value));
  } else if (command == _ui_p) {
    _particle.set_p(_ui_p->GetNew3VectorValue(value));
  } else if (command == _ui_p_unit) {
    _particle.set_p_unit(_ui_p_unit->GetNew3VectorValue(value).unit());
  } else if (command == _ui_p_mag) {
    _particle.set_p_mag(_ui_p_mag->GetNewDoubleValue(value));
  } else if (command == _ui_t0) {
    _particle.t = _ui_t0->GetNewDoubleValue(value);
  } else if (command == _ui_vertex) {
    _particle.set_vertex(_ui_vertex->GetNew3VectorValue(value));
  }
}
//----------------------------------------------------------------------------------------------

//__Generator Information String________________________________________________________________
std::ostream& Generator::Print(std::ostream& os) const {
  return os << "Generator Info:\n  "
            << "Name:        "  << _name                                     << "\n  "
            << "Description: "  << _description                              << "\n  "
            << "Particle ID: "  << _particle.id                              << "\n  "
            << "pT:          "  << G4BestUnit(_particle.pT(), "Momentum")    << "\n  "
            << "eta:         "  << _particle.eta()                           << "\n  "
            << "phi:         "  << G4BestUnit(_particle.phi(), "Angle")      << "\n  "
            << "ke:          "  << G4BestUnit(_particle.ke(), "Energy")      << "\n  "
            << "p:           "  << G4BestUnit(_particle.p(), "Momentum")     << "\n  "
            << "p_unit:      "  << _particle.p_unit()                        << "\n  "
            << "p_mag:       "  << G4BestUnit(_particle.p_mag(), "Momentum") << "\n"
            << "vertex:      (" << G4BestUnit(_particle.t, "Time")           << ", "
                                << G4BestUnit(_particle.x, "Length")         << ", "
                                << G4BestUnit(_particle.y, "Length")         << ", "
                                << G4BestUnit(_particle.z, "Length")         << ")\n";
}
//----------------------------------------------------------------------------------------------

//__Generator Specifications____________________________________________________________________
const Analysis::SimSettingList Generator::GetSpecification() const {
  return Analysis::Settings(SimSettingPrefix,
    "",        _name,
    "_PDG_ID", std::to_string(_particle.id),
    "_PT",     std::to_string(_particle.pT() / Units::Momentum)     + " "  + Units::MomentumString,
    "_ETA",    std::to_string(_particle.eta()),
    "_PHI",    std::to_string(_particle.phi() / Units::Angle)       + " "  + Units::AngleString,
    "_KE",     std::to_string(_particle.ke() / Units::Energy)       + " "  + Units::EnergyString,
    "_P", "(" + std::to_string(_particle.p().x() / Units::Momentum) + ", "
              + std::to_string(_particle.p().y() / Units::Momentum) + ", "
              + std::to_string(_particle.p().z() / Units::Momentum) + ") " + Units::MomentumString,
    "_P_UNIT", "(" + std::to_string(_particle.p_unit().x())         + ", "
                   + std::to_string(_particle.p_unit().y())         + ", "
                   + std::to_string(_particle.p_unit().z())         + ")",
    "_P_MAG", std::to_string(_particle.p_mag() / Units::Momentum)   + " "  + Units::MomentumString,
    "_VERTEX", "(" + std::to_string(_particle.t / Units::Time)      + ", "
                   + std::to_string(_particle.x / Units::Length)    + ", "
                   + std::to_string(_particle.y / Units::Length)    + ", "
                   + std::to_string(_particle.z / Units::Length)    + ")");
}
//----------------------------------------------------------------------------------------------

//__Generator Extra Details_____________________________________________________________________
const std::vector<std::vector<double>> Generator::ExtraDetails() const {
  return Tracking::EmptyExtra();
}
//----------------------------------------------------------------------------------------------

} /* namespace Physics */ //////////////////////////////////////////////////////////////////////

} } /* namespace MATHUSLA::MU */
