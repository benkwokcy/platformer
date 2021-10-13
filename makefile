BUILD_DIRECTORY=/Users/benkwok/Desktop/school.nosync/other/projects/platformer/build

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