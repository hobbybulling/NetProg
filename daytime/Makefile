CXX = g++
CXXFLAGS = -Wall -g -std=c++17
TARGET = daytime_client

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp

clean:
	rm -f $(TARGET)
