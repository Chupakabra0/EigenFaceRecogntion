#pragma once
#include <memory>
#include <opencv2/opencv.hpp>

#include "Consts.hpp"

class FaceRecognition {
public:
	cv::Vec<double, BASIS_IMAGE_SIZE>* CalculateMeanFace(const cv::Mat* imageVecMatrix) const;
	cv::Mat* CalculatePhiMatrix(const cv::Mat* imageVecMatrix, const cv::Vec<double, BASIS_IMAGE_SIZE>* meanFace) const;
	cv::Mat* CalculateCovarianceMatrix(cv::Mat* phiMatrix) const;
	std::pair<cv::Mat*, cv::Mat*> CalculateEigenValuesAndVectors(cv::Mat* cvrs) const;
	cv::Mat* CalculateWeightsMatrix(cv::Mat* phiMatrix, cv::Mat* eigenVectors) const;
	cv::Vec<double, BASIS_IMAGE_SIZE>* RestoreImage(const cv::Mat& weights, cv::Mat* eigenVectors, cv::Vec<double, BASIS_IMAGE_SIZE>* meanFace) const;
};
