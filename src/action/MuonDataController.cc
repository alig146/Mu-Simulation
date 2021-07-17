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
/// \file field/field04/src/F04StepMax.cc
/// \brief Implementation of the F04StepMax class
//

#include "MuonDataController.hh"

namespace MATHUSLA { namespace MU {

MuonDataController* MuonDataController::sController = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuonDataController::MuonDataController()
{
if(sController){
G4Exception("MuonDataController::MuonDataController","MuonDataControler01",FatalException, "MuonDataController::MuonDataController() has already been made.");
}else{
sController = this;
eventAction =0;
std::fstream eData;
        eData.open("src/action/muon5body_data.csv",std::fstream::in);
        //ignore first line
        G4String line;
	std::getline(eData,line);
        if(eData.is_open()){ //if the file is open
		srand(time(NULL));
                while(eData.peek() != EOF) //while the end of the file is NOT reached
		{
                        //9 vectors so use 9 get lines {e1x, e1y, e1z, e2x, e2y, e2z, e3x, e3y, e3z}
                        getline(eData, e1x, ',');
                        p1x.push_back(stod(e1x));
                        getline(eData, e1y, ',');
                        p1y.push_back(stod(e1y));
                        getline(eData, e1z, ',');
                        p1z.push_back(stod(e1z));
                        getline(eData, e2x, ',');
                        p2x.push_back(stod(e2x));
                        getline(eData, e2y, ',');
                        p2y.push_back(stod(e2y));
                        getline(eData, e2z, ',');
                        p2z.push_back(stod(e2z));
                        getline(eData, e3x, ',');
                        p3x.push_back(stod(e3x));
                        getline(eData, e3y, ',');
                        p3y.push_back(stod(e3y));
                        getline(eData, e3z, '\n'); //new line after z-momentum of the third electron
			p3z.push_back(stod(e3z));
                        i +=1;
                }
                eData.close(); //close the file     
	
        }else {
        G4cout << "Error: Unable to open file.  Make sure it is in the same directory or explicitly specify directory of file"; //if the file is not open output}
	}
}}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuonDataController::~MuonDataController() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuonDataController::getParticles(G4int timesRun, G4double* dataArray){
        dataArray[0] = p1x[timesRun];
        dataArray[1] = p1y[timesRun];
        dataArray[2] = p1z[timesRun];
        dataArray[3] = p2x[timesRun];
        dataArray[4] = p2y[timesRun];
        dataArray[5] = p2z[timesRun];
        dataArray[6] = p3x[timesRun];
        dataArray[7] = p3y[timesRun];
        dataArray[8] = p3z[timesRun];
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuonDataController::getRandomParticles(G4double* dataArray)
{
        int electronSample = rand() % 10000 + 0; //generates random number from 0 to 10,000 (total of 10,001)

        G4cout<<"electronSample: "<<electronSample<<G4endl;
        dataArray[0] = p1x[electronSample];
        dataArray[1] = p1y[electronSample];
        dataArray[2] = p1z[electronSample];
        dataArray[3] = p2x[electronSample];
        dataArray[4] = p2y[electronSample];
        dataArray[5] = p2z[electronSample];
        dataArray[6] = p3x[electronSample];
        dataArray[7] = p3y[electronSample];
        dataArray[8] = p3z[electronSample];
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuonDataController::incrementMuonDecays(){
MuonDecays+=1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool MuonDataController::getRandom(){
return Random;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int MuonDataController::getMuonDecays(){
return MuonDecays;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuonDataController::setMuonDecays(G4int number){
MuonDecays = number;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuonDataController::setRandom(G4bool value){
Random = value;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


MuonDataController* MuonDataController::getMuonDataController()
{

return sController;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuonDataController::setDecayInEvent(G4bool value){
DecayInEvent = value;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool MuonDataController::getDecayInEvent(){
return DecayInEvent;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
