TARGET = trabajo1

SRCS = main.cpp funciones.cpp interfaz.cpp  # Agregado procesar.cpp
OBJS = $(SRCS:.cpp=.o)

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

clean-all: clean
	rm -f holi.env

.PHONY: all clean clean-all
