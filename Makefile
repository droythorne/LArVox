# - Makefile for Liquid Argon Voxel utilities
#

# Locate ROOT 
ROOT_ROOT := $(shell root-config --prefix)
ifeq ($(ROOT_ROOT), )
    $(error running of root-config failed or reported a null value)
endif

ROOT_MAKEFILE := $(or $(wildcard $(ROOT_ROOT)/test/Makefile.arch), $(wildcard $(ROOT_ROOT)/share/doc/root/test/Makefile.arch), $(error failed to locate Makefile.arch under $(ROOT_ROOT)))
include $(ROOT_MAKEFILE)

#Build variables
SRCDIR := $(shell pwd)/src
APPDIR := $(shell pwd)/apps
PYSRCDIR := $(shell pwd)/pylarvox
OPUTDIR := $(shell pwd)/tmp

LIBNAME := $(OPUTDIR)/liblarvox.so
LIBDICTHEADER := $(OPUTDIR)/_larvox_dict.hpp
LIBDICTSOURCE := $(OPUTDIR)/_larvox_dict.cpp
LIBLINKDEF := $(SRCDIR)/larvox_linkdef.hh

LIBPYTHONLIBRARY := $(OPUTDIR)/_larvox_C.so
LIBPYTHONMODULE := $(PYSRCDIR)/larvox.py

