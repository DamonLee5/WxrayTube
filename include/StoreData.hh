#ifndef STOREDATA_H
#define STOREDATA_H

#include "globals.hh"
#include <vector>
#include <utility>
#include <string>

class StoreData
{
public:
  static StoreData* GetInstance(); // Retrieve the pointer to the singleton instance
  static void Initialize(G4int runNum, G4int pid); // Initialize the instance
  static void DeleteInstance(); // Delete the instance

  void FillEnergySpectrum(G4double eGamma); // Fill the histogram
  void WriteCSV(const std::string& filename); // Write data to a CSV file

private:
  StoreData(G4int runNum, G4int pid); // Private constructor
  ~StoreData(); // Private destructor

  static StoreData* instance; // Singleton instance

  std::vector<std::pair<G4double, G4double>> energySpectrum;
  G4int nBins;
  G4double minEnergy;
  G4double maxEnergy;
  G4double binWidth;
};

#endif // STOREDATA_H
