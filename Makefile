TARGET = planet_sim
BUILD_DIR = bin
SRC_DIR = src
OBJ_DIR = obj
GLAD_DIR = glad
GENERATOR = c

CXX=clang++
CC=clang
CCFLAGS=-I$(GLAD_DIR)/include
CXXFLAGS=-Wall -std=c++14 -I/usr/local/include -I$(GLAD_DIR)/include
LDFLAGS=-lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES)) 

.PHONY: clean all debug

all: executable

debug: GENERATOR = c-debug
debug: CXXFLAGS += -DDEBUG -g
debug: CCFLAGS += -DDEBUG -g
debug: executable

executable: mkBin mkObj glad $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJ_FILES) $(OBJ_DIR)/glad.o
	$(CXX) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp 
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/glad.o: $(GLAD_DIR)/src/glad.c
	$(CC) $(CCFLAGS) -c -o $@ $<

mkBin: 
	mkdir -p $(BUILD_DIR)

mkObj:
	mkdir -p $(OBJ_DIR)

glad:
	mkdir -p $(GLAD_DIR)
	glad --generator=$(GENERATOR) --api gl=3.3 --profile core --out-path glad/

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(OBJ_DIR)

super_clean: clean
	rm -rf $(GLAD_DIR)
