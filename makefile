FLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Wshadow
LIBRARIES = -I/usr/local/include/SDL2 -D_THREAD_SAFE -L/usr/local/lib -lSDL2 -lSDL2_ttf -lSDL2_mixer
FILES = *.cpp
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

clean:
	rm $(EXECUTABLE)