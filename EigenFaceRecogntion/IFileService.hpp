#pragma once
#include <opencv2/opencv.hpp>

#include "Consts.hpp"

struct IFileService {
public:
	virtual ~IFileService() = default;

	virtual void Write(const std::string& filePath, const cv::Vec<double, BASIS_IMAGE_SIZE>& meanFace, const cv::Mat& eigenMatrix,
		const cv::Mat& weightMatrix) = 0;

	virtual void Read(const std::string& filePath, cv::Vec<double, BASIS_IMAGE_SIZE>& meanFace, cv::Mat& eigenMatrix,
		cv::Mat& weightMatrix) = 0;
};
