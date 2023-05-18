#pragma once
#include <iostream>
#include <filesystem>
#include <opencv2/core/utils/logger.hpp>
#include <fmt/core.h>
#include <fmt/color.h>

#include "IImagesPathGetter.hpp"
#include "IImageReader.hpp"
#include "ImageProcessor.hpp"
#include "ImageConverter.hpp"
#include "FaceRecogntion.hpp"
#include "IAppLogger.hpp"
#include "IArgumentsParser.hpp"
#include "IFileService.hpp"

class App {
public:
	enum RETURN_CODES {
		NO_ERROR,
		IMAGE_READER_NULLPTR,
		IMAGE_PROCESSOR_NULLPTR,
		IMAGE_CONVERTER_NULLPTR,
		APP_LOGGER_NULLPTR,
		ARG_PARSER_NULLPTR,
		TRAIN_DATA_NULLPTR
	};

	explicit App(IImageReader* imageReader, IImageProcessor* imageProcessorChain,
		IAppLogger* appLogger, IArgumentsParser* argumentsParser, IFileService* fileService)
		: imageReader_(std::move(imageReader)), imageProcessorChain_(std::move(imageProcessorChain)),
		  appLogger_(std::move(appLogger)), argumentsParser_(std::move(argumentsParser)),
		  fileService_(std::move(fileService)), imageConverter_(new ImageConverter<cv::Vec<double, BASIS_IMAGE_SIZE>>()),
		  meanFace_(nullptr), eigenVectors_(nullptr), weightsMatrix_(nullptr) {

	}

	App(const App&) = delete;
	App(App&&)      = delete;

	~App() = default;

	App& operator=(const App&) = delete;
	App& operator=(App&&)      = delete;

	[[nodiscard]] int ExecuteApp(int argc, char** argv) {
		cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

		App::RETURN_CODES returnCode = this->DataValidation_();

		if (returnCode != App::RETURN_CODES::NO_ERROR) {
			return returnCode;
		}

		this->appLogger_->PrintMessage(fmt::format(fmt::fg(fmt::rgb(0, 255, 0)), "Application started...\n"));

		if (this->argumentsParser_->IsTrain()) {
			returnCode = this->Train_();
		}
		if (this->argumentsParser_->IsGuess()) {
			returnCode = this->Guess_();
		}

		return returnCode;
	}

private:
	std::unique_ptr<IImageReader> imageReader_;
	std::unique_ptr<IImageProcessor> imageProcessorChain_;
	std::unique_ptr<IAppLogger> appLogger_;
	std::unique_ptr<IArgumentsParser> argumentsParser_;
	std::unique_ptr<IFileService> fileService_;
	std::unique_ptr<ImageConverter<cv::Vec<double, BASIS_IMAGE_SIZE>>> imageConverter_;

	std::unique_ptr<cv::Vec<double, BASIS_IMAGE_SIZE>> meanFace_;
	std::unique_ptr<cv::Mat> eigenVectors_;
	std::unique_ptr<cv::Mat> weightsMatrix_;

	std::vector<std::string> fileNames_;

	[[nodiscard]] App::RETURN_CODES DataValidation_() {
		if (this->appLogger_ == nullptr) {
			return App::RETURN_CODES::APP_LOGGER_NULLPTR;
		}
		if (this->imageReader_ == nullptr) {
			this->appLogger_->PrintMessage(fmt::format(fmt::fg(fmt::rgb(255, 0, 0)), "Image reader is nullptr!!!\n"));

			return App::RETURN_CODES::IMAGE_READER_NULLPTR;
		}
		if (this->imageProcessorChain_ == nullptr) {
			this->appLogger_->PrintMessage(fmt::format(fmt::fg(fmt::rgb(255, 0, 0)), "Image processor is nullptr!!!\n"));

			return App::RETURN_CODES::IMAGE_PROCESSOR_NULLPTR;
		}
		if (this->argumentsParser_ == nullptr) {
			this->appLogger_->PrintMessage(fmt::format(fmt::fg(fmt::rgb(255, 0, 0)), "Arguments parser is nullptr!!!\n"));

			return App::RETURN_CODES::ARG_PARSER_NULLPTR;
		}
		if (this->imageConverter_ == nullptr) {
			this->appLogger_->PrintMessage(fmt::format(fmt::fg(fmt::rgb(255, 0, 0)), "Image coverter is nullptr!!!\n"));

			return App::RETURN_CODES::IMAGE_CONVERTER_NULLPTR;
		}

		return App::RETURN_CODES::NO_ERROR;
	}

