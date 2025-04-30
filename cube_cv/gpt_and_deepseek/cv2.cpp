#include <opencv2/opencv.hpp>
#include <iostream>

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

    // Edge detection using Canny with much looser thresholds
    cv::Mat edges;
    cv::Canny(gray, edges, 25, 60);  // Even looser thresholds (lower values)

    // Find contours
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Create an empty canvas to draw contours (same size as the input image)
    cv::Mat contourImage = cv::Mat::zeros(image.size(), CV_8UC3);

    // Draw all contours (thicker lines for visibility)
    if (!contours.empty()) {
        for (const auto& contour : contours) {
            cv::drawContours(contourImage, contours, -1, cv::Scalar(0, 255, 0), 2);
        }
    } else {
        std::cerr << "No contours detected.\n";
    }

    // Create a window with a custom size
    cv::namedWindow("Contours Only", cv::WINDOW_NORMAL);  // Enable resizing
    cv::resizeWindow("Contours Only", 500, 500);  // Resize the window to 500x500 pixels

    // Show the window with contours only
    cv::imshow("Contours Only", contourImage);

    // Wait for a keypress and close the window properly
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
