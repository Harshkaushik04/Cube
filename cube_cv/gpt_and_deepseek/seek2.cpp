#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

// Comparator to sort rectangles by their (y, x) positions (row-wise)
bool sortRectangles(const cv::Rect &a, const cv::Rect &b) {
    if (a.y < b.y - 10) return true;       // Different row (higher y = lower row)
    else if (a.y > b.y + 10) return false; // Different row
    else return a.x < b.x;                 // Same row, sort by x
}

int main() {
    // Load image
    cv::Mat image = cv::imread("images/cube_face.jpeg");
    if (image.empty()) {
        std::cerr << "Error: Could not load image." << std::endl;
        return -1;
    }

    // Convert to grayscale
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // Apply adaptive thresholding
    cv::Mat thresh;
    cv::adaptiveThreshold(gray, thresh, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C,
                          cv::THRESH_BINARY_INV, 11, 2);

    // Morphological closing to connect edges
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(thresh, thresh, cv::MORPH_CLOSE, kernel);

    // Find contours
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Filter and store valid facelets (rectangles)
    std::vector<cv::Rect> facelets;
    for (const auto &contour : contours) {
        cv::Rect rect = cv::boundingRect(contour);
        float aspectRatio = (float)rect.width / rect.height;
        int area = rect.area();

        // Filter based on size and aspect ratio (adjust thresholds as needed)
        if (area > 1000 && area < 50000 && aspectRatio > 0.8 && aspectRatio < 1.2) {
            facelets.push_back(rect);
        }
    }

    // Ensure exactly 9 facelets (for a 3x3 cube)
    if (facelets.size() != 9) {
        std::cerr << "Error: Detected " << facelets.size() << " facelets (expected 9)." << std::endl;
        return -1;
    }

    // Sort facelets in grid order (top-left to bottom-right)
    std::sort(facelets.begin(), facelets.end(), sortRectangles);

    // Create a 3x3 matrix (for storing facelet data, e.g., colors)
    std::vector<std::vector<cv::Vec3b>> faceletMatrix(3, std::vector<cv::Vec3b>(3));

    // Extract dominant color from each facelet (or other features)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int index = i * 3 + j;
            cv::Rect rect = facelets[index];
            cv::Mat faceletROI = image(rect);

            // Get average color (simplified; replace with better color detection)
            cv::Scalar avgColor = cv::mean(faceletROI);
            faceletMatrix[i][j] = cv::Vec3b(avgColor[0], avgColor[1], avgColor[2]);

            // Draw rectangle and index (for debugging)
            cv::rectangle(image, rect, cv::Scalar(0, 255, 0), 2);
            cv::putText(image, std::to_string(i * 3 + j), rect.tl() + cv::Point(10, 30),
                        cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
        }
    }

    // Display the result
    cv::imshow("Detected 3x3 Facelets", image);
    cv::waitKey(0);

    // Print the 3x3 matrix (example: average BGR values)
    std::cout << "3x3 Facelet Matrix (BGR values):" << std::endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cv::Vec3b color = faceletMatrix[i][j];
            std::cout << "(" << (int)color[0] << "," << (int)color[1] << "," << (int)color[2] << ") ";
        }
        std::cout << std::endl;
    }

    return 0;
}