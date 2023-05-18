#pragma once
#include <opencv2/opencv.hpp>

struct IImageProcessor {
public:
	virtual ~IImageProcessor()                                       = default;
	virtual [[nodiscard]] cv::Mat ProcessImage(const cv::Mat& image) = 0;
};

class ImageProcessor : public IImageProcessor {
public:
	explicit ImageProcessor()
		: ImageProcessor(nullptr) {

	}

	explicit ImageProcessor(ImageProcessor* nextImageProcessor)
		: nextImageProcessor_(nextImageProcessor) {

	}

	ImageProcessor(const ImageProcessor&) = delete;

	ImageProcessor(ImageProcessor&& move) noexcept {
		std::swap(move.nextImageProcessor_, this->nextImageProcessor_);
	}

	~ImageProcessor() override = default;

	ImageProcessor& operator=(const ImageProcessor&) = delete;

	ImageProcessor& operator=(ImageProcessor&& move) noexcept {
		std::swap(move.nextImageProcessor_, this->nextImageProcessor_);
	}

	[[nodiscard]] cv::Mat ProcessImage(const cv::Mat& image) override {
		if (this->nextImageProcessor_ == nullptr) {
			return image;
		}

		return this->nextImageProcessor_->ProcessImage(image);
	}

	ImageProcessor* GetNextImageProcessor() const {
		return this->nextImageProcessor_.get();
	}

	void SetNextImageProcessor(ImageProcessor* nextImageProcessor) {
		this->ResetNextImageProcessor();
		this->nextImageProcessor_ = std::make_unique<ImageProcessor>(std::move(nextImageProcessor));
	}

	void ResetNextImageProcessor() {
		this->nextImageProcessor_.reset();
	}

protected:
	std::unique_ptr<ImageProcessor> nextImageProcessor_;
};
