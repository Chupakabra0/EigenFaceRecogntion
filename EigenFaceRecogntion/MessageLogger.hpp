#pragma once
#include "IAppLogger.hpp"

class MessageLogger : public AppLogger {
public:
	explicit MessageLogger() = delete;

	explicit MessageLogger(std::ostream& out)
		: AppLogger(out) {

	}

	~MessageLogger() override = default;

	void PrintMatrix(const cv::Mat& matrix, const std::string& matrixName = "") override {

	}

	void PrintImage(const cv::Mat& image, const std::string& imageName = "") override {

	}

	void PrintMessage(const std::string& message) override {
		this->out_.get() << message << std::endl;
	}
};
