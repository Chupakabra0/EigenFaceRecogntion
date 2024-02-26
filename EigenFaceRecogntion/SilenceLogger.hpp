#pragma once
#include "IAppLogger.hpp"

class SilenceLogger : public AppLogger {
public:
	explicit SilenceLogger();

	~SilenceLogger() override = default;

	void PrintMatrix(const cv::Mat& matrix, const std::string& matrixName = "") override;
	void PrintImage(const cv::Mat& image, const std::string& imageName = "") override;
	void PrintMessage(const std::string& message) override;
};
