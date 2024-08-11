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
#include "utils.hh"
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
  // Define diamond with correct density
  G4double density = 3.52 * g/cm3;
  G4Material* diamond = new G4Material("Diamond", density, 1);
  diamond->AddElement(nistMaterials->FindOrBuildElement("C"), 1);
  G4Material* vacuum = nistMaterials->FindOrBuildMaterial("G4_Galactic");
  G4Material* air = nistMaterials->FindOrBuildMaterial("G4_AIR");
  // print the defined materials
  G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  // parameters of the geometry
  worldLength = 1 * cm;
  // targetLength = 5 * cm;
  // targetRotation = 90 * deg;
  // targetCutAngle = 19 * deg;
  // targetRadius = 1.5 * cm;
  // detectorLength = 5 * cm;

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

// Define the sizes
  G4double detectorSizeXY = 2*mm;
  G4double targetThickness = 0 * um;
  G4double diamondThickness = 0 * um;
  
  G4double detectorThickness = 0.2 * mm; // Thickness of the detector
  G4double gapThickness = 1.0 * mm; // Gap between the diamond and detector (adjust this as needed)

  try {
      Config config = readConfig("WxrayTube.cfg");
      targetThickness = config.targetThickness;
      diamondThickness = config.diamondThickness;
      G4cout << "targetThickness = " << targetThickness / um << " um" << G4endl;
      G4cout << "diamondThickness = " << diamondThickness / um << " um" << G4endl;
  } catch (const std::exception &e) {
      G4Exception("main", "RuntimeError", FatalException, e.what());
      return physicalWorld;
  }  
  // // solidTarget = new G4Box("target", targetLength / 2, targetLength / 2, targetLength / 2);
  // G4ThreeVector normCut(0, 0, 1);
  // normCut.rotateX(targetCutAngle);
  // solidTarget = new G4CutTubs("target",
	// 		      0, // inner radius
	// 		      targetRadius, // outer radius
	// 		      targetLength / 2, // length
	// 		      0, // starting phi angle in radiants (for e.g. a quarter of tube ess)
	// 		      2 * pi, // stopping phi angle
	// 		      G4ThreeVector(0, 0, -1), // external normal lower face
	// 		      normCut // external normal upper face
	// 		      );
  // logicalTarget = new G4LogicalVolume(solidTarget, W, "target");
  // // placement of the target with rotation
  // G4RotationMatrix rot;
  // rot.rotateY(-targetRotation);
  // physicalTarget = new G4PVPlacement(G4Transform3D(rot, G4ThreeVector(10 * cm, 0 * cm, 0 * cm)), // rotated via the matrix, at 10, 0, 0
	// 			     logicalTarget, // its logical volume
	// 			     "target", // name
	// 			     logicalWorld, // its mother volume
	// 			     false, // no boolean operations
	// 			     0); // copy number
  

  G4Box* tungstenBox = new G4Box("target", detectorSizeXY/2, detectorSizeXY/2, targetThickness/2);
  G4LogicalVolume* logicalTarget = new G4LogicalVolume(tungstenBox, W, "target");
  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -targetThickness/2), logicalTarget, "target", logicalWorld, false, 0);

  // 1. Create and place the Diamond
  G4Box* diamondBox = new G4Box("Diamond", detectorSizeXY/2, detectorSizeXY/2, diamondThickness/2);
  G4LogicalVolume* diamondLog = new G4LogicalVolume(diamondBox, diamond, "Diamond");
  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, diamondThickness/2), diamondLog, "Diamond", logicalWorld, false, 0);

  // 2. Create the Air Layer to fill the gap between Diamond and Detector
  G4Box* airBox = new G4Box("AirLayer", detectorSizeXY/2, detectorSizeXY/2, gapThickness/2);
  G4LogicalVolume* airLog = new G4LogicalVolume(airBox, air, "AirLayer");
  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, diamondThickness + gapThickness/2), airLog, "AirLayer", logicalWorld, false, 0);

  // Set the air layer to be gray
  G4VisAttributes* grayColor = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5)); // Gray color
  airLog->SetVisAttributes(grayColor);

  // 3. Create and place the Detector
  solidDetector = new G4Box("detector", detectorSizeXY / 2, detectorSizeXY / 2, detectorThickness / 2);
  logicalDetector = new G4LogicalVolume(solidDetector, vacuum, "detector");
  physicalDetector = new G4PVPlacement(0, // no rotation
              G4ThreeVector(0, 0, diamondThickness + gapThickness + detectorThickness/2),
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

  G4VisAttributes* diamondVisAtt= new G4VisAttributes(G4Colour(0.3,0.4,0.3)); // yellow
  diamondLog->SetVisAttributes(diamondVisAtt);

  G4VisAttributes* detectorVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,0.0)); // green
  logicalDetector->SetVisAttributes(detectorVisAtt);

  return physicalWorld; // return the world volume
}
