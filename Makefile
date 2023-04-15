CXX=g++
CXXFLAGS=-Wall -pedantic -Wextra -std=c++17 -g

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:src/%.cpp=build/%.o)

.PHONY: all
all: compile

.PHONY: run
run: compile
	./Chess

.PHONY: compile
compile: Chess

Chess: build/main
	cp $< $@

build/main: $(OBJECTS)
	mkdir -p $(@D)
	$(CXX) $^ -o $@

build/%.o: src/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $< -c -o $@

.PHONY: clean
clean:
	rm -rf Chess build/

deps:
	$(CXX) -MM src/*cpp


build/Bishop.o: src/Bishop.cpp src/Bishop.hpp src/ChessFigure.hpp \
 src/Position.hpp src/Board.hpp
build/Board.o: src/Board.cpp src/Board.hpp src/ChessFigure.hpp src/Position.hpp \
 src/construct_laout.hpp src/King.hpp src/Queen.hpp src/Rook.hpp \
 src/Knight.hpp src/Bishop.hpp src/Pawn.hpp
build/ChessFigure.o: src/ChessFigure.cpp src/ChessFigure.hpp src/Position.hpp \
 src/Board.hpp
build/Game.o: src/Game.cpp src/Game.hpp src/Board.hpp src/ChessFigure.hpp \
 src/Position.hpp src/Player.hpp src/Pawn.hpp src/check_input.hpp \
 src/Player_real.hpp
build/King.o: src/King.cpp src/King.hpp src/ChessFigure.hpp src/Position.hpp \
 src/check_input.hpp src/Board.hpp
build/Knight.o: src/Knight.cpp src/Knight.hpp src/ChessFigure.hpp \
 src/Position.hpp src/Board.hpp
build/Menu.o: src/Menu.cpp src/Menu.hpp src/Game.hpp src/Board.hpp \
 src/ChessFigure.hpp src/Position.hpp src/Player.hpp src/check_input.hpp \
 src/Player_real.hpp
build/Pawn.o: src/Pawn.cpp src/Pawn.hpp src/ChessFigure.hpp src/Position.hpp \
 src/Board.hpp
build/Player.o: src/Player.cpp src/Player.hpp src/Board.hpp src/ChessFigure.hpp \
 src/Position.hpp
build/Player_AI_1.o: src/Player_AI_1.cpp src/Player_AI_1.hpp src/Player.hpp \
 src/Board.hpp src/ChessFigure.hpp src/Position.hpp
build/Player_real.o: src/Player_real.cpp src/Player_real.hpp src/Player.hpp \
 src/Board.hpp src/ChessFigure.hpp src/Position.hpp src/check_input.hpp
build/Position.o: src/Position.cpp src/Position.hpp
build/Queen.o: src/Queen.cpp src/Queen.hpp src/ChessFigure.hpp src/Position.hpp \
 src/Board.hpp
build/Rook.o: src/Rook.cpp src/Rook.hpp src/ChessFigure.hpp src/Position.hpp \
 src/Board.hpp
build/check_input.o: src/check_input.cpp src/check_input.hpp src/Position.hpp \
 src/Board.hpp src/ChessFigure.hpp
build/main.o: src/main.cpp src/Menu.hpp src/Game.hpp src/Board.hpp \
 src/ChessFigure.hpp src/Position.hpp src/Player.hpp
