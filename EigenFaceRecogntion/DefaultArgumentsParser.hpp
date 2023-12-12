#pragma once
#include <args/args.hpp>

#include "IArgumentsParser.hpp"
#include "Utils.hpp"

class DefaultArgumentsParser : public IArgumentsParser {
public:
	explicit DefaultArgumentsParser(const std::string& description, const std::string& epilog = std::string());

	~DefaultArgumentsParser() override = default;

	bool ParseAll(int argc, char** argv) override;
	[[nodiscard]] bool IsTrain() override;
	[[nodiscard]] bool IsGuess() override;
	virtual [[nodiscard]] std::vector<std::string> CalculateTrainPaths() override;
	virtual [[nodiscard]] std::vector<std::string> CalculateGuessPaths() override;

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

