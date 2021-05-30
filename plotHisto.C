{
  gROOT->Reset();
  
  // Draw histos filled by Geant4 simulation 
  //   
  ////TFile f("run1.root");
  TFile f("Muon.root");      
  TCanvas* c0 = new TCanvas("histX", "Muon Decay Location X-Axis");
  TCanvas* c1 = new TCanvas("histY", "Muon Decay Location Y-Axis");
  TCanvas* c2 = new TCanvas("histZ", "Muon Decay Location Z-Axis");

  c0->SetLogy(1);
  c0->cd();
  c0->Update();
  
  TH1D* histX = (TH1D*)f.Get("0");
  histX->Draw("HIST");

  c1->SetLogy(1);
  c1->cd();
  c1->Update();

  TH1D* histY = (TH1D*)f.Get("1");
  histY->Draw("HIST");

  c2->SetLogy(1);
  c2->cd();
  c2->Update();

  TH1D* histZ = (TH1D*)f.Get("2");
  histZ->Draw("HIST");

	        
}  
