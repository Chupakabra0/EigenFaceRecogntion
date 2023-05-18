#pragma once
#include <vector>

struct IArgumentsParser {
public:
	virtual ~IArgumentsParser() = default;

	virtual bool ParseAll(int argc, char** argv) = 0;

	virtual [[nodiscard]] bool IsTrain() = 0;
	virtual [[nodiscard]] bool IsGuess() = 0;

	virtual [[nodiscard]] std::vector<std::string> CalculateGuessPaths()  = 0;
	virtual [[nodiscard]] std::vector<std::string> CalculateTrainPaths() = 0;
};
