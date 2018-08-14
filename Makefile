csrc = $(wildcard src/*.c)
ccsrc = $(wildcard src/*.cpp) 
obj = $(csrc:.c=.o) $(ccsrc:.cpp=.o)

LDFLAGS = -lglfw3 -lGL -lX11 -lXi -lXrandr -ldl -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread
idir=-Iinclude -I/home/julian/Programs/glfw-build/include
ccflags=-std=c++17 -Wall

target=particles-demo

%.o: %.cpp
	g++ -c $< $(idir) $(ccflags) -o $@

all: $(obj)
	g++ $(obj) $(LDFLAGS) -o $(target) $(ccflags) && tree

.PHONY: clean

clean:
	rm -rf src/*.o $(target); tree
