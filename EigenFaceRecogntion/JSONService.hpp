#pragma once
#include <fstream>
#include <nlohmann/json.hpp>

#include "IFileService.hpp"
#include "Utils.hpp"

class JSONService : public IFileService {
private:
	static constexpr const char* THRESHOLD_KEY     = "Threshold";
	static constexpr const char* MEAN_FACE_KEY     = "MeanFace";
	static constexpr const char* EIGEN_MATRIX_KEY  = "EigenMatrix";
	static constexpr const char* IMAGE_KEY         = "ImageDecomposition";

public:
	explicit JSONService() = default;
	
	~JSONService() override = default;

	// TODO: WRITE IMAGE'S NAMES
	void Write(const std::string& filePath, const std::shared_ptr<cv::Vec<double, BASIS_IMAGE_SIZE>>& meanFace, const std::shared_ptr<cv::Mat>& eigenMatrix,
		const std::shared_ptr<std::vector<ImageDecomposition>>& images, double threshold) override;

	void Read(const std::string& filePath, std::shared_ptr<cv::Vec<double, BASIS_IMAGE_SIZE>>& meanFace, std::shared_ptr<cv::Mat>& eigenMatrix,
		std::shared_ptr<std::vector<ImageDecomposition>>& images, double& threshold) override;
};
