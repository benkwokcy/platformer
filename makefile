FLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Wshadow
LIBRARIES = -lSDL2 -lSDL2_image -ltinyxml2
MORE_LIBRARIES = -lSDL2_ttf -lSDL2_mixer
FILES = code/*.cpp
EXECUTABLE = platformer

comp:
	g++ $(FILES) -o $(EXECUTABLE) $(FLAGS) $(LIBRARIES)

san:
	g++ $(FILES) -o $(EXECUTABLE) $(FLAGS) $(LIBRARIES) -g -fsanitize=address -fsanitize=undefined
	./$(EXECUTABLE)

fast:
	g++ $(FILES) -o $(EXECUTABLE) $(FLAGS) $(LIBRARIES) -O3
	./$(EXECUTABLE)

profile:
	g++ $(FILES)  -o $(EXECUTABLE) $(FLAGS) $(LIBRARIES) -g -O3
	./$(EXECUTABLE)
	
debug:
	g++ $(FILES) -o $(EXECUTABLE) $(FLAGS) $(LIBRARIES) -g
