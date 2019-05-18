BUILDDIR = build
SOURCEDIR = src

CXX = g++-8
CXXFLAGS = -O3 -Wall -std=c++17 -I$(SOURCEDIR) -c -g
LDFLAGS = -L. -lSDL2 -lSDL2_image -lSDL2_ttf -lpthread -lstdc++fs -Wl,-rpath,'$$ORIGIN',--Bstatic -lSDL2_fontcache -Wl,-Bdynamic

EXEC_FILE = MCTD3
OBJ = main.o mctd3.o logger.o simpleui.o main/fields.o main/functions.o main/init.o main/options.o main/loops.o
OBJECTS = $(patsubst %, $(BUILDDIR)/%, $(OBJ))

all: $(EXEC_FILE)

run: $(EXEC_FILE)
	./$(EXEC_FILE)

debug: $(EXEC_FILE)
	./$(EXEC_FILE) -d

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp
	mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILDDIR)/main/%.o: $(SOURCEDIR)/main/%.cpp
	mkdir -p $(BUILDDIR)/main/
	$(CXX) $(CXXFLAGS) -o $@ $^

$(EXEC_FILE): $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(BUILDDIR)
	rm -f $(EXEC_FILE)

remake: clean $(EXEC_FILE)