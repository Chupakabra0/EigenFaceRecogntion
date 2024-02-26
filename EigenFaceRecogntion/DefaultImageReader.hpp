#pragma once
#include "IImageReader.hpp"

class DefaultImageReader : public IImageReader {
public:
	explicit DefaultImageReader() = default;

	cv::Mat ReadImage(const std::string& path) const override;
};
