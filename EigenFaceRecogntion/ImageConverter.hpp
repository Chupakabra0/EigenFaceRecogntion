#pragma once
#include <opencv2/opencv.hpp>

#include "Consts.hpp"

template<class TargetClass>
struct ImageConverter {
public:
	TargetClass Convert(const cv::Mat& mat) {
		throw std::runtime_error("Unimplemented method");
	}

	cv::Mat ConvertBack(const TargetClass& element) {
		throw std::runtime_error("Unimplemented method");
	}
};

template<>
class ImageConverter<cv::Vec<double, BASIS_IMAGE_SIZE>> {
public:
	using Type = cv::Vec<double, BASIS_IMAGE_SIZE>;

	Type Convert(const cv::Mat& mat) {
		Type result;
		cv::Mat temp;

		mat.convertTo(temp, CV_64FC1);
		temp   = temp.reshape(0, 1);
		result = Type(temp);

		return result;
	}

	cv::Mat ConvertBack(const Type& vec) {
		cv::Mat result;

		result = cv::Mat(vec);
		cv::normalize(result, result, 0.0, 255.0, cv::NORM_MINMAX, CV_64FC1);
		result = result.reshape(0, BASIS_IMAGE_WIDTH);
		result.convertTo(result, CV_8UC1);

		return result;
	}
};
