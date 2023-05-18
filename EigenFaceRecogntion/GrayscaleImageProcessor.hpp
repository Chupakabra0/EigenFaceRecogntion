#pragma once
#include "ImageProcessor.hpp"

class GrayscaleImageProcessor : public ImageProcessor {
public:
	explicit GrayscaleImageProcessor() = default;

	explicit GrayscaleImageProcessor(ImageProcessor* nextImageProcessor)
		: ImageProcessor(nextImageProcessor) {

	}

	GrayscaleImageProcessor(const GrayscaleImageProcessor&)     = delete;
	GrayscaleImageProcessor(GrayscaleImageProcessor&&) noexcept = default;

	~GrayscaleImageProcessor() override = default;

	GrayscaleImageProcessor& operator=(const GrayscaleImageProcessor&)     = delete;
	GrayscaleImageProcessor& operator=(GrayscaleImageProcessor&&) noexcept = default;

	[[nodiscard]] cv::Mat ProcessImage(const cv::Mat& image) override {
		cv::Mat result;

		if (image.channels() != 1) {
			cv::cvtColor(image, result, cv::COLOR_BGR2GRAY);
		}
		else {
			result = image;
		}

		return ImageProcessor::ProcessImage(result);
	}
};
