#pragma once
#include "IAppLogger.hpp"

class ConsoleLogger : public AppLogger {
public:
	explicit ConsoleLogger() = delete;
	explicit ConsoleLogger(std::ostream& out);

	~ConsoleLogger() override = default;

	void PrintMatrix(const cv::Mat& matrix, const std::string& matrixName = "") override;
	void PrintImage(const cv::Mat& image, const std::string& imageName = "") override;
	void PrintMessage(const std::string& message) override;
};
