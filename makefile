FLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Wshadow
INCLUDES = -Icode -Icode/entities_and_components -Icode/scenes -Icode/gui
LIBRARIES = -lSDL2 -lSDL2_image -ltinyxml2
MORE_LIBRARIES = -lSDL2_ttf -lSDL2_mixer
FILES = code/*.cpp code/*/*.cpp
EXECUTABLE = platformer

comp:
	g++ $(FILES) -o $(EXECUTABLE) $(FLAGS) $(LIBRARIES) $(INCLUDES)

san:
	g++ $(FILES) -o $(EXECUTABLE) $(FLAGS) $(LIBRARIES) $(INCLUDES) -g -fsanitize=address -fsanitize=undefined
	./$(EXECUTABLE)

fast:
	g++ $(FILES) -o $(EXECUTABLE) $(FLAGS) $(LIBRARIES) $(INCLUDES) -O3
	./$(EXECUTABLE)

profile:
	g++ $(FILES)  -o $(EXECUTABLE) $(FLAGS) $(LIBRARIES) $(INCLUDES) -g -O3
	./$(EXECUTABLE)
	
debug:
	g++ $(FILES) -o $(EXECUTABLE) $(FLAGS) $(LIBRARIES) $(INCLUDES) -g
