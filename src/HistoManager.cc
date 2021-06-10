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
/// \file HistoManager.cc
/// \brief Implementation of the HistoManager class
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "HistoManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
  : fFileName("simulation")
{
  Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{

  // Create or get analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in HistoManager.hh
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetFileName(fFileName);
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetActivation(true);     //enable inactivation of histograms
  
  // Define histograms start values
  const G4int kMaxHisto = 3;
  const G4String id[] = {"th0","th1","th2","th3","th4","th5","th6","th7","th8","th9","th10","th11","th12","th13","th14"};
  const G4String title[] = 
          { "X-Axis Decay Location",      //0     1D 0
            "Y-Axis Decay Location",      //1     1D 1
            "Z-Axis Decay Location",      //2     1D 2
            "3D Decay Locations",         //3     3D 0
            "X-Y Decay Locations",        //4     2D 0
            "Y-Z Decay Locations",        //5     2D 1
            "Z-X Decay Locations",        //6     2D 2
            "Decay Momentum Values",      //7     1D 3
            "Non-Zero Decay Momentum Values",//8  1D 4
	    "Non-Zero Momentum X-Y Decay Locations",//9   2D 3
            "Non-Zero Momentum Y-Z Decay Locations",//10  2D 4
            "Non-Zero Momentum Z-X Decay Locations",//11  2D 5
            "Zero Momentum X-Y Decay Locations",//12      2D 6
            "Zero Momentum Y-Z Decay Locations",//13      2D 7
            "Zero Momentum Z-X Decay Locations"//14       2D 8
          };

  // Default values (to be reset via /analysis/h1/set command)               
  G4int nbins = 100;
  G4double vmin = -1000*m;
  G4double vmax = 1000 *m;

  // Create all histograms as inactivated 
  // as we have not yet set nbins, vmin, vmax
  
  //xyz axis 1D Decay Locations
  for (G4int k=0; k<kMaxHisto; k++) {
    G4int ih = analysisManager->CreateH1(id[k], title[k], nbins, vmin, vmax);
    analysisManager->SetH1Activation(ih, false);
  }

  //3D Decay locations
  G4int ih = analysisManager->CreateH3(id[3],title[3],nbins,vmin,vmax,nbins,vmin,vmax,nbins,vmin,vmax,"m","m","m","none","none","none","linear","linear","linear");
  analysisManager->SetH3Activation(ih,false);

  //2D Decay Locations
  for (G4int k =4; k<7; k++){
  G4int ij = analysisManager->CreateH2(id[k],title[k],nbins,vmin,vmax,nbins,vmin,vmax);
  analysisManager->SetH2Activation(ij,false);
  }  

//Momentum Values
  //1D Momentum Values
  for(G4int k =7; k<9;k++){
  G4int ij = analysisManager->CreateH1(id[k], title[k], nbins, 0, 15*GeV);
    analysisManager->SetH1Activation(ij, false);
  }

  //2D Decay Locations
  for(G4int k =9; k<15; k++){
  G4int ij = analysisManager->CreateH2(id[k],title[k],nbins,vmin,vmax,nbins,vmin,vmax);
  analysisManager->SetH2Activation(ij,false);
  }


}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//


