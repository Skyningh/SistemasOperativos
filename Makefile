TARGETS = trabajo1 procesar conteo_thread inverted_index

SRCS1 = main.cpp funciones.cpp interfaz.cpp
OBJS1 = $(SRCS1:.cpp=.o)

SRCS2 = procesar.cpp funciones.cpp interfaz.cpp
OBJS2 = $(SRCS2:.cpp=.o)

SRCS3 = conteo_thread.cpp funciones.cpp interfaz.cpp
OBJS3 = $(SRCS3:.cpp=.o)

SRCS4 = inverted_index.cpp funciones.cpp interfaz.cpp
OBJS4 = $(SRCS4:.cpp=.o)

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

all: $(TARGETS)

trabajo1: $(OBJS1)
	$(CXX) $(CXXFLAGS) $(OBJS1) -o trabajo1

procesar: $(OBJS2)
	$(CXX) $(CXXFLAGS) $(OBJS2) -o procesar

conteo_thread: $(OBJS3)
	$(CXX) $(CXXFLAGS) $(OBJS3) -o conteo

inverted_index: $(OBJS4)
	$(CXX) $(CXXFLAGS) $(OBJS4) -o ii

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) *.o

.PHONY: all clean
