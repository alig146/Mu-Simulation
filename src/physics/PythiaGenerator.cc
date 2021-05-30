/*
 * src/physics/PythiaGenerator.cc
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

#include <sstream>

#include "physics/PythiaGenerator.hh"

#include <Pythia8/ParticleData.h>

#include "geometry/Earth.hh"
#include "geometry/Cavern.hh"
#include "physics/Units.hh"
#include "util/string.hh"

namespace MATHUSLA { namespace MU {

namespace Physics { ////////////////////////////////////////////////////////////////////////////

//__G4ThreadLocal Static Variables______________________________________________________________
G4ThreadLocal Pythia8::Pythia* PythiaGenerator::_pythia = nullptr;
G4ThreadLocal std::vector<std::string>* PythiaGenerator::_pythia_settings = nullptr;
G4ThreadLocal bool PythiaGenerator::_settings_on = false;
  
G4ThreadLocal double PythiaPromptMuonFilter::_pCut=40.;
G4ThreadLocal double PythiaPromptMuonFilter::_ptCut=-999.;
G4ThreadLocal double PythiaPromptMuonFilter::_etaLoCut=0.6;
G4ThreadLocal double PythiaPromptMuonFilter::_etaHiCut=1.9;
G4ThreadLocal double PythiaPromptMuonFilter::_phiLoCut=-0.7;
G4ThreadLocal double PythiaPromptMuonFilter::_phiHiCut=0.7;

G4ThreadLocal double PythiaDisplacedFilter::_xLoCut=59500.;
G4ThreadLocal double PythiaDisplacedFilter::_xHiCut=89650.;
G4ThreadLocal double PythiaDisplacedFilter::_yLoCut=-50000.;
G4ThreadLocal double PythiaDisplacedFilter::_yHiCut=50000.;
G4ThreadLocal double PythiaDisplacedFilter::_zLoCut=69500.;
G4ThreadLocal double PythiaDisplacedFilter::_zHiCut=169500.;

  
  
//----------------------------------------------------------------------------------------------

//__Pythia Generator Construction_______________________________________________________________
PythiaGenerator::PythiaGenerator(Pythia8::Pythia* pythia)
    : Generator("pythia", "Pythia8 Generator.") {
  _pythia_settings = new std::vector<std::string>();
  SetPythia(pythia);

  _read_string = CreateCommand<Command::StringArg>("read_string", "Read Pythia String.");
  _read_string->SetParameterName("string", false);
  _read_string->AvailableForStates(G4State_PreInit, G4State_Idle);

  _read_file = CreateCommand<Command::StringArg>("read_file", "Read Pythia File.");
  _read_file->SetParameterName("file", false);
  _read_file->AvailableForStates(G4State_PreInit, G4State_Idle);

  _add_filter = CreateCommand<Command::StringArg>("add_filter", "Add a Pythia Filter.");
  _add_filter->SetParameterName("filter", false);
  _add_filter->AvailableForStates(G4State_PreInit, G4State_Idle);

  _add_filter_cut = CreateCommand<Command::StringArg>("add_filter_cut", "Add a Pythia Filter cut.");
  _add_filter_cut->SetParameterName("filter_cut", false);
  _add_filter_cut->AvailableForStates(G4State_PreInit, G4State_Idle);

}
//----------------------------------------------------------------------------------------------


//__Pythia Generator Construction_______________________________________________________________
  PythiaGenerator::PythiaGenerator(const std::vector<std::string>& settings) : PythiaGenerator() {
    SetPythia(settings);
  }
//----------------------------------------------------------------------------------------------

//__Pythia Generator Construction_______________________________________________________________
  PythiaGenerator::PythiaGenerator(const std::string& path) : PythiaGenerator() {
    SetPythia(path);
  }
//----------------------------------------------------------------------------------------------

namespace { ////////////////////////////////////////////////////////////////////////////////////

//__Setup Pythia Randomness_____________________________________________________________________
Pythia8::Pythia* _setup_random(Pythia8::Pythia* pythia) {
  int threadno=G4Threading::G4GetThreadId();
  pythia->readString("Random:setSeed = on");
  std::ostringstream oss;
  oss << "Random:seed = " << threadno;
  pythia->readString(oss.str());
  pythia->readString("Next:showScaleAndVertex = on");
  return pythia;
}
//----------------------------------------------------------------------------------------------

//__Reconstruct Pythia Object from Old Object___________________________________________________
Pythia8::Pythia* _reconstruct_pythia(Pythia8::Pythia* pythia) {
  if (!pythia) {
    return new Pythia8::Pythia();
  } else {
    auto out = new Pythia8::Pythia(pythia->settings, pythia->particleData);
    _setup_random(out);
    return out;
  }
}
//----------------------------------------------------------------------------------------------

//__Create Pythia from Settings_________________________________________________________________
Pythia8::Pythia* _create_pythia(std::vector<std::string>* settings,
                                bool& settings_on) {
  auto pythia = new Pythia8::Pythia();
  for (const auto& setting : *settings)
    pythia->readString(setting);
  _setup_random(pythia);
  pythia->init();
  settings_on = true;
  return pythia;
}
//----------------------------------------------------------------------------------------------

//__Convert Pythia Particle to Particle_________________________________________________________
Particle _convert_particle(const Pythia8::Particle& particle) {
  const auto xz = Cavern::rotate_from_P1(particle.zProd() * mm, -particle.xProd() * mm);
  Particle out(particle.id(),
               particle.tProd() * mm / c_light,
               static_cast<double>(xz.first),
               particle.yProd() * mm,
               static_cast<double>(xz.second + Earth::TotalShift() + Cavern::CMSIP()));
  out.set_pseudo_lorentz_triplet(particle.pT() * GeVperC, particle.eta(), particle.phi() * rad);
  out.genParticleRef=particle.index();
  return out;
}
//----------------------------------------------------------------------------------------------

} /* anonymous namespace */ ////////////////////////////////////////////////////////////////////

