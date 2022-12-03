CC = g++
PROJECT = button
SRC = button.cpp
LIBS = `wx-config --libs --cxxflags`
$(PROJECT): $(SRC)
	$(CC) $(SRC) $(LIBS) -o $(PROJECT)