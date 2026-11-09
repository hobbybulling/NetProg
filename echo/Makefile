CXX = g++
CXXFLAGS = -Wall -O2 -std=c++17
TARGET = echo_client

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp

clean:
	rm -f $(TARGET)
