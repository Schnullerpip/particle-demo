csrc = $(wildcard src/*.c)
ccsrc = $(wildcard src/*.cpp) 
obj = $(csrc:.c=.o) $(ccsrc:.cpp=.o)
DEPS = include/*.h include/*.hpp

LDFLAGS = -lglfw3 -lX11 -lXrandr -ldl -lXxf86vm -lXinerama -lXcursor -pthread

idir=-Iinclude -I/home/julian/Programs/glfw-build/include
ccflags=-std=c++17 -Wall

target=particles-demo

%.o: %.cpp
	g++ -c $< $(idir) $(ccflags) -o $@

all: $(obj)
	g++ $(obj) $(LDFLAGS) -o $(target) $(ccflags) && tree

coop: $(csrc) $(ccsrc)
	$(llvmBuild)/bin/coop src/collision_detection.cpp src/input_management.cpp src/main.cpp src/particle.cpp src/setup.cpp src/Shader.cpp

.PHONY: clean

clean:
	rm -rf src/*.o $(target); tree
