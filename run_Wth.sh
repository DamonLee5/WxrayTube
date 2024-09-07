#!/bin/bash

# Path to your Geant4 executable
GEANT4_EXEC=./WxrayTube

# Accept command-line arguments for WTHICKNESSES, KVPS, and srcdetDegree
WTHICKNESSES=($1)  # First argument will be space-separated values for WTHICKNESSES
KVPS=($2)          # Second argument will be space-separated values for KVPS
SRCDETDEGREE=($3)  # Third argument will be space-separated values for srcdetDegree
PHYSICSMODEL=$4
DTHICKNESSES=250

# Loop over thicknesses and sizes
for THICKNESS in "${WTHICKNESSES[@]}"
do
    # Rewrite WxrayTube.cfg
    CFG_FILE="WxrayTube.cfg"
    if [ -f $CFG_FILE ]; then
        rm $CFG_FILE
    fi
    
    echo "PHYSICSMODEL = ${PHYSICSMODEL};" >> $CFG_FILE
    echo "targetThickness = ${THICKNESS} um;" > $CFG_FILE
    echo "diamondThickness = ${DTHICKNESSES} um;" >> $CFG_FILE
    
    # Loop over srcdetDegree values
    for DEGREE in "${SRCDETDEGREE[@]}"
    do
        echo "srcdetDegree = ${DEGREE} deg;" >> $CFG_FILE

        # Loop from 1 keV to 150 keV
        for ENERGY in "${KVPS[@]}"
        do
            echo "ENERGY = ${ENERGY} kVp;" >> $CFG_FILE

            # Create a temporary macro file with the current energy
            for t in {1..200}
            do 
                seed1=$(date +%s%N | cut -b1-10)  # Seed based on the current time in nanoseconds
                seed2=$(date +%s%N | cut -b11-20) # Another seed based on the time
                MACRO_FILE="temp_macro_${t}.mac"
                cp run.mac $MACRO_FILE
                echo "/random/setSeeds $seed1 $seed2" >> $MACRO_FILE
                echo "/gun/energy ${ENERGY} keV" >> $MACRO_FILE

                # Run the Geant4 simulation
                $GEANT4_EXEC $MACRO_FILE > "${t}.log" &
            done
            wait

            # Move the output file to the output directory (if any)
            # mv output/ $OUTPUT_DIR/
        done

    done
    cp temp_macro_0.mac temp_macro.mac
    # Clean up temporary macro file
    rm temp_macro_*.mac

done
