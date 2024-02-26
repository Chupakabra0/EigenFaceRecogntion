#include "DefaultImageReader.hpp"

cv::Mat DefaultImageReader::ReadImage(const std::string& path) const {
	return cv::imread(path, cv::ImreadModes::IMREAD_ANYDEPTH | cv::ImreadModes::IMREAD_ANYCOLOR);
}
