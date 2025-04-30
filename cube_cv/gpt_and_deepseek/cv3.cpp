#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

int main() {
    // Load image
    cv::Mat image = cv::imread("images/cube_face.jpeg");
    if (image.empty()) {
        std::cerr << "Could not load image.\n";
        return -1;
    }

    // Convert to grayscale and blur
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 0);

    // Canny edge detection
    cv::Mat edges;
    cv::Canny(gray, edges, 25, 60);

    // Find contours
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Filter square-like contours
    std::vector<cv::Rect> facelets;
    for (const auto& contour : contours) {
        cv::Rect rect = cv::boundingRect(contour);
        float aspectRatio = static_cast<float>(rect.width) / rect.height;
        if (aspectRatio > 0.8f && aspectRatio < 1.2f && rect.area() > 500) {
            facelets.push_back(rect);
        }
    }

    if (facelets.size() != 9) {
        std::cerr << "Expected 9 facelets, but found " << facelets.size() << "\n";
        return -1;
    }

    // Sort facelets top to bottom, then left to right
    std::sort(facelets.begin(), facelets.end(), [](const cv::Rect& a, const cv::Rect& b) {
        if (std::abs(a.y - b.y) < 20)
            return a.x < b.x;
        return a.y < b.y;
    });

    // Extract average colors from facelets
    cv::Vec3b colorMatrix[3][3];
    for (int i = 0; i < 9; ++i) {
        cv::Rect roi = facelets[i];
        cv::Mat faceletROI = image(roi);
        cv::Scalar avgColor = cv::mean(faceletROI);  // BGR average

        int row = i / 3;
        int col = i % 3;
        colorMatrix[row][col] = cv::Vec3b(avgColor[0], avgColor[1], avgColor[2]);  // BGR
    }

    // Print 3x3 color matrix in RGB
    std::cout << "Color matrix (RGB):\n";
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cv::Vec3b c = colorMatrix[i][j];
            std::cout << "[" << (int)c[2] << "," << (int)c[1] << "," << (int)c[0] << "] ";
        }
        std::cout << std::endl;
    }

    // // Optional: show contours for debugging
    // cv::Mat contourImage = image.clone();
    // for (const auto& rect : facelets)
    //     cv::rectangle(contourImage, rect, cv::Scalar(0, 255, 0), 2);
    // cv::imshow("Detected Facelets", contourImage);
    // cv::waitKey(0);
    // cv::destroyAllWindows();

    return 0;
}