//__Generate Initial Particles__________________________________________________________________
void PythiaGenerator::GeneratePrimaryVertex(G4Event* g4event) {
  if (!_settings_on && !_pythia_settings->empty()) {
    _pythia = _create_pythia(_pythia_settings, _settings_on);
  } else if (!_pythia) {
    std::cout << "\n[ERROR] No Pythia Configuration Specified.\n";
  }

  ++_counter;
  _pythia->next();

  // "_last_event" is what is stored in the ntuple
  _last_event.clear();
  for (int i = 0; i < _pythia->event.size(); ++i)
    _last_event.push_back(_pythia->event[i]);

  if(!_filter)
    _filter=new PythiaFilter();

  // "filtered" is what is sent to Geant4 for propagation
  std::vector<int> indexlist;
  _filter->GetParticles(_pythia->event, indexlist);
 
  // add the G4index to the _last_event information (add 1 to match Geant4!)
  // then convert it to a particle for addition to the g4event
  for(unsigned long i = 0; i<indexlist.size(); ++i) {
    _last_event[indexlist[i]].G4index=i+1;
    Particle p=_convert_particle(_pythia->event[indexlist[i]]);
    AddParticle(p, *g4event);
  }


}
  
//----------------------------------------------------------------------------------------------

//__Get Last Event Data_________________________________________________________________________
GenParticleVector PythiaGenerator::GetLastEvent() const {
  return _last_event;
}
//----------------------------------------------------------------------------------------------

