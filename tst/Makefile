CFLAG = -g -c -std=c++17 -fmax-errors=1 -pthread
INCLUDE = -I../ -I/usr/include/jsoncpp 
CC = g++
TEST_EXE = $(patsubst %.cpp, ../obj/%.tx, $(wildcard *.cpp))
TEST_OBJ = $(patsubst %.cc, ../obj/%.t, $(wildcard *.cc))
GTKCFLAG = $(shell pkg-config gtkmm-3.0 --cflags) #-I$(ROOT)

all : $(TEST_EXE) $(TEST_OBJ)

../obj/%.t : %.cc
	$(CC) $(INCLUDE) $(CFLAG) $(GTKCFLAG) $< -o $@

../obj/%.tx : %.cpp
	$(CC) $(INCLUDE) $(CFLAG) $(GTKCFLAG) $< -o $@
