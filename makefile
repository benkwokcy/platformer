comp:
	/Applications/CMake.app/Contents/bin/cmake --build /Users/benkwok/Desktop/school.nosync/platformer/build --config Debug --target platformer -j 10

run:
	/Users/benkwok/Desktop/school.nosync/platformer/build/platformer

debug: comp
	/Users/benkwok/Desktop/school.nosync/platformer/build/platformer

