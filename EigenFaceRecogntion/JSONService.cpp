#include "JSONService.hpp"

#include <fstream>

void JSONService::Write(const std::string& filePath, const std::shared_ptr<cv::Vec<double, BASIS_IMAGE_SIZE>>& meanFace, const std::shared_ptr<cv::Mat>& eigenMatrix,
	const std::shared_ptr<std::vector<ImageDecomposition>>& images, double threshold) {
	if (meanFace == nullptr || eigenMatrix == nullptr || images == nullptr) {
		throw std::runtime_error("Nullptr error while writing");
	}

	std::fstream file(filePath, std::fstream::out);

	if (!file.is_open()) {
		throw std::runtime_error("JSON Write error");
	}

	nlohmann::json json = {
		{ JSONService::THRESHOLD_KEY, threshold },
		{ JSONService::MEAN_FACE_KEY, ConvertVecToVector(*meanFace) },
		{ JSONService::EIGEN_MATRIX_KEY, ConvertMatToVectors(*eigenMatrix) },
		{ JSONService::IMAGE_KEY, *images }
	};

	file << json.dump(4);
}

// TODO: READ IMAGE'S NAMES
void JSONService::Read(const std::string& filePath, std::shared_ptr<cv::Vec<double, BASIS_IMAGE_SIZE>>& meanFace, std::shared_ptr<cv::Mat>& eigenMatrix,
	std::shared_ptr<std::vector<ImageDecomposition>>& images, double& threshold) {
	std::fstream file(filePath, std::fstream::in);

	if (!file.is_open()) {
		throw std::runtime_error("JSON Read error");
	}

	nlohmann::json json;
	file >> json;

	std::vector<ImageDecomposition> temp = json.at(JSONService::IMAGE_KEY);

	threshold   = json.at(JSONService::THRESHOLD_KEY);
	meanFace    = std::make_shared<cv::Vec<double, BASIS_IMAGE_SIZE>>(ConvertVectorToVec<BASIS_IMAGE_SIZE>(json.at(JSONService::MEAN_FACE_KEY)));
	eigenMatrix = std::make_shared<cv::Mat>(ConvertVectorsToMat(json.at(JSONService::EIGEN_MATRIX_KEY)));
	images      = std::make_shared<std::vector<ImageDecomposition>>(std::move(temp));
}
