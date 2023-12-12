#include "ImageProcessor.hpp"

ImageProcessor::ImageProcessor()
	: ImageProcessor(nullptr) {

}

ImageProcessor::ImageProcessor(ImageProcessor* nextImageProcessor)
	: nextImageProcessor_(nextImageProcessor) {

}

ImageProcessor::ImageProcessor(ImageProcessor&& move) noexcept {
	std::swap(move.nextImageProcessor_, this->nextImageProcessor_);
}

[[nodiscard]] cv::Mat ImageProcessor::ProcessImage(const cv::Mat& image) {
	if (this->nextImageProcessor_ == nullptr) {
		return image;
	}

	return this->nextImageProcessor_->ProcessImage(image);
}

ImageProcessor* ImageProcessor::GetNextImageProcessor() const {
	return this->nextImageProcessor_.get();
}

void ImageProcessor::SetNextImageProcessor(ImageProcessor* nextImageProcessor) {
	this->ResetNextImageProcessor();
	this->nextImageProcessor_ = std::make_unique<ImageProcessor>(std::move(nextImageProcessor));
}

void ImageProcessor::ResetNextImageProcessor() {
	this->nextImageProcessor_.reset();
}
