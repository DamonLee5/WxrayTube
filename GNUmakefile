
name := WxrayTube
G4TARGET := $(name)
G4EXLIB := true

ROOTCFLAGS = `root-config --cflags`
ROOTLIBS = `root-config --nonew --libs`
CPPFLAGS += $(ROOTCFLAGS)
EXTRALIBS +=  $(ROOTLIBS)

all: lib bin

include $(G4INSTALL)/config/architecture.gmk
include $(G4INSTALL)/config/binmake.gmk

