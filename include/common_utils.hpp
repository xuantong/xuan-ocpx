//
// Created by Dell on 2023/12/11 0011.
//

#ifndef SPM_PLATE_COMMON_UTILS_HPP
#define SPM_PLATE_COMMON_UTILS_HPP

#include <stdexcept>
#include <string>

void checkCondition(bool condition, const std::string &errorMessage) {
    if (!condition) {
        throw std::runtime_error(errorMessage);
    }
}


#endif //SPM_PLATE_COMMON_UTILS_HPP
