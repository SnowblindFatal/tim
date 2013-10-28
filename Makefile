#g++ -std=c++11 arkanoid.cc -I ~/SFML-2.1/include -lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio -L ~/SFML-2.1/lib -Wl,-rpath=~/SFML-2.1/lib -Wl,-rpath-link=~/SFML-2.1/lib -Wall
SFML=/usr
CC=g++
CFLAGS=-c -g -std=c++0x -Wall -Wextra -pedantic -I $(SFML)/include
LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -L $(SFML)/lib -Wl,-rpath=$(SFML)/lib -Wl,-rpath-link=$(SFML)/lib
SOURCES=src/main.cpp src/game/BaseGame.cpp src/game/EditMode.cpp src/game/PlayMode.cpp src/game/GameState.cpp src/game/MainMenu.cpp src/game/PlayingField.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=machine

all: $(SOURCES) $(EXECUTABLE)
  
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@ 

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
