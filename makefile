CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -O3 -fopenmp
LDFLAGS = -fopenmp

TARGET = raytracer

.PHONY: all clean

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp $(LDFLAGS)

clean:
	rm -f $(TARGET) output.ppm
