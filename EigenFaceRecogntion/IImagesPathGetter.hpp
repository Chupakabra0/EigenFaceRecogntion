#pragma once
#include <set>
#include <string>

struct IImagesPathGetter {
public:
	virtual ~IImagesPathGetter()                                     = default;
	virtual [[nodiscard]] std::set<std::string> GetFilePaths() const = 0;
};
