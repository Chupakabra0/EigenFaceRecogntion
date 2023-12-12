#include "DefaultArgumentsParser.hpp"

DefaultArgumentsParser::DefaultArgumentsParser(const std::string& description, const std::string& epilog)
	: argumentParser_(std::make_unique<args::ArgumentParser>(description, epilog)),
	help_(*this->argumentParser_, "help", "FUCK YOU!", { 'h', "help" }),
	guessPaths_(*this->argumentParser_, "guess", "Recognize person on given image", { 'g', "guess" }),
	trainPaths_(*this->argumentParser_, "train", "Train AI on given image list", { 't', "train" }) {

}

bool DefaultArgumentsParser::ParseAll(int argc, char** argv) {
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

[[nodiscard]] bool DefaultArgumentsParser::IsTrain() {
	return this->trainPaths_.Matched();
}

[[nodiscard]] bool DefaultArgumentsParser::IsGuess() {
	return this->guessPaths_.Matched();
}

[[nodiscard]] std::vector<std::string> DefaultArgumentsParser::CalculateTrainPaths() {
	return this->trainPaths_.Get();
}

[[nodiscard]] std::vector<std::string> DefaultArgumentsParser::CalculateGuessPaths() {
	return this->guessPaths_.Get();
}
