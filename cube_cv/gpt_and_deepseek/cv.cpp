#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
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

    // Edge detection
    cv::Mat edges;
    cv::Canny(gray, edges, 20,60);

    // Find contours
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    cout<<"hello"<<endl;
    // Filter and process square contours
    for (const auto& contour : contours) {
        double area = cv::contourArea(contour);
        if (area < 100) continue; // skip small regions

        std::vector<cv::Point> approx;
        cv::approxPolyDP(contour, approx, 0.02 * cv::arcLength(contour, true), true);
        cout<<"hi"<<endl;
        cout<<"approx size:"<<approx.size()<<endl;
        for(int i=0;i<approx.size();i++){
            cout<<approx[i]<<endl;
        }
        if (approx.size() == 4 && cv::isContourConvex(approx)) {
            // Get bounding box around the detected square
            cv::Rect boundingBox = cv::boundingRect(approx);

            // Crop the region of interest (ROI) from the image
            cv::Mat roi = image(boundingBox);

            // Calculate the average color in the ROI
            cv::Scalar avgColor = cv::mean(roi);

            // Display the average color in BGR format
            std::cout << "Average Color (BGR): " 
                      << "Blue: " << avgColor[0] 
                      << ", Green: " << avgColor[1] 
                      << ", Red: " << avgColor[2] << std::endl;

            // Create a 3x3 matrix representing the color in the BGR format
            cv::Mat colorMatrix = cv::Mat::zeros(3, 3, CV_8UC3);
            colorMatrix.setTo(cv::Scalar(avgColor[0], avgColor[1], avgColor[2]));

            // Display the 3x3 color matrix
            std::cout << "3x3 Color Matrix:\n" << colorMatrix << std::endl;
        }
    }

    return 0;
}
