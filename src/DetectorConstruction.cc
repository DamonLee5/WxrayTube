#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4CutTubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

using namespace CLHEP; // for units

DetectorConstruction::DetectorConstruction()//:
  // solidWorld(0), logicalWorld(0), physicalWorld(0),
  // solidTarget(0), logicalTarget(0), physicalTarget(0),
  // worldLength(0), targetLength(0), targetRotation(0), detectorLength(0)
{
}

DetectorConstruction::~DetectorConstruction()
{
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Material definition
  G4NistManager* nistMaterials = G4NistManager::Instance();
  G4Material* W = nistMaterials->FindOrBuildMaterial("G4_W");
  G4Material* vacuum = nistMaterials->FindOrBuildMaterial("G4_Galactic");

  // print the defined materials
  G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  // parameters of the geometry
  worldLength = 30 * cm;
  targetLength = 5 * cm;
  targetRotation = 90 * deg;
  targetCutAngle = 19 * deg;
  targetRadius = 1.5 * cm;
  detectorLength = 5 * cm;

  // volume definition and placement
  solidWorld = new G4Box("world", worldLength / 2, worldLength / 2, worldLength / 2);
  logicalWorld = new G4LogicalVolume(solidWorld, vacuum, "world");
  physicalWorld = new G4PVPlacement(0, // no rotation
				    G4ThreeVector(), // at 0, 0, 0
				    logicalWorld, // its logical volume
				    "world", // name
				    0, // its mother volume
				    false, // no boolean operations
				    0); // copy number

  // solidTarget = new G4Box("target", targetLength / 2, targetLength / 2, targetLength / 2);
  G4ThreeVector normCut(0, 0, 1);
  normCut.rotateX(targetCutAngle);
  solidTarget = new G4CutTubs("target",
			      0, // inner radius
			      targetRadius, // outer radius
			      targetLength / 2, // length
			      0, // starting phi angle in radiants (for e.g. a quarter of tube ess)
			      2 * pi, // stopping phi angle
			      G4ThreeVector(0, 0, -1), // external normal lower face
			      normCut // external normal upper face
			      );
  logicalTarget = new G4LogicalVolume(solidTarget, W, "target");
  // placement of the target with rotation
  G4RotationMatrix rot;
  rot.rotateY(-targetRotation);
  physicalTarget = new G4PVPlacement(G4Transform3D(rot, G4ThreeVector(10 * cm, 0 * cm, 0 * cm)), // rotated via the matrix, at 10, 0, 0
				     logicalTarget, // its logical volume
				     "target", // name
				     logicalWorld, // its mother volume
				     false, // no boolean operations
				     0); // copy number
  
  solidDetector = new G4Box("detector", detectorLength / 2, detectorLength / 2, detectorLength / 2);
  logicalDetector = new G4LogicalVolume(solidDetector, vacuum, "detector");
  physicalDetector = new G4PVPlacement(0, // no rotation
				       G4ThreeVector(7.5 * cm, -9.5 * cm, 0 * cm),
				       logicalDetector, // its logical volume
				       "detector", // name
				       logicalWorld, // its mother volume
				       false, // no boolean operations
				       0); // copy number

  // visualization properties
  G4VisAttributes* worldVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0)); // white
  logicalWorld->SetVisAttributes(worldVisAtt);

  G4VisAttributes* targetVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0)); // yellow
  logicalTarget->SetVisAttributes(targetVisAtt);

  G4VisAttributes* detectorVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,0.0)); // green
  logicalDetector->SetVisAttributes(detectorVisAtt);

  return physicalWorld; // return the world volume
}
