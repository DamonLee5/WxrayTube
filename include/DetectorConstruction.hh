/**
 *Class used to construct the geometry of the simulation
 */

#ifndef DETECTORCONSTRUNCTION_H
#define DETECTORCONSTRUNCTION_H

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

class G4Box;
class G4CutTubs;
class G4LogicalVolume;
class G4VPhysicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  // the world
  G4Box* solidWorld;
  G4LogicalVolume* logicalWorld;
  G4VPhysicalVolume* physicalWorld;

  // the target of the x ray tube
  G4CutTubs* solidTarget;
  G4LogicalVolume* logicalTarget;
  G4VPhysicalVolume* physicalTarget;

  // the sensitive volume to record the energy of the x rays
  G4Box* solidDetector;
  G4LogicalVolume* logicalDetector;
  G4VPhysicalVolume* physicalDetector;

  G4double worldLength;
  G4double targetLength;
  G4double targetRotation;
  G4double targetCutAngle;
  G4double targetRadius;
  G4double detectorLength;

public:
  DetectorConstruction();
  ~DetectorConstruction();
  G4VPhysicalVolume* Construct();
};

#endif // #ifndef DETECTORCONSTRUNCTION_H