//__Messenger Set Value_________________________________________________________________________
void PythiaGenerator::SetNewValue(G4UIcommand* command,
                                  G4String value) {
  if (command == _read_string) {
    _pythia_settings->push_back(value);
    _settings_on = true;
  } else if (command == _read_file) {
    SetPythia(value);
    Generator::SetNewValue(command, value);
  } else if (command == _add_filter) {
    value.toLower();
    if(value.contains("promptmuon")) {
      if(_filter) delete _filter;
      _filter = new PythiaPromptMuonFilter();
    }
    if(value.contains("displaced")) {
      if(_filter) delete _filter;
      _filter = new PythiaDisplacedFilter();
    }
  } else if (command ==_add_filter_cut) {
    std::vector<std::string> tokens;
    util::string::split(util::string::strip(value), tokens, " ");
    PythiaPromptMuonFilter *f1=dynamic_cast<PythiaPromptMuonFilter *>(_filter);
    PythiaDisplacedFilter *f2=dynamic_cast<PythiaDisplacedFilter *>(_filter);

    if(tokens.size()>=2) {
      double val=std::atof(tokens[1].c_str());
      if(f1) {
	if(tokens[0].find("pcut")!=std::string::npos)          { f1->_pCut=val; }
	else if(tokens[0].find("ptcut")!=std::string::npos)    { f1->_ptCut=val; }
	else if(tokens[0].find("etalocut")!=std::string::npos) { f1->_etaLoCut=val; }
	else if(tokens[0].find("etahicut")!=std::string::npos) { f1->_etaHiCut=val; }
	else if(tokens[0].find("philocut")!=std::string::npos) { f1->_phiLoCut=val; }
	else if(tokens[0].find("phihicut")!=std::string::npos) { f1->_phiHiCut=val; }
      }
      else if(f2) {
	if(tokens[0].find("xlocut")!=std::string::npos)      { f2->_xLoCut=val; }
	else if(tokens[0].find("xhicut")!=std::string::npos) { f2->_xHiCut=val; }
	else if(tokens[0].find("ylocut")!=std::string::npos) { f2->_yLoCut=val; }
	else if(tokens[0].find("yhicut")!=std::string::npos) { f2->_yHiCut=val; }
	else if(tokens[0].find("zlocut")!=std::string::npos) { f2->_zLoCut=val; }
	else if(tokens[0].find("zhicut")!=std::string::npos) { f2->_zHiCut=val; }
      }      
    }
    
  }
}
//----------------------------------------------------------------------------------------------

//__Set Pythia Object from Copy_________________________________________________________________
void PythiaGenerator::SetPythia(Pythia8::Pythia* pythia) {
  if (!pythia)
    return;
  _counter = 0ULL;
  _pythia_settings->clear();
  _settings_on = false;
  _pythia = _reconstruct_pythia(pythia);
  _pythia->init();
}
//----------------------------------------------------------------------------------------------

//__Set Pythia Object from Settings_____________________________________________________________
void PythiaGenerator::SetPythia(const std::vector<std::string>& settings) {
  *_pythia_settings = settings;
  _counter = 0ULL;
  _pythia = _create_pythia(_pythia_settings, _settings_on);
}
//----------------------------------------------------------------------------------------------

//__Set Pythia Object from Settings_____________________________________________________________
void PythiaGenerator::SetPythia(const std::string& path) {
  _counter = 0ULL;
  _pythia_settings->clear();
  _settings_on = false;
  _path = path;
  _pythia = new Pythia8::Pythia();
  _pythia->readFile(_path);
  _setup_random(_pythia);
  _pythia->init();
}
//----------------------------------------------------------------------------------------------

