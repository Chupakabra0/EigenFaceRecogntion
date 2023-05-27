#pragma once
#include <iostream>
#include <filesystem>
#include <algorithm>
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

#include "ImageDecomposition.hpp"

class App {
public:
	enum RETURN_CODES {
		NO_ERROR,
		IMAGE_READER_NULLPTR,
		IMAGE_PROCESSOR_NULLPTR,
		IMAGE_CONVERTER_NULLPTR,
		APP_LOGGER_NULLPTR,
		ARG_PARSER_NULLPTR,
		TRAIN_DATA_NULLPTR,
		TRAIN_DATA_EMPTY
	};

	explicit App(IImageReader* imageReader, IImageProcessor* imageProcessorChain,
		IAppLogger* appLogger, IArgumentsParser* argumentsParser, IFileService* fileService)
		: imageReader_(std::move(imageReader)), imageProcessorChain_(std::move(imageProcessorChain)),
		  appLogger_(std::move(appLogger)), argumentsParser_(std::move(argumentsParser)),
		  fileService_(std::move(fileService)), imageConverter_(new ImageConverter<cv::Vec<double, BASIS_IMAGE_SIZE>>()),
		  meanFace_(nullptr), eigenVectors_(nullptr), eigenValues_(nullptr), trainImages_(new std::vector<ImageDecomposition>()) {

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

	std::shared_ptr<cv::Vec<double, BASIS_IMAGE_SIZE>> meanFace_;
	std::shared_ptr<cv::Mat> eigenVectors_;
	std::shared_ptr<cv::Vec<double, BASIS_IMAGE_SIZE>> eigenValues_;
	std::shared_ptr<std::vector<ImageDecomposition>> trainImages_;

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
		// Preparations for loop
		const std::vector<std::string> filePaths = this->argumentsParser_->CalculateTrainPaths();

		// C-pointer because of face recogntion construction
		auto* imageVecMatrix = new cv::Mat(static_cast<int>(filePaths.size()), BASIS_IMAGE_SIZE, CV_64FC1, 0.0);

		for (auto i = 0; i < filePaths.size(); ++i) {
			const auto& fp = filePaths[i];

			this->appLogger_->PrintMessage(fmt::format("File path: {}\n", fp));

			// Image processing
			const auto image          = this->imageReader_->ReadImage(fp);
			const auto processedImage = this->imageProcessorChain_->ProcessImage(image);
			const auto vec            = this->imageConverter_->Convert(processedImage);

			this->appLogger_->PrintImage(image, fp);
			this->appLogger_->PrintImage(processedImage, fp);

			// cv::Vec to cv::Mat
			for (auto j = 0; j < imageVecMatrix->cols; ++j) {
				imageVecMatrix->at<double>(i, j) = vec(j);
			}

			this->appLogger_->PrintMatrix(*imageVecMatrix, "Images Matrix");
		}

		// Calculations of mean face, phi faces and covariance matrix
		const auto faceRecogntion = std::make_unique<FaceRecognition>(imageVecMatrix);
		const auto meanFace       = std::shared_ptr<cv::Vec<double, BASIS_IMAGE_SIZE>>(faceRecogntion->CalculateMeanFace());
		const auto phiMatrix      = std::unique_ptr<cv::Mat>(faceRecogntion->CalculatePhiMatrix(meanFace.get()));
		const auto cvrsMatrix     = std::shared_ptr<cv::Mat>(faceRecogntion->CalculateCovarianceMatrix(phiMatrix.get()));

		this->appLogger_->PrintImage(this->imageConverter_->ConvertBack(*meanFace), "Mean Face");
		cv::imwrite("Mean_Face.jpg", this->imageConverter_->ConvertBack(*meanFace));

		// Eigen calculations
		const auto [tempValues, tempMatrix] = faceRecogntion->CalculateEigenValuesAndVectors(cvrsMatrix.get());

		// Take first n eigen vectors
		//const auto epsilon  = 0.97;
		//const auto eigenSum = *cv::sum(*tempValues).val;
		//auto csum           = 0.0;
		auto n              = std::min(imageVecMatrix->rows, imageVecMatrix->cols);
		
		//for (auto i = 0; i < tempValues->rows; ++i) {
		//	csum += tempValues->at<double>(i, 0);

		//	if (csum / eigenSum >= epsilon) {
		//		n = i + 1;
		//		break;
		//	}
		//}

		//const auto n = static_cast<int>(filePaths.size());
		auto eigenVectors = std::make_shared<cv::Mat>(n, tempMatrix->cols, CV_64FC1, 0.0);
		for (auto i = 0; i < eigenVectors->rows; ++i) {
			for (auto j = 0; j < eigenVectors->cols; ++j) {
				eigenVectors->at<double>(i, j) = tempMatrix->at<double>(i, j);
			}

			auto tempImage = std::make_unique<cv::Mat>();
			cv::normalize(eigenVectors->row(i), *tempImage, 0.0, 255.0, cv::NORM_MINMAX, CV_64FC1);

			this->appLogger_->PrintImage(this->imageConverter_->ConvertBack(*tempImage), fmt::format("Eigenface_{}.jpg", i));
			cv::imwrite(fmt::format("Eigenface_{}.jpg", i), this->imageConverter_->ConvertBack(*tempImage));
		}

		for (auto i = 0; i < eigenVectors->rows; ++i) {
			auto tempImage = std::make_unique<cv::Mat>();
			cv::normalize(eigenVectors->row(i), *tempImage, 0.0, 255.0, cv::NORM_MINMAX, CV_64FC1);
			*tempImage = this->imageConverter_->ConvertBack(*tempImage);

			this->appLogger_->PrintImage(*tempImage, fmt::format("Eigenface_{}.jpg", i));
			cv::imwrite(fmt::format("Eigenface_{}.jpg", i), *tempImage);
		}

		delete tempValues;
		delete tempMatrix;

		// Weights calculations
		const auto weightsMatrix = std::unique_ptr<cv::Mat>(faceRecogntion->CalculateWeightsMatrix(phiMatrix.get(), eigenVectors.get()));

		this->appLogger_->PrintMatrix(*weightsMatrix, "Weights Matrix");

		// Check by restoring basic images
		this->RestoreBasicImages_(faceRecogntion.get(), filePaths, weightsMatrix.get(), eigenVectors.get(), meanFace.get());

		this->appLogger_->PrintMessage(fmt::format(fmt::fg(fmt::rgb(0, 255, 0)), "Application ended...\n"));

		// Remember important data
		this->meanFace_     = meanFace;
		this->eigenVectors_ = eigenVectors;
		for (auto i = 0; i < weightsMatrix->rows; ++i) {
			this->trainImages_->emplace_back(filePaths[i], weightsMatrix->row(i));
		}

		this->fileService_->Write("temp.json", *this->meanFace_, *this->eigenVectors_, *this->trainImages_);

		return App::RETURN_CODES::NO_ERROR;
	}

