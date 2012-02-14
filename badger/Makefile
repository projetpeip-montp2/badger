EXEC = badger

SRC = $(wildcard src/*.cpp ../console/*.cpp ../serial/src/*.cpp ../serial/src/Posix/*.cpp)
OBJ = $(SRC:.cpp=.o)

CXX = g++

WARNINGSFLAGS = -W -Werror -Wall -Wfatal-errors -pedantic -pedantic-errors

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) -o $(EXEC) $(OBJ) -O2

%.o: %.cpp
	$(CXX) -o $@ -c $< $(WARNINGSFLAGS) -std=c++0x -O2 -I../console/ -I./include/ -I../serial/include -I../serial/src/ -I../serial/src/Unix

.PHONY: clean mrproper

clean:
	rm -rf $(OBJ)

mrproper: clean
	rm -rf ./$(EXEC)
