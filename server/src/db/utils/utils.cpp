#include <vector>
#include <string>
#include "./utils.h"

const std::string& join(const std::vector<std::string>& arr, const std::string delimeter) {
    std::string* result = new std::string();
    for (const auto& item : arr)
        *result += item + delimeter;
    *result = result->substr(0, result->size() - delimeter.size());
    return *result;
}
