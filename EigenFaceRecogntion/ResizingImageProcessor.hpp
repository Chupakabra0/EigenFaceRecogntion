#pragma once
#include "ImageProcessor.hpp"

class ResizingImageProcessor : public ImageProcessor {
public:
	explicit ResizingImageProcessor() = delete;

	explicit ResizingImageProcessor(int imageHeight, int imageWidth)
		: ResizingImageProcessor(imageHeight, imageWidth, nullptr) {

	}

	explicit ResizingImageProcessor(int imageHeight, int imageWidth, ImageProcessor* nextImageProcessor)
		: ImageProcessor(nextImageProcessor), imageSize_(imageHeight, imageWidth) {

	}

	ResizingImageProcessor(const ResizingImageProcessor&) = delete;
	ResizingImageProcessor(ResizingImageProcessor&&) noexcept = default;

	~ResizingImageProcessor() override = default;

	ResizingImageProcessor& operator=(const ResizingImageProcessor&) = delete;
	ResizingImageProcessor& operator=(ResizingImageProcessor&&) noexcept = default;

	[[nodiscard]] cv::Mat ProcessImage(const cv::Mat& image) override {
		cv::Mat result;
		cv::resize(image, result, this->imageSize_);

		return ImageProcessor::ProcessImage(result);
	}

	[[nodiscard]] int GetImageHeight() const {
		return this->imageSize_.height;
	}

	void SetImageHeight(int height) {
		this->imageSize_.height = height;
	}

	[[nodiscard]] int GetImageWidth() const {
		return this->imageSize_.height;
	}

	void SetImageWidth(int width) {
		this->imageSize_.width = width;
	}

private:
	cv::Size imageSize_;
};
