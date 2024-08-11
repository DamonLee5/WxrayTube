#include "StoreData.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
#include <iomanip>

StoreData* StoreData::instance = nullptr;

StoreData* StoreData::GetInstance()
{
  if (!instance)
  {
    G4Exception("StoreData::GetInstance", "StoreData", FatalException, "Instance not yet created!");
  }
  return instance;
}

void StoreData::Initialize(G4int runNum, G4int pid)
{
  if (instance == nullptr)
  {
    instance = new StoreData(runNum, pid);
  }
}

void StoreData::DeleteInstance()
{
  if (instance != nullptr)
  {
    delete instance;
    instance = nullptr;
  }
}

StoreData::StoreData(G4int runNum, G4int pid)
  : nBins(1500), minEnergy(0.05), maxEnergy(150.05), binWidth((maxEnergy - minEnergy) / nBins)
{
  for (G4int i = 0; i < nBins; ++i)
  {
    G4double binCenter = minEnergy + (i + 0.5) * binWidth;
    energySpectrum.push_back({binCenter, 0});
  }
}

StoreData::~StoreData()
{
}

void StoreData::FillEnergySpectrum(G4double eGamma)
{
  if (eGamma >= minEnergy && eGamma < maxEnergy)
  {
    // G4cout << "Update E spectrum." << G4endl;
    G4int bin = static_cast<G4int>((eGamma - minEnergy) / binWidth);
    energySpectrum[bin].second += 1;
  }
    else{
    G4cout << minEnergy<<' '<<eGamma<<' '<<maxEnergy << G4endl;
  }
}

void StoreData::WriteCSV(const std::string& filename)
{
  std::ofstream outFile(filename);
  outFile << "BinCenter, BinContent\n";
  
  for (const auto& bin : energySpectrum)
  {
    outFile << std::fixed << std::setprecision(2) << bin.first << ", " << bin.second << "\n";
  }
  
  outFile.close();
}
