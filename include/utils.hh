#ifndef utils_h
#define utils_h 1

#include <G4String.hh>
#include <G4UnitsTable.hh>
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Element.hh"
#include "G4SystemOfUnits.hh"
#include <map>
#include <regex>
#include <string>

G4Material* CreateMaterial(const std::string& name, const std::string& formula, G4double density);

struct Config {
    G4double targetThickness;
    G4double diamondThickness;
    G4double srcdetDegree;
};


Config readConfig(const G4String &filename);

#endif // UTILS_H
