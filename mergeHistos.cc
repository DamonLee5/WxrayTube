
/**
 * root macro to merge the histograms of the xray spectrum
 * from various runs produced in different processes
 * the histogram is drawn on screen and the bin content is written to a file
 */

#include "TFile.h"
#include "TH1D.h"
#include "TString.h"

#include "iostream"
#include "fstream"

void mergeHistos()
{
  TSystemDirectory* dir = new TSystemDirectory("output", "output");
  TList* files = dir->GetListOfFiles();
  int nFiles = files->GetSize();

  if(nFiles == 0)
    {
      std::cout << "No files to read\n";
      return;
    }

 
  TFile* inFile;
  TH1D* totHisto = 0;
  TString name;
  bool first = true;
  for(int i = 0; i < nFiles; ++i)
    {
      name = dir->GetName();
      name += "/";
      name += files->At(i)->GetName();
      if(name.EndsWith(".root"))
	{
	  // std::cout << name << std::endl;
	  inFile = TFile::Open(name);

	  if(first)
	    {
	      first = false;
	      totHisto = (TH1D*) inFile->Get("energySpectrum");
	      continue;
	    }

	  totHisto->Add(totHisto, (TH1D*) inFile->Get("energySpectrum"));
	}
    }

  totHisto->Draw();

  std::ofstream outFile;
  outFile.open("binContent.dat", std::ofstream::out);

  outFile << "#Energy spectrum\n";
  outFile << "#Energy [keV]\tnumber of entries\n\n";

  int nBins = totHisto->GetNbinsX();

  for(int i = 1; i <= nBins; ++i)
    {
      //std::cout << (double) totHisto->GetBinCenter(i) << '\t' << (double) totHisto->GetBinContent(i) << std::endl;
      outFile << totHisto->GetBinCenter(i) << '\t' << totHisto->GetBinContent(i) << '\n';
    }

  outFile.close();

  TFile* outRoot = new TFile("spectrum.root", "RECREATE");
  outRoot->cd();
  totHisto->Write();
  outRoot->Close();

  return;
}
