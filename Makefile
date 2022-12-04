CC = g++
PROJECT = test
SRC = ingame_frame.cpp
LIBS = `wx-config --libs --cxxflags`
$(PROJECT): $(SRC)
	$(CC) $(SRC) $(LIBS) -o $(PROJECT)