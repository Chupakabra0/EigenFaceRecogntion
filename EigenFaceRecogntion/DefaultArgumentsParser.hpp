#pragma once
#include <args/args.hpp>

#include "IArgumentsParser.hpp"
#include "Utils.hpp"

class DefaultArgumentsParser : public IArgumentsParser {
public:
	explicit DefaultArgumentsParser(const std::string& description, const std::string& epilog = std::string())
		: argumentParser_(std::make_unique<args::ArgumentParser>(description, epilog)),
	      help_(*this->argumentParser_, "help", "FUCK YOU!", {'h', "help"}),
		  guessPaths_(*this->argumentParser_, "guess", "Recognize person on given image", { 'g', "guess" }),
		  trainPaths_(*this->argumentParser_, "train", "Train AI on given image list", { 't', "train" }) {

	}

	~DefaultArgumentsParser() override = default;

	bool ParseAll(int argc, char** argv) override {
		try {
			return this->argumentParser_->ParseCLI(argc, argv);
		}
		catch (const args::Help&) {
			std::cout << *this->argumentParser_;

			return false;
		}
		catch (const args::ParseError& error) {
			std::cerr << error.what() << std::endl;

			return false;
		}
		catch (const args::ValidationError& error) {
			std::cerr << error.what() << std::endl;

			return false;
		}
	}

	[[nodiscard]] bool IsTrain() override {
		return this->trainPaths_.Matched();
	}

	[[nodiscard]] bool IsGuess() override {
		return this->guessPaths_.Matched();
	}

	virtual [[nodiscard]] std::vector<std::string> CalculateTrainPaths() override {
		return this->trainPaths_.Get();
	}

	virtual [[nodiscard]] std::vector<std::string> CalculateGuessPaths() override {
		return this->guessPaths_.Get();
	}

private:
	std::unique_ptr<args::ArgumentParser> argumentParser_;
	args::HelpFlag help_;
	args::Group group_;

	struct VectorStringReader {
	public:
		void operator()(const std::string& name, const std::string& value, std::vector<std::string>& destination) {
			destination = SplitString(value, ',');
		}
	};

	args::ValueFlag<std::vector<std::string>, VectorStringReader> guessPaths_;
	args::ValueFlag<std::vector<std::string>, VectorStringReader> trainPaths_;
};
