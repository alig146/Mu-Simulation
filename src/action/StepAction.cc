#include "action.hh"
#include <tls.hh>
#include <G4MTRunManager.hh>
#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
namespace MATHUSLA { namespace MU {


TTree* StepAction::_step_data = nullptr;

double StepDataStore::_step_x = 0;
double StepDataStore::_step_y = 0;
double StepDataStore::_step_z = 0;
double StepDataStore::_step_px = 0;
double StepDataStore::_step_py = 0;
double StepDataStore::_step_pz = 0;
double StepDataStore::_energy_loss = 0;
double StepDataStore::_deposit = 0;
int StepDataStore::_pdg = 0;
int StepDataStore::_material_index = 0;

void StepDataStore::Initialize(int id){

    auto event_id = std::to_string(id);
    auto file_name = "step_data/event_" + event_id + ".root";

    StepAction::_step_data = new TTree(file_name.c_str(), file_name.c_str());

    StepAction::_step_data->Branch("X_S", &_step_x, "X_S/D");
    StepAction::_step_data->Branch("Y_S", &_step_y, "Y_S/D");
    StepAction::_step_data->Branch("Z_S", &_step_z, "Z_S/D");

    StepAction::_step_data->Branch("PX_S", &_step_px, "PX_S/D");
    StepAction::_step_data->Branch("PY_S", &_step_py, "PY_S/D");
    StepAction::_step_data->Branch("PZ_S", &_step_pz, "PZ_S/D");

    StepAction::_step_data->Branch("DE", &_energy_loss, "DE/D");
    StepAction::_step_data->Branch("DEPOSIT", &_deposit, "DEPOSIT/D");
    StepAction::_step_data->Branch("PDG", &_pdg, "PDG/I");
    StepAction::_step_data->Branch("MATERIAL", &_material_index, "MATERIAL/I");
  }

//__Action Initialization Constructor___________________________________________________________
StepAction::StepAction() : G4UserSteppingAction() {

}
//----------------------------------------------------------------------------------------------

void StepAction::UserSteppingAction( const G4Step* step){
  

  const auto step_point = step->GetPreStepPoint();
  const auto post_step_point = step->GetPostStepPoint();
  const auto position   = G4LorentzVector(step_point->GetGlobalTime(), step_point->GetPosition());
  const auto momentum   = G4LorentzVector(step_point->GetTotalEnergy(), step_point->GetMomentum());

  StepDataStore::_step_x = position.x();
  StepDataStore::_step_y = position.y();
  StepDataStore::_step_z = position.z();

  StepDataStore::_step_px = momentum.x();
  StepDataStore::_step_py = momentum.y();
  StepDataStore::_step_pz = momentum.z();

  StepDataStore::_deposit = step->GetTotalEnergyDeposit();

  StepDataStore::_energy_loss = step_point->GetKineticEnergy() - post_step_point->GetKineticEnergy();

  StepDataStore::_pdg = step->GetTrack()->GetParticleDefinition()->GetPDGEncoding();

  StepDataStore::_material_index = step_point->GetMaterial()->GetIndex();
  _step_data->Fill();

  


}

void StepAction::WriteTree(int id){
  auto event_id = std::to_string(id);
  auto file_name = "step_data/event_" + event_id + ".root";

  TFile* f = new TFile(file_name.c_str(), "RECREATE");
  f->cd();
  _step_data->Write();
  f->Write();
  f->Close();
}


} } /* namespace MATHUSLA::MU */