LIBSOURCES := $(filter-out *_dict.cpp, $(wildcard $(SRCDIR)/*.cpp)) $(LIBDICTSOURCE)
LIBHEADERS := $(filter-out $(LIBLINKDEF), $(wildcard $(SRCDIR)/*.hpp))
INCDIRS := -I /usr/local/include

LIBOBJECTS := $(patsubst %.cpp, $(OPUTDIR)/%.o, $(notdir $(LIBSOURCES)))

EXAMPLE_APP_SOURCE := $(APPDIR)/example.cpp
EXAMPLE_APP_OBJECT := $(OPUTDIR)/example.o
EXAMPLE_APP := $(OPUTDIR)/example

package := larvox
ifeq ($(prefix), )
    prefix := /home/droythorne/local
endif
version := 0.1
exec_prefix := $(prefix)
bindir := $(exec_prefix)/bin
includedir := $(prefix)/include
ARCH := $(shell uname -m | cut -s -d "_" -f 2)
libdir := $(exec_prefix)/lib$(ARCH)
pylibdir := $(shell python -c "import distutils.sysconfig; print distutils.sysconfig.get_python_lib(1,prefix='$(exec_prefix)')")

#Targets
.PHONY: clean

all: larvox pylarvox larvox_example Makefile

larvox: $(LIBNAME)

larvox_example: $(EXAMPLE_APP)
 
$(LIBNAME): $(LIBOBJECTS) 
	@echo "- Generating dynamic library $@"
	@$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@ $(ROOTLIBS)

$(LIBDICTSOURCE): $(SRCDIR)/TVoxel.hpp $(LIBLINKDEF)
	@echo "- Generating RootCint dictionary $@"
	@mkdir -p tmp 2>/dev/null
	@$(ROOT_ROOT)/bin/rootcint -f $@ -c -p $^

#tvoxeldict.cpp: src/voxels/TVoxel.hpp 
#	@rootcint $@ -c -p $^ src/voxels/TVoxel_linkdef.h 
pylarvox: $(LIBNAME) $(LIBPYTHONMODULE)
	@echo "Creating Python module and wrapped library"
	@cp $(LIBNAME) $(LIBPYTHONLIBRARY)
	@cp $(LIBPYTHONMODULE) $(OPUTDIR)

$(EXAMPLE_APP) : $(EXAMPLE_APP_OBJECT) $(LIBNAME) Makefile
	@echo "- Building app $@"
	@$(CXX) $(ROOTLIBS) -L$(OPUTDIR) -llarvox $< $(OutPutOpt) $@
	@echo "#!/bin/bash" > $(OPUTDIR)/run_example
	@echo "LD_LIBRARY_PATH=$(OPUTDIR):$$LD_LIBRARY_PATH $(OPUTDIR)/example \$$1" >> $(OPUTDIR)/run_example
	@chmod u+x $(OPUTDIR)/run_example

%.o: %.hh

%.o: %.hpp

$(OPUTDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "- Building C++ object $@"
	@mkdir -p tmp 2>/dev/null
	@$(CXX) $(CXXFLAGS) -I$(SRCDIR) -I$(INCDIRS) -c $^ -o $@

%.o: %.cc
	@echo "- Building C++ object $@"
	@mkdir -p tmp 2>/dev/null
	@$(CXX) $(CXXFLAGS) -I$(SRCDIR) -I$(INCDIRS) -c $^ -o $@

$(OPUTDIR)/%.o: $(APPDIR)/%.cpp
	@echo "- Building C++ object $@"
	@mkdir -p tmp 2>/dev/null
	@$(CXX) $(CXXFLAGS) -I$(SRCDIR) -I$(INCDIRS) -c $^ -o $@



install: all
	@echo "- Installing larvox in $(prefix)"
	@echo "- installing directory $(includedir)"
	@install -d $(includedir)/$(package)
	@echo "- installing headers in $(includedir)/$(package)"
	@install -m 644 $(LIBHEADERS) $(includedir)/$(package)
	@echo "- installing directory $(libdir)"
	@install -d $(libdir)
	@echo "- installing libraries in $(libdir)"
	@install $(LIBNAME) $(libdir)
	@echo "- installing python modules in $(pylibdir)"
	@install -d $(pylibdir)
	@install $(LIBPYTHONLIBRARY) $(pylibdir)
	@install -m 644 $(LIBPYTHONMODULE) $(pylibdir)
	@echo "- byte-compiling python modules"
	@python -c "import py_compile; py_compile.compile('$(pylibdir)/larvox.py')"
	@python -O -c "import py_compile; py_compile.compile('$(pylibdir)/larvox.py')"
	@echo "- install done..."



clean:
	@echo "- Cleaning up"
	@rm -Rf tmp


package_source:
	@rm -Rf .pkg 2>/dev/null
	@rm -f $(package)-$(version).tar.bz2 2>/dev/null
	@mkdir -p .pkg/$(package)-$(version)
	@cp -r src .pkg/$(package)-$(version)
	@cp -r pylatte .pkg/$(package)-$(version)
	@cp -r apps .pkg/$(package)-$(version)
	@cp Makefile .pkg/$(package)-$(version)
	@cp README .pkg/$(package)-$(version)
	@cp LICENSE .pkg/$(package)-$(version)
	@cd .pkg ; tar -jcvf ../$(package)-$(version).tar.bz2 $(package)-$(version)



#prefix = /home/droythorne/local
#larvox_version_number = 0.1
#larvox_root = $(prefix)/LArVox/$(larvox_version_number)
#LIBRARIES=
#VOXEL_LIBRARIES=

#flags_for_gcc= -Wextra -Wfatal-errors -Wwrite-strings -Woverloaded-virtual -fno-nonansi-builtins -fno-gnu-keywords -fstrict-aliasing
#local_CFLAGS = -g $(flags_for_gcc)
#local_LDFLAGS= 
#ROOT_LIBS:=$(shell root-config --libs) -lSpectrum 
#ROOT_CFLAGS=`root-config --cflags`
#ROOT_INCLUDE=`root-config --incdir`
#INCFLAGS = -I $(larvox_root)/src -I /usr/include/boost -I $(prefix) -I $(prefix)/include  -I /usr/include/tnt_126 -I /usr/include/jama125 -I $(ROOT_INCLUDE) -I /usr/local/include/tclap

#LARVOX_SRC =  src/my_kdrecon_classes/HarrisPlessey.cpp CmdLineParser.cpp LArVoxTest_HP.cpp
#VOXEL_SRC = $(addprefix src/, voxels/Voxel.cpp voxels/VoxelSetTransforms.cpp voxels/TreeToVoxSet.cpp voxels/VoxSetToTree.cpp voxels/VoxelTree.cpp voxels/VoxelSet3DKDConv.cpp voxels/VoxelNonMaxSupp.cpp voxels/TVoxel.cpp) 
#LARVOX_OBJ = $(LARVOX_SRC:.cpp=.o)
#VOXEL_OBJ = $(VOXEL_SRC:.cpp=.o) tvoxeldict.o
#BIN = CmdLineParserTest LArVoxTest_HP
#CmdLineParserTest : CmdLineParserTest.o CmdLineParser.o
#	g++ $(CFLAGS) -o $@ $^
#LArVoxTest_HP : $(VOXEL_OBJ) $(LARVOX_OBJ)
#	g++ $(CFLAGS) $(ROOT_CFLAGS) -o $@ $^ $(local_LDFLAGS) $(ROOT_LIBS) 
#might be necessary to remove -fPIC flag for all code that is not used to compile libraries for root
#%.o : %.cpp
#	g++ -fPIC $(CFLAGS) $(local_CFLAGS) -c $< $(INCFLAGS) -o $(larvox_root)/$@
#tvoxeldict.cpp: src/voxels/TVoxel.hpp 
#	@rootcint $@ -c -p $^ src/voxels/TVoxel_linkdef.h 
#.PHONY: clean
#clean:
#	rm -f $(BIN) ./*.o
