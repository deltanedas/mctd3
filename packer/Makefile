BUILDDIR = build
OBJECTDIR = objects
SOURCEDIR = src

CXX = g++
CXXFLAGS = -O3 -Wall -Weffc++ -ansi -pedantic -std=c++17 -I$(SOURCEDIR) -c -g
LDFLAGS = -L. -lstdc++fs -Wl,-rpath,'$$ORIGIN'

EXEC_FILE = packer
OBJ = packer
OBJECTS = $(patsubst %, $(OBJECTDIR)/%.o, $(OBJ))

DEBUGGER = valgrind
DEBUGGERFLAGS = --track-origins=yes --leak-check=full

all: $(EXEC_FILE)

run: $(EXEC_FILE)
	$(BUILDDIR)/$(EXEC_FILE)

debug: $(EXEC_FILE)
	$(DEBUGGER) $(DEBUGGERFLAGS) $(BUILDDIR)/$(EXEC_FILE) -d

$(OBJECTDIR)/%.o: $(SOURCEDIR)/%.cpp
	mkdir -p `dirname $@`
	$(CXX) $(CXXFLAGS) -o $@ $^

$(EXEC_FILE): $(OBJECTS)
	mkdir -p $(BUILDDIR)
	$(CXX) -o $(BUILDDIR)/$@ $^ $(LDFLAGS)

clean:
	rm -rf $(OBJECTDIR)
	rm -f $(BUILDDIR)/$(EXEC_FILE)

remake: clean $(EXEC_FILE)
