#include "StoreData.hh"

#include "TH1D.h"
#include "TFile.h"

#include "stdlib.h"

#include "globals.hh"

StoreData* StoreData::instance = 0;

StoreData* StoreData::GetInstance()
{
  return instance;
}

StoreData::StoreData(int runNum, int pid)
{
  if(instance != 0)
    {
      G4cout << "There is already an instance, a new one will not be created!" << G4endl;
      return;
    }

  instance = this;

  char fileName[200];
  sprintf(fileName, "output/outFileRun%ipid%i.root", runNum, pid);
  outFile = new TFile(fileName, "RECREATE");
  energySpectrum = new TH1D("energySpectrum", "Energy spectrum of the x-rays;Energy [keV];Number of entries", 800, 0, 40);
  energySpectrum->SetFillColor(kBlue);
  energySpectrum->SetLineColor(kRed);
}

StoreData::~StoreData()
{
  instance = 0;
}

void StoreData::WriteData()
{
  outFile->cd();

  energySpectrum->Write();

  outFile->Close();

  return;
}

void StoreData::FillEnergySpectrum(double eGamma)
{
  energySpectrum->Fill(eGamma);
  return;
}
