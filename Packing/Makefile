BUILDDIR = build
SOURCEDIR = src

CXX = g++
CXXFLAGS = -O3 -Wall -Weffc++ -ansi -pedantic -std=c++17 -I$(SOURCEDIR) -c -g
LDFLAGS = -L. -lstdc++fs -Wl,-rpath,'$$ORIGIN'

EXEC_FILE = packer
OBJ = packer.o
OBJECTS = $(patsubst %, $(BUILDDIR)/%, $(OBJ))

all: $(EXEC_FILE)

run: $(EXEC_FILE)
	./$(EXEC_FILE)

valgrind: $(EXEC_FILE)
	valgrind --track-origins=yes --leak-check=full ./$(EXEC_FILE) -d

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
