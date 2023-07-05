LIB_GLUT_WINDOWS = -lglfw3dll -lglu32 -lglut32 -lopengl32 -lglew32
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

fromhead:
	g++ -g -std=c++17 -static -I./include src/\*cpp -lglfw3dll -lglu32 -lglut32 -lopengl32 -lglew32 -o main.exe

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

# $(BUILD_DIR)/wave.o: src/wave.cpp ./include/shader.h ./include/Camera.h ./include/sphere.h ./include/Ocean.hpp ./include/Height.hpp ./include/Philipps.hpp ./include/glad/glad.h 
# 	$(CC) $(INCLUDE) $(CC_FLAGS) -o $@ -c $<
# $(BUILD_DIR)/CubeMap.o: src/CubeMap.cpp ./include/CubeMap.hpp
# 	$(CC) $(INCLUDE) $(CC_FLAGS) -o $@ -c $<

# $(BUILD_DIR)/Camera.o: src/Camera.cpp ./include/Camera.h
# 	$(CC) $(INCLUDE) $(CC_FLAGS) -o $@ -c $<

# $(BUILD_DIR)/FFT.o: src/FFT.cpp ./include/FFT.hpp
# 	$(CC) $(INCLUDE) $(CC_FLAGS) -o $@ -c $<

# $(BUILD_DIR)/Height.o: src/Height.cpp ./include/Height.hpp
# 	$(CC) $(INCLUDE) $(CC_FLAGS) -o $@ -c $<

# $(BUILD_DIR)/Ocean.o: src/Ocean.cpp ./include/Ocean.hpp ./include/Height.hpp
# 	$(CC) $(INCLUDE) $(CC_FLAGS) -o $@ -c $<

# $(BUILD_DIR)/Philipps.o: src/Philipps.cpp include/Philipps.hpp
# 	$(CC) $(INCLUDE) $(CC_FLAGS) -o $@ -c $<

# $(BUILD_DIR)/shader.o: src/shader.cpp ./include/shader.h ./include/glad/glad.h
# 	$(CC) $(INCLUDE) $(CC_FLAGS) -o $@ -c $<

# $(BUILD_DIR)/sphere.o: src/sphere.cpp ./include/sphere.h ./include/shader.h ./include/glad/glad.h
# 	$(CC) $(INCLUDE) $(CC_FLAGS) -o $@ -c $<

# $(BUILD_DIR)/Model.o: src/Model.cpp ./include/Model.hpp
# 	$(CC) $(INCLUDE) $(CC_FLAGS) -o $@ -c $<

# $(BUILD_DIR)/wave_czh.o: src/wave_czh.cpp ./include/wave_czh.hpp
# 	$(CC) $(INCLUDE) $(CC_FLAGS) -o $@ -c $<


# $(BUILD_DIR)/stb_image.o: src/stb_image.cpp ./include/stb_image.h
# 	$(CC) $(INCLUDE) $(CC_FLAGS) -o $@ -c $<

# $(BUILD_DIR)/glad.o: src/glad.cpp ./include/glad/glad.h
# 	$(CC) $(INCLUDE) $(CC_FLAGS) -o $@ -c $<

clean:
	$(shell rm -f $(BUILD_DIR)/*.o)
	$(shell rm -rf $(BUILD_DIR))