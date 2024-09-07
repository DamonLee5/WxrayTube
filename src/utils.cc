#include "utils.hh"
#include <G4Exception.hh>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Element.hh"
#include "G4SystemOfUnits.hh"
#include <map>
#include <regex>
#include <string>

G4Material* CreateMaterial(const std::string& name, const std::string& formula, G4double density) {
    G4NistManager* nist = G4NistManager::Instance();
    std::map<std::string, G4Element*> elementMap;

    // Define a regular expression to parse the chemical formula
    std::regex elementRegex("([A-Z][a-z]*)(\\d*)");
    std::smatch match;

    // Create a map to hold elements and their quantities
    std::map<G4Element*, G4int> elementQuantities;

    auto formulaIt = formula.cbegin();
    while (std::regex_search(formulaIt, formula.cend(), match, elementRegex)) {
        std::string elementSymbol = match[1];
        G4int quantity = match[2].str().empty() ? 1 : std::stoi(match[2]);

        // Find or build the element
        G4Element* element = nullptr;
        if (elementMap.find(elementSymbol) == elementMap.end()) {
            element = nist->FindOrBuildElement(elementSymbol);
            if (!element) {
                G4cerr << "Element " << elementSymbol << " not found!" << G4endl;
                return nullptr;
            }
            elementMap[elementSymbol] = element;
        } else {
            element = elementMap[elementSymbol];
        }

        // Add the element and its quantity to the map
        elementQuantities[element] += quantity;

        // Move to the next part of the formula
        formulaIt = match.suffix().first;
    }

    // Create the material
    G4Material* material = new G4Material(name, density, elementQuantities.size());
    for (const auto& elementQuantity : elementQuantities) {
        material->AddElement(elementQuantity.first, elementQuantity.second);
    }

    return material;
}


// Helper function to trim whitespace from a string
G4String trim(const G4String& str) {
    size_t first = str.find_first_not_of(" \t");
    size_t last = str.find_last_not_of(" \t");
    if (first == G4String::npos || last == G4String::npos)
        return ""; // String contains only whitespace
    return str.substr(first, last - first + 1);
}

// Define the readConfig function
Config readConfig(const G4String &filename) {
    Config config;
    std::ifstream file(filename);

    // Check if the file opens successfully
    if (!file.is_open()) {
        G4Exception("readConfig", "FileNotFound", FatalException, 
            ("Could not open file: " + filename).c_str());
    }

    G4String line;
    while (std::getline(file, line)) {
        // Remove comments (marked by ';')
        size_t pos = line.find(';');
        if (pos != G4String::npos) {
            line = line.substr(0, pos);
        }

        // Trim the line and skip empty lines
        line = trim(line);
        if (line.empty()) continue;

        // Extract key-value pairs
        std::istringstream lineStream(line);
        G4String key;

        if (std::getline(lineStream, key, '=')) {
            G4String value;
            std::getline(lineStream, value);

            // Trim whitespace from key and value
            key = trim(key);
            value = trim(value);

            G4double val;
            G4String unit;

            // Handle different keys in the config file
            if (key == "PhysicsModel") {
                config.PhysicsModel = std::stoi(value); // No unit expected for PhysicsModel
            } else if (key == "targetThickness" || key == "diamondThickness" || key == "srcdetDegree") {
                std::istringstream valueStream(value);
                valueStream >> val >> unit;

                if (unit.empty()) {
                    G4Exception("readConfig", "MissingUnit", FatalException, 
                        ("Missing unit for " + key).c_str());
                }

                // Validate unit existence and set config values accordingly
                if (G4UnitDefinition::GetCategory(unit) == "") {
                    G4Exception("readConfig", "InvalidUnit", FatalException, 
                        ("Invalid unit in config: " + unit).c_str());
                }

                G4double unitValue = G4UnitDefinition::GetValueOf(unit);

                if (key == "targetThickness") {
                    config.targetThickness = val * unitValue;
                } else if (key == "diamondThickness") {
                    config.diamondThickness = val * unitValue;
                } else if (key == "srcdetDegree") {
                    config.srcdetDegree = val * unitValue;
                }
            }
        }
    }

    file.close();
    return config;
}
