#include "GrayscaleImageProcessor.hpp"

GrayscaleImageProcessor::GrayscaleImageProcessor(ImageProcessor* nextImageProcessor)
	: ImageProcessor(nextImageProcessor) {

}

[[nodiscard]] cv::Mat GrayscaleImageProcessor::ProcessImage(const cv::Mat& image) {
	cv::Mat result;

	if (image.channels() != 1) {
		cv::cvtColor(image, result, cv::COLOR_BGR2GRAY);
	}
	else {
		result = image;
	}

	return ImageProcessor::ProcessImage(result);
}
