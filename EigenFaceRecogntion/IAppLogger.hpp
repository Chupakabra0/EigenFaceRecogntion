#pragma once
#include <opencv2/opencv.hpp>

struct IAppLogger {
public:
	virtual void PrintMatrix(const cv::Mat& matrix, const std::string& matrixName) = 0;
	virtual void PrintImage(const cv::Mat& image, const std::string& imageName)    = 0;
	virtual void PrintMessage(const std::string& message)                          = 0;

	virtual ~IAppLogger() = default;
};

struct AppLogger : public IAppLogger {
public:
	explicit AppLogger() = delete;

	explicit AppLogger(std::ostream& out)
		: out_(out) {

	}

	~AppLogger() override = default;

protected:
	std::reference_wrapper<std::ostream> out_;
};
