#include "ResizingImageProcessor.hpp"

ResizingImageProcessor::ResizingImageProcessor(int imageHeight, int imageWidth)
	: ResizingImageProcessor(imageHeight, imageWidth, nullptr) {

}

ResizingImageProcessor::ResizingImageProcessor(int imageHeight, int imageWidth, ImageProcessor* nextImageProcessor)
	: ImageProcessor(nextImageProcessor), imageSize_(imageHeight, imageWidth) {

}

[[nodiscard]] cv::Mat ResizingImageProcessor::ProcessImage(const cv::Mat& image) {
	cv::Mat result;
	cv::resize(image, result, this->imageSize_);

	return ImageProcessor::ProcessImage(result);
}

[[nodiscard]] int ResizingImageProcessor::GetImageHeight() const {
	return this->imageSize_.height;
}

void ResizingImageProcessor::SetImageHeight(int height) {
	this->imageSize_.height = height;
}

[[nodiscard]] int ResizingImageProcessor::GetImageWidth() const {
	return this->imageSize_.height;
}

void ResizingImageProcessor::SetImageWidth(int width) {
	this->imageSize_.width = width;
}
