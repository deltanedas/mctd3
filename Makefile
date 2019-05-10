CXX = g++-8
CXXFLAGS = -O3 -Wall -std=c++17 -I. -c -g
LDFLAGS = -L. -lSDL2 -lSDL2_image -lSDL2_ttf -lpthread -lstdc++fs -Wl,-rpath,'$$ORIGIN',--Bstatic -lSDL2_fontcache -Wl,-Bdynamic

BUILDDIR = build

EXEC_FILE = MCTD3
OBJ = main.cpp.o mctd3.cpp.o logger.cpp.o simpleui.cpp.o
OBJECTS = $(patsubst %, $(BUILDDIR)/%, $(OBJ))

all: $(EXEC_FILE)

run: $(EXEC_FILE)
	./$(EXEC_FILE)

debug: $(EXEC_FILE)
	./$(EXEC_FILE) -d

$(BUILDDIR)/%.cpp.o: %.cpp
	mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(EXEC_FILE): $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(BUILDDIR)/*.cpp.o
	rm -f $(EXEC_FILE)
