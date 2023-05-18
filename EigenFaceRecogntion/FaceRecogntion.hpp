#pragma once
#include <memory>
#include <opencv2/opencv.hpp>

#include "Consts.hpp"

class FaceRecognition {
public:
	explicit FaceRecognition() = delete;

	explicit FaceRecognition(cv::Mat* imageVecMatrix)
		: imageVecMatrix_(std::move(imageVecMatrix)) {

	}

	~FaceRecognition() = default;

	cv::Vec<double, BASIS_IMAGE_SIZE>* CalculateMeanFace() const {
		auto* meanFace = new cv::Vec<double, BASIS_IMAGE_SIZE>();

		cv::reduce(*this->imageVecMatrix_, *meanFace, 0, cv::REDUCE_AVG);

		return meanFace;
	}

	cv::Mat* CalculatePhiMatrix(const cv::Vec<double, BASIS_IMAGE_SIZE>* meanFace) const {
		auto* phiMatrix = new cv::Mat();
		auto temp       = std::make_unique<cv::Mat>();

		cv::repeat(meanFace->t(), this->imageVecMatrix_->rows, 1, *temp);
		cv::subtract(*this->imageVecMatrix_, *temp, *phiMatrix);

		return phiMatrix;
	}

	cv::Mat* CalculateCovarianceMatrix(cv::Mat* phiMatrix) const {
		auto* cvrs = new cv::Mat();
		auto avgs  = std::make_unique<cv::Mat>();

		cv::calcCovarMatrix(*phiMatrix, *cvrs, *avgs, cv::COVAR_ROWS | cv::COVAR_NORMAL);
		*cvrs /= phiMatrix->rows - 1;

		return cvrs;
	}

	std::pair<cv::Vec<double, BASIS_IMAGE_SIZE>*, cv::Mat*> CalculateEigenValuesAndVectors(cv::Mat* cvrs) const {
		auto* eigenValues  = new cv::Vec<double, BASIS_IMAGE_SIZE>();
		auto* eigenVectors = new cv::Mat();

		cv::eigenNonSymmetric(*cvrs, *eigenValues, *eigenVectors);

		return std::make_pair(eigenValues, eigenVectors);
	}

	cv::Mat* CalculateWeightsMatrix(cv::Mat* phiMatrix, cv::Mat* eigenVectors) const {
		auto* weightMatrix = new cv::Mat(phiMatrix->rows, phiMatrix->rows, CV_64FC1, 0.0);

		for (auto i = 0; i < weightMatrix->rows; ++i) {
			for (auto j = 0; j < weightMatrix->cols; ++j) {
				weightMatrix->at<double>(i, j) = eigenVectors->row(j).dot(phiMatrix->row(i));
			}
		}

		return weightMatrix;
	}

	cv::Vec<double, BASIS_IMAGE_SIZE>* RestoreImage(const cv::Mat& weights, cv::Mat* eigenVectors, cv::Vec<double, BASIS_IMAGE_SIZE>* meanFace) const {
		auto* temp = new cv::Vec<double, BASIS_IMAGE_SIZE>();

		for (auto i = 0; i < weights.cols; ++i) {
			for (auto j = 0; j < eigenVectors->cols; ++j) {
				(*temp)(j) += eigenVectors->at<double>(i, j) * weights.at<double>(0, i);
			}
		}

		*temp += *meanFace;

		return temp;
	}

private:
	std::unique_ptr<cv::Mat> imageVecMatrix_;
};
