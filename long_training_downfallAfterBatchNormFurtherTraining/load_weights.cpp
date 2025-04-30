#include <torch/script.h>  
#include <iostream>

int main() {
    try {
        // Load the model
        torch::jit::script::Module module = torch::jit::load("weights/best.pt");

        // Iterate through parameters (weights)
        for (const auto& param : module.parameters()) {
            std::cout << "Shape: " << param.sizes() << std::endl;
            std::cout << param << std::endl;  // Prints weights
        }
    } catch (const c10::Error& e) {
        std::cerr << "Error loading model: " << e.what() << std::endl;
    }
    return 0;
}
