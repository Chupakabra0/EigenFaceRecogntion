#pragma once
#include "IImageProcessor.hpp"

class ImageProcessor : public IImageProcessor {
public:
	explicit ImageProcessor();
	explicit ImageProcessor(ImageProcessor* nextImageProcessor);

	ImageProcessor(const ImageProcessor&) = delete;
	ImageProcessor(ImageProcessor&& move) noexcept;

	~ImageProcessor() override = default;

	ImageProcessor& operator=(const ImageProcessor&) = delete;
	ImageProcessor& operator=(ImageProcessor&& move) noexcept {
		std::swap(move.nextImageProcessor_, this->nextImageProcessor_);
	}

	[[nodiscard]] cv::Mat ProcessImage(const cv::Mat& image) override;
	ImageProcessor* GetNextImageProcessor() const;
	void SetNextImageProcessor(ImageProcessor* nextImageProcessor);
	void ResetNextImageProcessor();

protected:
	std::unique_ptr<ImageProcessor> nextImageProcessor_;
};
