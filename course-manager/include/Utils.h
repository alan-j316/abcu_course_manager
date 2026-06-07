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


#include <cctype>
#include <string>


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
        static int strCmp(const std::string& a, const std::string& b)
        {
            const auto charCount = std::min(a.length(), b.length());

            // Compare each character for equality
            // Return false on first character mismatch
            for (std::size_t i = 0; i < charCount; ++i)
            {
                // If the first character of the first string is lexicographically greater return 1
                if (std::tolower(a[i]) > std::tolower(b[i]))
                    return 1;
                // Similarly, when the first character is less than subsequent character is string2, return -1
                else if (std::tolower(a[i]) < std::tolower(b[i]))
                    return -1;
            }

            // Edge case: Either strings are substring of each other
            // If so return 1 if first string is of greater length, else -1 and 0 if both are of same length
            if (a.length() > b.length())
                return 1;
            else if (a.length() < b.length())
                return -1;
            return 0;
        }

    } // namespace utils

} // namespace abcu
