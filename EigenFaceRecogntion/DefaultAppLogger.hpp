#pragma once
#include <ostream>

#include "IAppLogger.hpp"
#include "Consts.hpp"

class DefaultAppLogger : public IAppLogger {
public:
	explicit DefaultAppLogger() = delete;

	explicit DefaultAppLogger(std::ostream& out)
		: out_(out) {

	}

	~DefaultAppLogger() = default;

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
		if (!imageName.empty()) {
			this->out_.get() << imageName << ":" << std::endl;
		}

		this->out_.get() << "Type: " << image.type() << std::endl;
		this->out_.get() << "Rows: " << image.rows << std::endl;
		this->out_.get() << "Cols: " << image.cols << std::endl;

		for (int i = 0; i < image.rows; i++) {
			for (int j = 0; j < image.cols; j++) {
				this->out_.get() << static_cast<int>(image.at<unsigned char>(i, j)) << " ";
			}
			this->out_.get() << std::endl;
		}

		cv::imshow(imageName, image);
		cv::waitKey();
	}

	void PrintMessage(const std::string& message) override {
		this->out_.get() << message << std::endl;
	}

private:
	std::reference_wrapper<std::ostream> out_;
};
