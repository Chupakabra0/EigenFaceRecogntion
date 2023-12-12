#include "TestCasesImagesPathGetter.hpp"

#include <fmt/format.h>

TestCasesImagesPathGetter::TestCasesImagesPathGetter(int count)
	: count_(count) {
	for (auto i = 0; i < this->count_; ++i) {
		this->paths_.insert(fmt::format("TestImages\\{}.jpg", i + 1));
	}
}

[[nodiscard]] std::set<std::string> TestCasesImagesPathGetter::GetFilePaths() const {
	return this->paths_;
}
