TARGET = planet_sim
BUILD_DIR = bin
SRC_DIR = src
OBJ_DIR = obj

CXX=clang++
CPPFLAGS=-Wall -std=c++14
LDFLAGS=-lGLEW -lGLFW -framework OpenGL

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

.PHONY: clean all 

all: mkBin mkObj $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJ_FILES)
	$(CXX) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp 
	$(CXX) $(CPPFLAGS) -c -o $@ $<

mkBin: 
	mkdir -p $(BUILD_DIR)

mkObj:
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(OBJ_DIR)
