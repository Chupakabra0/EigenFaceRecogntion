#pragma once
#include <fstream>
#include <nlohmann/json.hpp>

#include "IFileService.hpp"
#include "Utils.hpp"

class JSONService : public IFileService {
private:
	static constexpr const char* MEAN_FACE_KEY     = "MeanFace";
	static constexpr const char* EIGEN_MATRIX_KEY  = "EigenMatrix";
	static constexpr const char* WEIGHT_MATRIX_KEY = "WeightMatrix";

public:
	explicit JSONService() {

	}
	
	~JSONService() override = default;

	// TODO: WRITE IMAGE'S NAMES
	void Write(const std::string& filePath, const cv::Vec<double, BASIS_IMAGE_SIZE>& meanFace, const cv::Mat& eigenMatrix,
		const cv::Mat& weightMatrix) override {
		std::fstream file(filePath, std::fstream::out);

		if (!file.is_open()) {
			throw std::runtime_error("JSON Write error");
		}

		nlohmann::json json = {
			{ JSONService::MEAN_FACE_KEY, ConvertVecToVector(meanFace) },
			{ JSONService::EIGEN_MATRIX_KEY, ConvertMatToVectors(eigenMatrix) },
			{ JSONService::WEIGHT_MATRIX_KEY, ConvertMatToVectors(weightMatrix) }
		};

		file << json.dump(4);
	}

	// TODO: READ IMAGE'S NAMES
	void Read(const std::string& filePath, cv::Vec<double, BASIS_IMAGE_SIZE>& meanFace, cv::Mat& eigenMatrix,
		cv::Mat& weightMatrix) override {
		std::fstream file(filePath, std::fstream::in);

		if (!file.is_open()) {
			throw std::runtime_error("JSON Read error");
		}

		nlohmann::json json;
		file >> json;

		meanFace     = ConvertVectorToVec<BASIS_IMAGE_SIZE>(json.at(JSONService::MEAN_FACE_KEY));
		eigenMatrix  = ConvertVectorsToMat(json.at(JSONService::EIGEN_MATRIX_KEY));
		weightMatrix = ConvertVectorsToMat(json.at(JSONService::WEIGHT_MATRIX_KEY));
	}
};
