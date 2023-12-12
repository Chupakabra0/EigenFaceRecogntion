#include "SilenceLogger.hpp"

SilenceLogger::SilenceLogger()
	: AppLogger(std::cerr) {

}

void SilenceLogger::PrintMatrix(const cv::Mat& matrix, const std::string& matrixName) {
	// Thx to Ann-Alex for surviving ten years
}

void SilenceLogger::PrintImage(const cv::Mat& image, const std::string& imageName) {

}

void SilenceLogger::PrintMessage(const std::string& message) {

}
