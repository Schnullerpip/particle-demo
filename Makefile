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
	$(llvmBuild)/bin/coop

r:
	git reset --hard

.PHONY: clean

clean:
	rm -rf src/*.o $(target)
	rm CMakeCache.txt
	rm cmake_install.cmake
	rm build.ninja
	rm rules.ninja

