/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"
#include "jng/platform/key_codes.hpp"

namespace jng {
    namespace Platform {

        /* Opens an open filename dialog native to the system
        * 
        * \param filter C-style string that describes the file type filter
        * \return string contaning choosen filepath or an empty string when user canceled
        */
        std::string openFilenameDialog(const char* filter);

        /* Opens a save filename dialog native to the system
        * 
        * \param filter C-style string that describes the file type filter
        * \return string contaning choosen filepath or an empty string when user canceled
        */
        std::string saveFilenameDialog(const char* filter);

    } // namespace Platform
} // namespace jng
