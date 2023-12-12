#include "MessageLogger.hpp"

MessageLogger::MessageLogger(std::ostream& out)
	: AppLogger(out) {

}

void MessageLogger::PrintMatrix(const cv::Mat& matrix, const std::string& matrixName) {

}

void MessageLogger::PrintImage(const cv::Mat& image, const std::string& imageName) {

}

void MessageLogger::PrintMessage(const std::string& message) {
	this->out_.get() << message << std::endl;
}
