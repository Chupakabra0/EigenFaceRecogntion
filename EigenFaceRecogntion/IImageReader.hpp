#pragma once
#include <string>
#include <opencv2/opencv.hpp>

struct IImageReader {
public:
	virtual ~IImageReader()                                                = default;
	virtual [[nodiscard]] cv::Mat ReadImage(const std::string& path) const = 0;
};
