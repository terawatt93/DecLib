CXX = `root-config --cxx`
CXXFLAGS = `root-config --cflags` -fPIC -g -Wall
ROOTLIBS = `root-config --glibs` 
SHARED = -shared
SRCS = DecLibTestDict.cxx DecLibTest.cpp
HDRS = DecLibTestLinkDef.hh DecLibTest.hh
PROGRAM = DecLibTest.so

all: $(PROGRAM)

DecLibTestDict.cxx: $(HDRS) DecLibTestLinkDef.hh
	@echo "Generating dictionary ..."
	#@rootcint -f $@ -c -p $^
	@rootcling -f DecLibTestDict.cxx -rml DecLibTest.so -rmf DecLibTestDict.rootmap DecLibTest.hh DecLibTestLinkDef.hh 

$(PROGRAM): $(SRCS)
	@echo "Building $(PROGRAM) ..."
	@rm -f $(PROGRAM)
	@$(CXX) $(CXXFLAGS) $(SHARED) -o $@ $^ $(ROOTLIBS)
	@echo "done"
#options:
install:; @echo 'export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:'`pwd`>>~/.bashrc; echo 'export CPATH=$$CPATH:'`pwd`>>~/.bashrc
clean:; @rm -rf core *.so *.rootmap *.cxx *.pcm
