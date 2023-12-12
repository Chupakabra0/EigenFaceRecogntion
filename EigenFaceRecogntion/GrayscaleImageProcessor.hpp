#pragma once
#include "ImageProcessor.hpp"

class GrayscaleImageProcessor : public ImageProcessor {
public:
	explicit GrayscaleImageProcessor() = default;
	explicit GrayscaleImageProcessor(ImageProcessor* nextImageProcessor);

	GrayscaleImageProcessor(const GrayscaleImageProcessor&) = delete;
	GrayscaleImageProcessor(GrayscaleImageProcessor&&) noexcept = default;

	~GrayscaleImageProcessor() override = default;

	GrayscaleImageProcessor& operator=(const GrayscaleImageProcessor&) = delete;
	GrayscaleImageProcessor& operator=(GrayscaleImageProcessor&&) noexcept = default;

	[[nodiscard]] cv::Mat ProcessImage(const cv::Mat& image) override;
};
