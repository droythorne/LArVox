prefix = /home/epp/phraar/Documents/Programs/Voxels
larvox_version_number = 0.1
larvox_root = $(prefix)/LArVox/$(larvox_version_number)
VOXEL_LIBRARIES=

flags_for_gcc= -Wextra -Wfatal-errors -Wwrite-strings -Woverloaded-virtual -fno-nonansi-builtins -fno-gnu-keywords -fstrict-aliasing
local_CFLAGS = -g $(flags_for_gcc)
local_LDFLAGS= -L $(prefix)/SQLite3
ROOT_LIBS=`root-config --libs` -lSpectrum
ROOT_CFLAGS=`root-config --cflags`
ROOT_INCLUDE=`root-config --incdir`
INCFLAGS = -I . -I $(prefix)/kdrecon -I $(larvox_root)/src -I /home/epp/phraar/Boost/boost_1_41_0 -I $(prefix) -I /home/epp/phraar/libs/jama_125 -I /home/epp/phraar/libs/tnt_126 -I /home/epp/phraar/libs -I $(ROOT_INCLUDE)


VOXEL_SRC = Voxel.cpp TreeToVoxSet.cpp VoxelSetTransforms.cpp

VOXEL_OBJ = $(VOXEL_SRC:.cpp=.o)
TreeToVoxSetTest : $(LIBRARIES) $(VOXEL_OBJ) TreeToVoxSetTest.o
	gcc $(VOXEL_OBJ) -o  $@  -I . $(LDFLAGS) $(local_LDFLAGS) $(addprefix -l,$(LIBRARIES)) $(ROOT_LIBS) 
TreeToVoxSetTest2 : $(LIBRARIES) $(VOXEL_OBJ) TreeToVoxSetTest2.o
	gcc $(VOXEL_OBJ) TreeToVoxSetTest2.o -o  $@  -I . $(LDFLAGS) $(local_LDFLAGS) $(addprefix -l,$(LIBRARIES)) $(ROOT_LIBS) 
TestVoxSetToTree : Voxel.o TreeToVoxSet.o VoxSetToTree.o TVoxel.o tvoxeldict.o TestVoxSetToTree.o TreeToTVoxel.o
	gcc -o  $@  $^ $(LDFLAGS) $(local_LDFLAGS) $(addprefix -l,$(LIBRARIES)) $(ROOT_LIBS)
TestVoxSetToTree_old :  Voxel.o TreeToVoxSet.o VoxSetToTree_old.o TestVoxSetToTree_old.o
	gcc -o  $@  $^ $(LDFLAGS) $(local_LDFLAGS) $(addprefix -l,$(LIBRARIES)) $(ROOT_LIBS)
TestVoxelNonMaxSupp : Voxel.o TreeToVoxSet.o VoxelNonMaxSupp.o VoxelTree.o TestVoxelNonMaxSupp.o
	gcc -o  $@  $^ $(LDFLAGS) $(local_LDFLAGS) $(addprefix -l,$(LIBRARIES)) $(ROOT_LIBS)
%.o : %.cpp
	g++ $(CFLAGS) $(local_CFLAGS) -c $< $(INCFLAGS)
tvoxeldict.cpp: TVoxel.hpp 
	@rootcint $@ -c -p $^ TVoxel_linkdef.h
.PHONY: clean
clean:
	rm -f TreeToVoxSetTest TestVoxSetToTree TestVoxelNonMaxSupp ./*.o
	rm -f tvoxeldict.cpp tvoxeldict.h

