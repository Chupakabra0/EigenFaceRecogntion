#pragma once
#include "IAppLogger.hpp"

class ConsoleLogger : public AppLogger {
public:
	explicit ConsoleLogger() = delete;

	explicit ConsoleLogger(std::ostream& out)
		: AppLogger(out) {

	}

	~ConsoleLogger() override = default;

	void PrintMatrix(const cv::Mat& matrix, const std::string& matrixName = "") override {
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

	void PrintImage(const cv::Mat& image, const std::string& imageName = "") override {

	}

	void PrintMessage(const std::string& message) override {
		this->out_.get() << message << std::endl;
	}
};
