#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Sphere.hh"
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
  // print the defined materials
  G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  // parameters of the geometry
  worldLength = 3 * cm;

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

  // Parameters of the G4Sphere
  G4double innerRadius = 10.*mm;              // Inner radius of the sphere
  G4double outerRadius = 10.1*mm;            // Outer radius of the sphere (100 mm as specified)
  G4double startPhiAngle = 0.*deg;           // Starting phi angle (0 degrees)
  G4double deltaPhiAngle = 360.*deg;         // Total phi angle (360 degrees for a full circle)
  G4double startThetaAngle = 0.*deg;        // Starting theta angle (close to 90 deg for a plane-like shape)
  G4double srcdetDegree = 2.*deg;         // Apex angle (2 degrees as specified)

  try {
      Config config = readConfig("WxrayTube.cfg");
      targetThickness = config.targetThickness;
      diamondThickness = config.diamondThickness;
      srcdetDegree = config.srcdetDegree;
      G4cout << "targetThickness = " << targetThickness / um << " um" << G4endl;
      G4cout << "diamondThickness = " << diamondThickness / um << " um" << G4endl;
      G4cout << "srcdetDegree = " << srcdetDegree / deg << " deg" << G4endl;
  } catch (const std::exception &e) {
      G4Exception("main", "RuntimeError", FatalException, e.what());
      return physicalWorld;
  }  

  
  G4Box* tungstenBox = new G4Box("target", detectorSizeXY/2, detectorSizeXY/2, targetThickness/2);
  G4LogicalVolume* logicalTarget = new G4LogicalVolume(tungstenBox, W, "target");
  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -targetThickness/2), logicalTarget, "target", logicalWorld, false, 0);

  // 1. Create and place the Diamond
  G4Box* diamondBox = new G4Box("Diamond", detectorSizeXY/2, detectorSizeXY/2, diamondThickness/2);
  G4LogicalVolume* diamondLog = new G4LogicalVolume(diamondBox, diamond, "Diamond");
  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, diamondThickness/2), diamondLog, "Diamond", logicalWorld, false, 0);

  // 3. Create and place the Detector


  // Create the G4Sphere geometry with the parameters
  solidDetector = new G4Sphere("detector",
                                innerRadius,
                                outerRadius,
                                startPhiAngle,
                                deltaPhiAngle,
                                startThetaAngle,
                                srcdetDegree);

  // Create logical volume (you can choose a better name for "DetectorLogical")
  logicalDetector = new G4LogicalVolume(solidDetector,
                                        vacuum,
                                        "detector");

  // Position the detector (you can adjust the position according to your setup)
  G4ThreeVector detectorPosition = G4ThreeVector(0, 0, 0);  // Centered at the origin

  // Create physical volume
  new G4PVPlacement(0,                           // No rotation
                    detectorPosition,            // Position
                    logicalDetector,             // Logical volume
                    "detector",          // Name
                    logicalWorld,                // Mother volume (assuming you have a world volume)
                    false,                       // No boolean operations
                    0,                           // Copy number
                    true);                       // Check overlaps

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
