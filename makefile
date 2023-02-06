BUILD_DIRECTORY=./build

init:
	cmake -H. -B build -DCMAKE_BUILD_TYPE=Release

comp:
	cmake --build $(BUILD_DIRECTORY) --config Debug --target platformer -j 4

run:
	$(BUILD_DIRECTORY)/platformer

debug:
	cmake --build $(BUILD_DIRECTORY) --config Debug --target platformer -j 4
	$(BUILD_DIRECTORY)/platformer

fast:
	cmake --build $(BUILD_DIRECTORY) --config Release --target platformer -j 4
	$(BUILD_DIRECTORY)/platformer