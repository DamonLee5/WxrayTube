#!/bin/bash

# Path to your Geant4 executable
GEANT4_EXEC=./WxrayTube

# Detector parameters
WTHICKNESSES=(1 3 5 7)
DTHICKNESSES=250
KVPS=(40 80 150)
# Loop over thicknesses and sizes
for THICKNESS in "${WTHICKNESSES[@]}"
do

    # Rewrite WxrayTube.cfg
    CFG_FILE="WxrayTube.cfg"
    if [ -f $CFG_FILE ]; then
        rm $CFG_FILE
    fi

    echo "targetThickness = ${THICKNESS} um;" > $CFG_FILE
    echo "diamondThickness = ${DTHICKNESSES} um;" >> $CFG_FILE

    # Loop from 1 keV to 150 keV
    for ENERGY in "${KVPS[@]}"
    do
        # Create output directory
        OUTPUT_DIR="output_Wth${THICKNESS}_C${DTHICKNESSES}_${ENERGY}kVp"
        # Create a temporary macro file with the current energy
        MACRO_FILE=temp_macro.mac
        cp run.mac $MACRO_FILE
        echo "/gun/energy ${ENERGY} keV" >> $MACRO_FILE
        
        # Run the Geant4 simulation
        $GEANT4_EXEC $MACRO_FILE
        
        # Move the output file to the output directory
        mv output/ $OUTPUT_DIR/
    done

    # Clean up temporary macro file
    rm $MACRO_FILE

done
