# Compiler and Flags
CXX = g++

# Torch Paths (Set to your actual LibTorch installation path)
TORCH_DIR = /home/harsh/libtorch
TORCH_INCLUDE = -I$(TORCH_DIR)/include -I$(TORCH_DIR)/include/torch/csrc/api/include
TORCH_LIBS = -L$(TORCH_DIR)/lib -Wl,--no-as-needed -ltorch -ltorch_cuda -lc10 -ltorch_cpu

# Output directory
OUTPUT_DIR = outputs
TARGET = $(OUTPUT_DIR)/cube

# Ensure output directory exists
$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

# Build the executable
$(TARGET): cube.cpp env.cpp | $(OUTPUT_DIR)
	$(CXX) $(TORCH_INCLUDE) main.cpp agent.cpp model.cpp cube.cpp env.cpp -o $(TARGET) $(TORCH_LIBS)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean the build files
clean:
	rm -f $(TARGET)


