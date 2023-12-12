#pragma once
#include "ImageProcessor.hpp"

class ResizingImageProcessor : public ImageProcessor {
public:
	explicit ResizingImageProcessor() = delete;
	explicit ResizingImageProcessor(int imageHeight, int imageWidth);
	explicit ResizingImageProcessor(int imageHeight, int imageWidth, ImageProcessor* nextImageProcessor);

	ResizingImageProcessor(const ResizingImageProcessor&) = delete;
	ResizingImageProcessor(ResizingImageProcessor&&) noexcept = default;

	~ResizingImageProcessor() override = default;

	ResizingImageProcessor& operator=(const ResizingImageProcessor&) = delete;
	ResizingImageProcessor& operator=(ResizingImageProcessor&&) noexcept = default;

	[[nodiscard]] cv::Mat ProcessImage(const cv::Mat& image) override;
	[[nodiscard]] int GetImageHeight() const;
	void SetImageHeight(int height);
	[[nodiscard]] int GetImageWidth() const;
	void SetImageWidth(int width);

private:
	cv::Size imageSize_;
};
