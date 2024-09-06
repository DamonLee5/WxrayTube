cmake_minimum_required(VERSION 3.10)
project(WxrayTube)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the installation prefix of ROOT to CMAKE_PREFIX_PATH
list(APPEND CMAKE_PREFIX_PATH /usr/local/opt/root)  # Update this path to your ROOT installation

# Find Geant4 package
find_package(Geant4 REQUIRED)

# Find ROOT package
find_package(ROOT REQUIRED COMPONENTS Hist RIO)

# Include Geant4 use file to set up necessary environment variables
include(${Geant4_USE_FILE})

# Include directories for ROOT and Geant4
include_directories(${Geant4_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)

# Source files
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(WxrayTube WxrayTube.cc ${sources} ${headers})
# Link libraries
target_link_libraries(${PROJECT_NAME} ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B6. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(WxrayTube_SCRIPTS
  init.mac
  init_vis.mac
  simulate.mac
  vis.mac
  run.mac
  WxrayTube.cfg
  run_Wth.sh
  )

foreach(_script ${WxrayTube_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()



# Install executable
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# Optional: For setting up data files, environment variables, etc.
# For example, if you need to set environment variables or copy data files, you can use the following commands
# configure_file(${PROJECT_SOURCE_DIR}/data/file.txt ${CMAKE_BINARY_DIR}/data/file.txt COPYONLY)
# set(ENV{VARIABLE_NAME} "value")
