comp:
	/Applications/CMake.app/Contents/bin/cmake --build /Users/benkwok/Desktop/school.nosync/platformer/build --config Debug --target platformer -j 10

run:
	/Users/benkwok/Desktop/school.nosync/platformer/build/platformer

debug:
	/Applications/CMake.app/Contents/bin/cmake --build /Users/benkwok/Desktop/school.nosync/platformer/build --config Debug --target platformer -j 10
	/Users/benkwok/Desktop/school.nosync/platformer/build/platformer

fast:
	/Applications/CMake.app/Contents/bin/cmake --build /Users/benkwok/Desktop/school.nosync/platformer/build --config Release --target platformer -j 10
	/Users/benkwok/Desktop/school.nosync/platformer/build/platformer

