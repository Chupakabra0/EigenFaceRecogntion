#pragma once
#include "IAppLogger.hpp"

class DefaultAppLogger : public IAppLogger {
public:
	explicit DefaultAppLogger() = delete;
	explicit DefaultAppLogger(std::ostream& out);

	~DefaultAppLogger() = default;

	void PrintMatrix(const cv::Mat& matrix, const std::string& matrixName = "") override;
	void PrintImage(const cv::Mat& image, const std::string& imageName = "") override;
	void PrintMessage(const std::string& message) override;

private:
	std::reference_wrapper<std::ostream> out_;
};
