#include "ImageLogger.hpp"

ImageLogger::ImageLogger()
	: AppLogger(std::cerr) {

}

void ImageLogger::PrintMatrix(const cv::Mat& matrix, const std::string& matrixName) {

}

void ImageLogger::PrintImage(const cv::Mat& image, const std::string& imageName) {
	cv::imshow(imageName, image);
	cv::waitKey();
}

// Thx to Arthur Hackerman for being the best passive Python dev ever

void ImageLogger::PrintMessage(const std::string& message) {

}
