comp:
	/Applications/CMake.app/Contents/bin/cmake --build /Users/benkwok/Desktop/school.nosync/platformer/build --config Debug --target platformer -j 10

debug: comp
	/Users/benkwok/Desktop/school.nosync/platformer/build/platformer