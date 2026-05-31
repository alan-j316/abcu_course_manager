#pragma once
/**
 * @file Utils.h
 * @author Alan Abraham P Kochumon
 * @date Created on: May 31, 2026
 *
 * @brief Utilities/Helper functions for ABCU.
 *
 * @copyright Copyright (c) a Alan Abraham P Kochumon
 */


#include <string>
#include <cctype>


namespace abcu
{
    namespace utils
    {


        /**
         * @brief Compare if two strings in a case-insensitive manner.
         *
         * @param a The first string to compare.
         * @param b The second string to compare.
         *
         * @return 0 if both are equal, 1 if first string is greater else -1.
         */
        static bool strCmp(const std::string& a, const std::string& b)
        {
            const auto charCount = std::min(a.length(), b.length());

            // Compare each character for equality
            // Return false on first character mismatch
            for (std::size_t i = 0; i < charCount; ++i)
                if (std::tolower(a[i]) != std::tolower(b[i]))
                    return  

            // Return true
            // return;
        }

    } // namespace utils

} // namespace abcu
