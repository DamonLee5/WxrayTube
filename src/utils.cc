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


// Define the readConfig function
Config readConfig(const G4String &filename) {
    Config config;
    std::ifstream file(filename);

    // Check if the file opens successfully
    if (!file.is_open()) {
        G4Exception("readConfig", "FileNotFound", FatalException, ("Could not open file: " + filename).c_str());
    }

    G4String line;
    while (std::getline(file, line)) {
        // Remove comments (marked by ';')
        size_t pos = line.find(';');
        if (pos != G4String::npos) {
            line = line.substr(0, pos);
        }

        // Trim leading/trailing spaces (only remove them around the key and value)
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line.empty()) continue;  // Skip empty lines

        // Extract key-value pairs
        std::istringstream lineStream(line);
        G4String key;

        if (std::getline(lineStream, key, '=')) {
            G4String value;
            std::getline(lineStream, value);

            // Trim whitespace from key and value
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            G4double val;
            G4String unit;

            if (key == "targetThickness" || key == "diamondThickness" || key == "srcdetDegree") {
                std::istringstream valueStream(value);
                valueStream >> val >> unit;

                // Check if the unit exists before applying
                if (G4UnitDefinition::GetCategory(unit) == "") {
                    G4Exception("readConfig", "InvalidUnit", FatalException, ("Invalid unit in config: " + unit).c_str());
                }

                // Set the appropriate values in the config struct
                if (key == "targetThickness") {
                    config.targetThickness = val * G4UnitDefinition::GetValueOf(unit);
                } else if (key == "diamondThickness") {
                    config.diamondThickness = val * G4UnitDefinition::GetValueOf(unit);
                } else if (key == "srcdetDegree") {
                    config.srcdetDegree = val * G4UnitDefinition::GetValueOf(unit);
                }
            }
        }
    }

    file.close();
    return config;
}