	[[nodiscard]] App::RETURN_CODES Guess_() {
		if (this->meanFace_ == nullptr || this->eigenVectors_ == nullptr || this->trainImages_ == nullptr) {
			if (std::filesystem::exists("temp.json")) {
				this->fileService_->Read("temp.json", *this->meanFace_, *this->eigenVectors_, *this->trainImages_);
			}
			else {
				return App::RETURN_CODES::TRAIN_DATA_NULLPTR;
			}
		}
		if (this->eigenVectors_->rows == 0 || this->trainImages_->empty()) {
			return App::RETURN_CODES::TRAIN_DATA_EMPTY;
		}

		const std::vector<std::string> filePaths = this->argumentsParser_->CalculateGuessPaths();

		for (auto i = 0; i < filePaths.size(); ++i) {
			const auto& fp = filePaths[i];

			this->appLogger_->PrintMessage(fmt::format("File path: {}\n", fp));

			const auto image          = this->imageReader_->ReadImage(fp);
			const auto processedImage = this->imageProcessorChain_->ProcessImage(image);
			const auto vec            = this->imageConverter_->Convert(processedImage);

			this->appLogger_->PrintImage(image, fp);
			this->appLogger_->PrintImage(processedImage, fp);

			const auto phi  = std::make_unique<cv::Vec<double, BASIS_IMAGE_SIZE>>(vec - *this->meanFace_);
			auto weights    = std::make_unique<cv::Mat>(1, this->trainImages_->front().GetCoeffs().size(), CV_64FC1, 0.0);

			for (auto i = 0; i < weights->cols; ++i) {
				weights->at<double>(0, i) = this->eigenVectors_->row(i).dot(phi->t());
			}

			auto data = std::make_unique<cv::Mat>(this->trainImages_->size(), this->trainImages_->front().GetCoeffs().size(), CV_64FC1, 0.0);
			for (auto i = 0; i < this->trainImages_->size(); ++i) {
				for (auto j = 0; j < this->trainImages_->front().GetCoeffs().size(); ++j) {
					data->at<double>(i, j) = (*this->trainImages_)[i].GetCoeffs()[j];
				}
			}

			auto distances = std::make_unique<cv::Mat>(1, weights->cols, CV_64FC1, 0.0);
			for (auto i = 0; i < distances->cols; ++i) {
				distances->at<double>(0, i) = cv::norm(*weights, data->row(i), cv::NORM_L1);
				//distances->at<double>(0, i) = cv::norm(*weights, data->row(i), cv::NORM_L2);
				//distances->at<double>(0, i) = 1.0 - (weights->dot(data->row(i)) / cv::norm(*weights, cv::NORM_L2) / cv::norm(data->row(i), cv::NORM_L2));
			}

			fmt::print("Image name: {}\n================\n", fp);

			double minValue = 0.0;
			cv::Point minIndex = cv::Point();
			cv::minMaxLoc(*distances, &minValue, nullptr, &minIndex, nullptr);

			fmt::print("{} fit image: {}\nDistance: {}\n",
				"Best", this->trainImages_->at(minIndex.x).GetName(), minValue
			);

			fmt::print("================\n---------------\n");
		}

		return App::RETURN_CODES::NO_ERROR;
	}

	void RestoreBasicImages_(FaceRecognition* faceRecogntion, const std::vector<std::string>& filePaths,
		cv::Mat* weightsMatrix, cv::Mat* eigenVectors, cv::Vec<double, BASIS_IMAGE_SIZE>* meanFace) {
		for (auto i = 0; i < filePaths.size(); ++i) {
			const auto restoredImage = faceRecogntion->RestoreImage(weightsMatrix->row(i), eigenVectors, meanFace);

			this->appLogger_->PrintImage(this->imageConverter_->ConvertBack(*restoredImage), "Restored Image");
			cv::imwrite(fmt::format("Restored_{}.jpg", i + 1), this->imageConverter_->ConvertBack(*restoredImage));
		}
	}
};
