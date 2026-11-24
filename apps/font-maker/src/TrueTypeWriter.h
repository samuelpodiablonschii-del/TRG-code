#pragma once

#include "FontProject.h"

#include <string>

namespace trg::font {

bool exportTrueType(const FontProject& project, const std::string& outputPath,
                    std::string* errorMessage = nullptr);

}
