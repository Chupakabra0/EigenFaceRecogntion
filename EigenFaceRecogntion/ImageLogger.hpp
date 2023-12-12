#pragma once
#include "IAppLogger.hpp"

class ImageLogger : public AppLogger {
public:
	explicit ImageLogger();

	~ImageLogger() override = default;

	void PrintMatrix(const cv::Mat& matrix, const std::string& matrixName = "") override;
	void PrintImage(const cv::Mat& image, const std::string& imageName = "") override;
	void PrintMessage(const std::string& message) override;
};
