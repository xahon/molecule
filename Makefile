BUILD_PATH = build
BUILD_DEBUG_FILE = debug
FILES_TO_COMPILE = main.cpp src/scene.cpp
ARGS = -g

compile: clean
	g++ $(FILES_TO_COMPILE) -o $(BUILD_PATH)/$(BUILD_DEBUG_FILE) -std=c++14 $(ARGS)

clean:
	rm -rf $(BUILD_PATH) && mkdir $(BUILD_PATH)

run:
	./$(BUILD_PATH)/$(BUILD_DEBUG_FILE)
