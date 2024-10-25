TARGETS = trabajo1 procesar conteo_thread inverted_index planificador distribuidor core ejecutador

SRCS1 = main.cpp funciones.cpp interfaz.cpp
OBJS1 = $(SRCS1:.cpp=.o)

SRCS2 = procesar.cpp funciones.cpp interfaz.cpp
OBJS2 = $(SRCS2:.cpp=.o)

SRCS3 = conteo_thread.cpp funciones.cpp interfaz.cpp
OBJS3 = $(SRCS3:.cpp=.o)

SRCS4 = inverted_index.cpp funciones.cpp interfaz.cpp
OBJS4 = $(SRCS4:.cpp=.o)

SRCS5 = planificador.cpp
OBJS5 = $(SRCS5:.cpp=.o)

SRCS6 = distribuidor.cpp
OBJS6 = $(SRCS6:.cpp=.o)

SRCS7 = core.cpp
OBJS7 = $(SRCS7:.cpp=.o)

SRCS8 = ejecutador.cpp funciones.cpp
OBJS8 = $(SRCS8:.cpp=.o)

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

all: $(TARGETS)

trabajo1: $(OBJS1)
	$(CXX) $(CXXFLAGS) $(OBJS1) -o trabajo1

procesar: $(OBJS2)
	$(CXX) $(CXXFLAGS) $(OBJS2) -o procesar

conteo_thread: $(OBJS3)
	$(CXX) $(CXXFLAGS) $(OBJS3) -o conteo_thread

inverted_index: $(OBJS4)
	$(CXX) $(CXXFLAGS) $(OBJS4) -o inverted_index

planificador: $(OBJS5)
	$(CXX) $(CXXFLAGS) $(OBJS5) -o planificador

distribuidor: $(OBJS6)
	$(CXX) $(CXXFLAGS) $(OBJS6) -o distribuidor

core: $(OBJS7)
	$(CXX) $(CXXFLAGS) $(OBJS7) -o core

ejecutador: $(OBJS8)
	$(CXX) $(CXXFLAGS) $(OBJS8) -o ejecutador

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) *.o

.PHONY: all clean
