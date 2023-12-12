#include "FaceRecognition.hpp"

cv::Vec<double, BASIS_IMAGE_SIZE>* FaceRecognition::CalculateMeanFace(const cv::Mat* imageVecMatrix) const {
	auto* meanFace = new cv::Vec<double, BASIS_IMAGE_SIZE>();

	cv::reduce(*imageVecMatrix, *meanFace, 0, cv::REDUCE_AVG);

	return meanFace;
}

cv::Mat* FaceRecognition::CalculatePhiMatrix(const cv::Mat* imageVecMatrix, const cv::Vec<double, BASIS_IMAGE_SIZE>* meanFace) const {
	auto* phiMatrix = new cv::Mat();
	auto temp = std::make_unique<cv::Mat>();

	cv::repeat(meanFace->t(), imageVecMatrix->rows, 1, *temp);
	cv::subtract(*imageVecMatrix, *temp, *phiMatrix);

	return phiMatrix;
}

cv::Mat* FaceRecognition::CalculateNormalCovarianceMatrix(cv::Mat* phiMatrix) const {
	auto* cvrs = new cv::Mat();
	auto avgs = std::make_unique<cv::Mat>();

	cv::calcCovarMatrix(*phiMatrix, *cvrs, *avgs, cv::COVAR_ROWS | cv::COVAR_NORMAL);
	*cvrs /= phiMatrix->rows;
	//*cvrs = *phiMatrix->t() * phiMatrix / phiMatrix->rows;

	return cvrs;
}

cv::Mat* FaceRecognition::CalculateScrambledCovarianceMatrix(cv::Mat* phiMatrix) const {
	auto* cvrs = new cv::Mat();
	auto avgs = std::make_unique<cv::Mat>();

	cv::calcCovarMatrix(*phiMatrix, *cvrs, *avgs, cv::COVAR_ROWS | cv::COVAR_SCRAMBLED);
	*cvrs /= phiMatrix->rows;

	return cvrs;
}

std::pair<cv::Mat*, cv::Mat*> FaceRecognition::CalculateEigenValuesAndVectors(cv::Mat* cvrs) const {
	auto* eigenValues = new cv::Mat();
	auto* eigenVectors = new cv::Mat();

	cv::eigen(*cvrs, *eigenValues, *eigenVectors);

	return std::make_pair(eigenValues, eigenVectors);
}

cv::Mat* FaceRecognition::CalculateWeightsMatrix(cv::Mat* phiMatrix, cv::Mat* eigenVectors) const {
	auto* weightMatrix = new cv::Mat(phiMatrix->rows, eigenVectors->rows, CV_64FC1, 0.0);

	for (auto i = 0; i < weightMatrix->rows; ++i) {
		for (auto j = 0; j < weightMatrix->cols; ++j) {
			weightMatrix->at<double>(i, j) = eigenVectors->row(j).dot(phiMatrix->row(i));
		}
	}

	return weightMatrix;
}

// Thx Danila Knight for support and help

cv::Vec<double, BASIS_IMAGE_SIZE>* FaceRecognition::RestoreImage(const cv::Mat& weights, cv::Mat* eigenVectors, cv::Vec<double, BASIS_IMAGE_SIZE>* meanFace) const {
	auto* temp = new cv::Vec<double, BASIS_IMAGE_SIZE>();

	for (auto i = 0; i < weights.cols; ++i) {
		for (auto j = 0; j < eigenVectors->cols; ++j) {
			(*temp)(j) += eigenVectors->at<double>(i, j) * weights.at<double>(0, i);
		}
	}

	*temp += *meanFace;

	return temp;
}
