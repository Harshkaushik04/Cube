#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Load the image
    cv::Mat image = cv::imread("images/cube_face.jpeg");
    if (image.empty()) {
        std::cerr << "Error: Could not load the image." << std::endl;
        return -1;
    }

    // Get image dimensions
    int width = image.cols;  // Width (number of columns)
    int height = image.rows; // Height (number of rows)

    // Calculate aspect ratio (width / height)
    float aspectRatio = static_cast<float>(width) / height;

    // Print results
    std::cout << "Image Width: " << width << std::endl;
    std::cout << "Image Height: " << height << std::endl;
    std::cout << "Aspect Ratio: " << aspectRatio << std::endl;

    return 0;
}