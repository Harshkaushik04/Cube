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

    // Convert to grayscale
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // Improve contrast
    cv::Mat equalized;
    cv::equalizeHist(gray, equalized);

    // Adaptive threshold to detect edges better
    cv::Mat thresh;
    cv::adaptiveThreshold(equalized, thresh, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C,
                          cv::THRESH_BINARY_INV, 11, 3);

    // Morphological closing to connect broken lines
    cv::Mat morph;
    cv::morphologyEx(thresh, morph, cv::MORPH_CLOSE,
                     cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)),
                     cv::Point(-1, -1), 2);

    // Find contours
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(morph, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Debugging image
    cv::Mat debugImage = image.clone();
    std::vector<cv::Rect> facelets;
    int index = 0;

    std::cout << "Detected " << contours.size() << " contours.\n";

    for (const auto& contour : contours) {
        cv::Rect rect = cv::boundingRect(contour);
        float aspectRatio = static_cast<float>(rect.width) / rect.height;
        int area = rect.area();

        std::cout << "Contour " << index << ": ";
        std::cout << "x=" << rect.x << ", y=" << rect.y
                  << ", width=" << rect.width << ", height=" << rect.height
                  << ", area=" << area << ", aspectRatio=" << aspectRatio << "\n";

        // Filter out tiny or huge rectangles and enforce square shape
        if (rect.width > 200 && rect.height > 200 &&
            rect.width < 500 && rect.height < 500 ) {
            
            facelets.push_back(rect);

            // Draw rectangle
            cv::rectangle(debugImage, rect, cv::Scalar(0, 255, 0), 2);

            // Only show index + numeric values, no labels
            std::string text = std::to_string(index) + " " +
                               std::to_string(rect.width) + " " +
                               std::to_string(rect.height) + " " +
                               std::to_string(area);

            // Put text inside the rectangle
            cv::putText(debugImage, text, rect.tl() + cv::Point(3, 20),
                        cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(0, 0, 255), 1);
        }

        index++;
    }

    std::cout << "Filtered " << facelets.size() << " valid facelets.\n";

    // Display
    cv::namedWindow("Detected Facelets", cv::WINDOW_NORMAL);  // Enable resizing
    cv::imshow("Detected Facelets", debugImage);
    cv::imwrite("debug_detected_facelets.png", debugImage);  // Optional save
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