	[[nodiscard]] App::RETURN_CODES Train_() {
		const std::set<std::string> filePaths = this->argumentsParser_->CalculateTrainPaths();
		auto* imageVecMatrix                  = new cv::Mat(static_cast<int>(filePaths.size()), BASIS_IMAGE_SIZE, CV_64FC1, 0.0);

		for (auto i = 0; i < filePaths.size(); ++i) {
			const auto& fp = *std::next(filePaths.begin(), i);

			this->appLogger_->PrintMessage(fmt::format("File path: {}\n", fp));

			const auto image          = this->imageReader_->ReadImage(fp);
			const auto processedImage = this->imageProcessorChain_->ProcessImage(image);
			const auto vec            = this->imageConverter_->Convert(processedImage);

			this->appLogger_->PrintImage(image, fp);
			this->appLogger_->PrintImage(processedImage, fp);

			for (auto j = 0; j < imageVecMatrix->cols; ++j) {
				imageVecMatrix->at<double>(i, j) = vec(j);
			}

			this->appLogger_->PrintMatrix(*imageVecMatrix, "Images Matrix");
			this->fileNames_.push_back(fp);
		}

		std::unique_ptr<FaceRecognition> faceRecogntion = std::make_unique<FaceRecognition>(imageVecMatrix);

		this->meanFace_ = std::unique_ptr<cv::Vec<double, BASIS_IMAGE_SIZE>>(faceRecogntion->CalculateMeanFace());

		const auto phiMatrix  = std::unique_ptr<cv::Mat>(faceRecogntion->CalculatePhiMatrix(this->meanFace_.get()));
		const auto cvrsMatrix = std::unique_ptr<cv::Mat>(faceRecogntion->CalculateCovarianceMatrix(phiMatrix.get()));

		this->appLogger_->PrintImage(this->imageConverter_->ConvertBack(*this->meanFace_), "Mean Face");
		cv::imwrite("Mean_Face.jpg", this->imageConverter_->ConvertBack(*this->meanFace_));

		const auto [tempValues, tempMatrix] = faceRecogntion->CalculateEigenValuesAndVectors(cvrsMatrix.get());

		this->eigenVectors_  = std::make_unique<cv::Mat>(static_cast<int>(filePaths.size()), tempMatrix->cols, CV_64FC1, 0.0);
		for (auto i = 0; i < filePaths.size(); ++i) {
			for (auto j = 0; j < tempMatrix->cols; ++j) {
				this->eigenVectors_->at<double>(i, j) = tempMatrix->at<double>(i, j);
			}
		}

		this->weightsMatrix_ = std::unique_ptr<cv::Mat>(
			faceRecogntion->CalculateWeightsMatrix(phiMatrix.get(), this->eigenVectors_.get())
		);

		this->appLogger_->PrintMatrix(*this->weightsMatrix_, "Weights Matrix");
		this->fileService_->Write("temp.json", *this->meanFace_, *this->eigenVectors_, *this->weightsMatrix_);

		this->RestoreBasicImages_(faceRecogntion, filePaths, this->weightsMatrix_, this->eigenVectors_, this->meanFace_);

		this->appLogger_->PrintMessage(fmt::format(fmt::fg(fmt::rgb(0, 255, 0)), "Application ended...\n"));

		return App::RETURN_CODES::NO_ERROR;
	}

	[[nodiscard]] App::RETURN_CODES Guess_() {
		if (this->meanFace_ == nullptr || this->eigenVectors_ == nullptr || this->weightsMatrix_ == nullptr) {
			if (std::filesystem::exists("temp.json")) {
				this->fileService_->Read("temp.json", *this->meanFace_, *this->eigenVectors_, *this->weightsMatrix_);
			}
			else {
				return App::RETURN_CODES::TRAIN_DATA_NULLPTR;
			}
		}

		const std::set<std::string> filePaths = this->argumentsParser_->CalculateGuessPaths();
		//auto* imageVecMatrix                  = new cv::Mat(static_cast<int>(filePaths.size()), BASIS_IMAGE_SIZE, CV_64FC1, 0.0);

		for (auto i = 0; i < filePaths.size(); ++i) {
			const auto& fp = *std::next(filePaths.begin(), i);

			this->appLogger_->PrintMessage(fmt::format("File path: {}\n", fp));

			const auto image          = this->imageReader_->ReadImage(fp);
			const auto processedImage = this->imageProcessorChain_->ProcessImage(image);
			const auto vec            = this->imageConverter_->Convert(processedImage);

			this->appLogger_->PrintImage(image, fp);
			this->appLogger_->PrintImage(processedImage, fp);

			const auto phi  = std::make_unique<cv::Vec<double, BASIS_IMAGE_SIZE>>(vec - *this->meanFace_);
			auto weights    = std::make_unique<cv::Mat>(1, this->weightsMatrix_->cols, CV_64FC1, 0.0);

			for (auto i = 0; i < weights->cols; ++i) {
				weights->at<double>(0, i) = this->eigenVectors_->row(i).dot(phi->t());
			}

			auto distances = std::make_unique<cv::Mat>(1, weights->cols, CV_64FC1, 0.0);
			for (auto i = 0; i < distances->cols; ++i) {
				distances->at<double>(0, i) = cv::norm(*weights, this->weightsMatrix_->row(i), cv::NORM_L2);
			}

			double minValue = 0.0;
			cv::Point minIndex = cv::Point();
			cv::minMaxLoc(*distances, &minValue, nullptr, &minIndex, nullptr);

			this->appLogger_->PrintMessage(fmt::format("\nEuclid:\nIt's {}-th face\nDistance: {}\nName: {}",
				minIndex.x, minValue, this->fileNames_[minIndex.x])
			);
		}

		return App::RETURN_CODES::NO_ERROR;
	}

	void RestoreBasicImages_(const std::unique_ptr<FaceRecognition>& faceRecogntion, const std::set<std::string>& filePaths,
		const std::unique_ptr<cv::Mat>& weightsMatrix, const std::unique_ptr<cv::Mat>& eigenVectors,
		const std::unique_ptr<cv::Vec<double, BASIS_IMAGE_SIZE>>& meanFace) {
		for (auto i = 0; i < filePaths.size(); ++i) {
			const auto restoredImage = faceRecogntion->RestoreImage(weightsMatrix->row(i), eigenVectors.get(), meanFace.get());

			this->appLogger_->PrintImage(this->imageConverter_->ConvertBack(*restoredImage), "Restored Image");
			cv::imwrite(fmt::format("Restored_{}.jpg", i + 1), this->imageConverter_->ConvertBack(*restoredImage));
		}
	}
};
