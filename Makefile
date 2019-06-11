BUILDDIR = build
OBJECTDIR = objects
SOURCEDIR = src

CXX = g++
CXXFLAGS = -O3 -Wall -Weffc++ -ansi -pedantic -std=c++17 -I$(SOURCEDIR) -c -g
LDFLAGS = -L$(SOURCEDIR) -llua5.3 -lSDL2 -lSDL2_image -lSDL2_ttf -lpthread -lstdc++fs -Wl,-rpath,'$$ORIGIN',--Bstatic -lSDL2_fontcache -Wl,-Bdynamic

EXEC_FILE = MCTD3
OBJ = main mctd3 main/fields main/functions main/init main/options main/loops
OBJECTS = $(patsubst %, $(OBJECTDIR)/%.o, $(OBJ))

DEBUGGER = valgrind
DEBUGGERFLAGS = --track-origins=yes --leak-check=full

all: $(EXEC_FILE) packer

run: $(EXEC_FILE)
	$(BUILDDIR)/$(EXEC_FILE)

debug: $(EXEC_FILE)
	$(DEBUGGER) $( $(BUILDDIR)/$(EXEC_FILE) -d

$(OBJECTDIR)/%.o: $(SOURCEDIR)/%.cpp
	mkdir -p $(OBJECTDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJECTDIR)/main/%.o: $(SOURCEDIR)/main/%.cpp
	mkdir -p $(OBJECTDIR)/main/
	$(CXX) $(CXXFLAGS) -o $@ $^

$(EXEC_FILE): $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(OBJECTDIR)
	rm -f $(BUILDDIR)/$(EXEC_FILE)

remake: clean $(EXEC_FILE)

packer:
	cd packer
	make
