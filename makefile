FLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Wshadow -v
LIBRARIES = -lSDL2 -lSDL2_image -ltinyxml2
MORE_LIBRARIES = -lSDL2_ttf -lSDL2_mixer
FILES = code/*.cpp
EXECUTABLE = platformer

run:
	g++ $(FILES) -o $(EXECUTABLE) $(FLAGS) $(LIBRARIES)
	./$(EXECUTABLE)

san:
	g++ $(FILES) -o $(EXECUTABLE) -g $(FLAGS) $(LIBRARIES) -fsanitize=address -fsanitize=undefined 
	./$(EXECUTABLE)

debug:
	g++ $(FILES) -o $(EXECUTABLE) -g $(FLAGS) $(LIBRARIES)

fast:
	g++ $(FILES) -o $(EXECUTABLE) -O3 $(FLAGS) $(LIBRARIES)
	./$(EXECUTABLE)

profile:
	g++ $(FILES) -g -o $(EXECUTABLE) -O3 $(FLAGS) $(LIBRARIES)
	./$(EXECUTABLE)

clean:
	rm $(EXECUTABLE)