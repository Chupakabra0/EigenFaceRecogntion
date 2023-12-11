#pragma once
#include <opencv2/opencv.hpp>

#include "Consts.hpp"
#include "ImageDecomposition.hpp"

struct IFileService {
public:
	virtual ~IFileService() = default;

	virtual void Write(const std::string& filePath, const std::shared_ptr<cv::Vec<double, BASIS_IMAGE_SIZE>>& meanFace, const std::shared_ptr<cv::Mat>& eigenMatrix,
		const std::shared_ptr<std::vector<ImageDecomposition>>& images) = 0;

	virtual void Read(const std::string& filePath, std::shared_ptr<cv::Vec<double, BASIS_IMAGE_SIZE>>& meanFace, std::shared_ptr<cv::Mat>& eigenMatrix,
		std::shared_ptr<std::vector<ImageDecomposition>>& images) = 0;
};
