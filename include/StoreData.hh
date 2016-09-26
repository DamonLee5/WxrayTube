/**
 *This class is used to store the x ray spectrum.
 *It contains one histogram and produces a root file.
 *The class must have just one istance for each run so a method to retrieve the pointer to the object is implemented.
 *The instance is initialized in the RunAction class and there is deleted at the end of the run.
 *The method that fills the histogram is called in the SteppingAction.
 */

#ifndef STOREDATA_H
#define STOREDATA_H

class TFile;
class TH1D;

class StoreData
{
  static StoreData* instance;
  TFile* outFile;
  TH1D* energySpectrum;

public:
  StoreData(int runNum, int pid);
  ~StoreData();

  static StoreData* GetInstance(); // retrieve the pointer to the object of the class

  void FillEnergySpectrum(double eGamma); // fill the histogram

  void WriteData(); // write the root file with the histo
};

#endif // #ifndef STOREDATA_H
