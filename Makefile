TARGET = trabajo1

SRCS = main.cpp funciones.cpp

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)

clean-all: clean
	rm -f .env

.PHONY: all clean clean-all