//__PythiaGenerator Specifications______________________________________________________________
  const Analysis::SimSettingList PythiaGenerator::GetSpecification() const {
    Analysis::SimSettingList config;
    if (_path.empty() && !_pythia_settings->empty()) {
      config = Analysis::IndexedSettings(SimSettingPrefix, "_SETTING_", *_pythia_settings);
    } else if (!_path.empty()) {
      config.emplace_back(SimSettingPrefix, "_CONFIG", _path);
    }

    Analysis::SimSettingList out;
    out.reserve(2UL + config.size());
    out.emplace_back(SimSettingPrefix, "", _name);
    out.emplace_back(SimSettingPrefix, "_FILTER",_filter->GetName());
    Analysis::SimSettingList filterspec=_filter->GetSpecification();
    out.insert(out.cend(), filterspec.begin(), filterspec.end());
    out.insert(out.cend(),
	       std::make_move_iterator(config.begin()),
	       std::make_move_iterator(config.end()));
    
    
    out.emplace_back(SimSettingPrefix, "_EVENTS", std::to_string(_counter));
    
    return out;
  }
  //----------------------------------------------------------------------------------------------
  
  void PythiaFilter::GetParticles(const Pythia8::Event& event, std::vector<int>& indexlist) {
	  indexlist.clear();
	  for (int i = 0; i < event.size(); ++i)
		  if(event[i].isFinal())
			  indexlist.push_back(i);
	  return;
  }
  
  void PythiaPromptMuonFilter::GetParticles(const Pythia8::Event& event, std::vector<int>& indexlist) {
    indexlist.clear();
    for (int i = 0; i < event.size(); ++i) {
		//		if(event[i].isFinal() && event[i].idAbs()==13 && event[i].pAbs()>_pCut && event[i].pT()>_ptCut && event[i].eta()>_etaLoCut && event[i].eta()<_etaHiCut && event[i].phi()>_phiLoCut && event[i].phi()<_phiHiCut)
		if(event[i].isFinal() && event[i].idAbs()==13 && event[i].pT()>_ptCut && event[i].phi()>_phiLoCut && event[i].phi()<_phiHiCut) {
			if (event[i].eta()>0.6 && event[i].eta()<0.8 && event[i].pAbs()>40.0) {
				indexlist.push_back(i);
			}
            else if (event[i].eta()>=0.8 && event[i].eta()<1.0 && event[i].pAbs()>45.0) {
                indexlist.push_back(i);
            }
			else if (event[i].eta()>=1.0 && event[i].eta()<1.2 && event[i].pAbs()>45.0) {
				indexlist.push_back(i);
	 		}
			else if (event[i].eta()>=1.2 && event[i].eta()<1.4 && event[i].pAbs()>55.0) {
				indexlist.push_back(i);
			}
			else if (event[i].eta()>=1.4 && event[i].eta()<1.6 && event[i].pAbs()>65.0) {
				indexlist.push_back(i);
			}
			else if (event[i].eta()>=1.6 && event[i].eta()<1.9 && event[i].pAbs()>80.0) {
				indexlist.push_back(i);
			}

		}

	}
	return;

  }

  const Analysis::SimSettingList PythiaPromptMuonFilter::GetSpecification(void) const {
    Analysis::SimSettingList out;
    out.emplace_back("FILTER_P", std::to_string(_pCut));
    out.emplace_back("FILTER_PT", std::to_string(_ptCut));
    out.emplace_back("FILTER_ETALO", std::to_string(_etaLoCut));
    out.emplace_back("FILTER_ETALHI", std::to_string(_etaHiCut));
    out.emplace_back("FILTER_PHILO", std::to_string(_phiLoCut));
    out.emplace_back("FILTER_PHIHI", std::to_string(_phiHiCut));
    return out;
  }
  
  void PythiaDisplacedFilter::GetParticles(const Pythia8::Event& event, std::vector<int>& indexlist) {
    indexlist.clear();
    for (int i = 0; i < event.size(); ++i)
      if(event[i].isFinal() && event[i].zProd()>_zLoCut && event[i].zProd()<_zHiCut && event[i].yProd()>_yLoCut && event[i].yProd()<_yHiCut && event[i].xProd()>_xLoCut && event[i].xProd()<_xHiCut)
	indexlist.push_back(i);
    return;
  }

  const Analysis::SimSettingList PythiaDisplacedFilter::GetSpecification(void) const {
    Analysis::SimSettingList out;
    out.emplace_back("FILTER_XLO",std::to_string(_xLoCut));
    out.emplace_back("FILTER_XHI",std::to_string(_xHiCut));
    out.emplace_back("FILTER_YLO",std::to_string(_yLoCut));
    out.emplace_back("FILTER_YHI",std::to_string(_yHiCut));
    out.emplace_back("FILTER_ZLO",std::to_string(_zLoCut));
    out.emplace_back("FILTER_ZHI",std::to_string(_zHiCut));
    return out;
  }



  
  } /* namespace Physics */ //////////////////////////////////////////////////////////////////////
  
} } /* namespace MATHUSLA::MU */
