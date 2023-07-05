LIB_GLUT_WINDOWS = -lm -lGL -lGLU -lglut -lglfw -lGLEW
CC             = g++
CC_FLAGS       = -static -std=c++17 -g
EXEC           = main

BUILD_DIR = build
BIN_DIR   = .
SRC_DIR   = src
MODULES   = ./
OBJ     = $(foreach sdir, $(SRC_DIR), $(patsubst $(sdir)/%.cpp, $(BUILD_DIR)/%.o, $(wildcard $(sdir)/*.cpp)))

INCLUDE = -I./include
VPATH = $(SRC_DIR), ./include

# all: lib_windows make_dir $(BIN_DIR)/$(EXEC)
all: lib_windows make_dir $(BIN_DIR)/$(EXEC)

# fromhead:
# 	g++ -g -std=c++17 -static -I./include src/\*cpp -lglfw3dll -lglu32 -lglut32 -lopengl32 -lglew32 -o main

make_dir:
	@mkdir -p $(BUILD_DIR)

lib_windows:
	$(eval LD_FLAGS = $(LIB_GLUT_WINDOWS))

# create binary
$(BIN_DIR)/$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LD_FLAGS)

# objects
$(BUILD_DIR)/%.o: src/%.cpp
	$(CC) $(INCLUDE) $(CC_FLAGS) -o $@ -c $<

clean:
	$(shell rm -f $(BUILD_DIR)/*.o)
	$(shell rm -rf $(BUILD_DIR))