/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "utilities/helpers.hpp"

namespace jng {

    /*std::string wstringToString(const std::wstring& wideString)
    {
        std::string temp(wideString.length(), ' ');
        std::copy(wideString.begin(), wideString.end(), temp.begin());
        return temp;
    }*/

    std::wstring stringToWstring(const std::string& string)
    {
        std::wstring temp(string.length(), L' ');
        std::copy(string.begin(), string.end(), temp.begin());
        return temp;
    }

} // namespace jng
