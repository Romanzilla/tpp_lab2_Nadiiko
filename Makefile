CXX = x86_64-w64-mingw32-g++
CXXFLAGS = -std=c++17 -O2 -Wall -mwindows -municode
LDFLAGS = -lgdi32 -luser32

all: task1.exe task2.exe

task1.exe: task1_graph.cpp
	$(CXX) $(CXXFLAGS) task1_graph.cpp -o task1.exe $(LDFLAGS)

task2.exe: task2_scene.cpp
	$(CXX) $(CXXFLAGS) task2_scene.cpp -o task2.exe $(LDFLAGS)

clean:
	rm -f task1.exe task2.exe
