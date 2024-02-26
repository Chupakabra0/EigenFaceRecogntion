#include "DefaultAppLogger.hpp"

DefaultAppLogger::DefaultAppLogger(std::ostream& out)
	: out_(out) {

}

void DefaultAppLogger::PrintMatrix(const cv::Mat& matrix, const std::string& matrixName) {
	if (!matrixName.empty()) {
		this->out_.get() << matrixName << ":" << std::endl;
	}

	this->out_.get() << "Type: " << matrix.type() << std::endl;
	this->out_.get() << "Rows: " << matrix.rows << std::endl;
	this->out_.get() << "Cols: " << matrix.cols << std::endl;

	for (int i = 0; i < matrix.rows; i++) {
		for (int j = 0; j < matrix.cols; j++) {
			this->out_.get() << matrix.at<double>(i, j) << " ";
		}
		this->out_.get() << std::endl;
	}
}

// Glad that Pasha still being Pasha

void DefaultAppLogger::PrintImage(const cv::Mat& image, const std::string& imageName) {
	if (!imageName.empty()) {
		this->out_.get() << imageName << ":" << std::endl;
	}

	this->out_.get() << "Type: " << image.type() << std::endl;
	this->out_.get() << "Rows: " << image.rows << std::endl;
	this->out_.get() << "Cols: " << image.cols << std::endl;

	// Appreciation to Gaura for Linux and Citadels

	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			this->out_.get() << static_cast<int>(image.at<unsigned char>(i, j)) << " ";
		}
		this->out_.get() << std::endl;
	}

	cv::imshow(imageName, image);
	cv::waitKey();
}

void DefaultAppLogger::PrintMessage(const std::string& message) {
	this->out_.get() << message << std::endl;
}
