prefix = /home/epp/phraar/Documents/Programs/Voxels
larvox_version_number = 0.1
larvox_root = $(prefix)/LArVox/$(larvox_version_number)
LIBRARIES=
VOXEL_LIBRARIES=

flags_for_gcc= -Wextra -Wfatal-errors -Wwrite-strings -Woverloaded-virtual -fno-nonansi-builtins -fno-gnu-keywords -fstrict-aliasing
local_CFLAGS = -g $(flags_for_gcc)
local_LDFLAGS= 
ROOT_LIBS:=$(shell root-config --libs) -lSpectrum 
ROOT_CFLAGS=`root-config --cflags`
ROOT_INCLUDE=`root-config --incdir`
INCFLAGS = -I $(larvox_root)/src -I /home/epp/phraar/Boost/boost_1_41_0 -I $(prefix)  -I /home/epp/phraar/libs/tnt_126 -I /home/epp/phraar/libs/jama_125 -I $(ROOT_INCLUDE) -I /home/epp/phraar/libs/tclap-1.1.0/include

LARVOX_SRC =  src/my_kdrecon_classes/HarrisPlessey.cpp CmdLineParser.cpp LArVoxTest_HP.cpp
VOXEL_SRC = $(addprefix src/, voxels/Voxel.cpp voxels/VoxelSetTransforms.cpp voxels/TreeToVoxSet.cpp voxels/VoxSetToTree.cpp voxels/VoxelTree.cpp voxels/VoxelSet3DKDConv.cpp voxels/VoxelNonMaxSupp.cpp voxels/TVoxel.cpp) 
LARVOX_OBJ = $(LARVOX_SRC:.cpp=.o)
VOXEL_OBJ = $(VOXEL_SRC:.cpp=.o) tvoxeldict.o
BIN = CmdLineParserTest LArVoxTest_HP
CmdLineParserTest : CmdLineParserTest.o CmdLineParser.o
	g++ $(CFLAGS) -o $@ $^
LArVoxTest_HP : $(VOXEL_OBJ) $(LARVOX_OBJ)
	g++ $(CFLAGS) $(ROOT_CFLAGS) -o $@ $^ $(local_LDFLAGS) $(ROOT_LIBS) 
%.o : %.cpp
	g++ $(CFLAGS) $(local_CFLAGS) -c $< $(INCFLAGS) -o $(larvox_root)/$@
tvoxeldict.cpp: src/voxels/TVoxel.hpp 
	@rootcint $@ -c -p $^ src/voxels/TVoxel_linkdef.h 
.PHONY: clean
clean:
	rm -f $(BIN) ./*.o
