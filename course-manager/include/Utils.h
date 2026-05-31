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
         * @brief Compare if two strings are equal in case-insensitive manner.
         *
         * @param a The first string to compare.
         * @param b The second string to compare.
         *
         * @return True if both the strings are equals(ignoring case-sensitivity).
         */
        static bool strEq(const std::string& a, const std::string& b)
        {
            const auto charCount = a.length();

            // If the character count in the strings are not equal then return `false`
            if (charCount != b.length())
                return false;

            // Compare each character for equality
            // Return false on first character mismatch
            for (std::size_t i = 0; i < charCount; ++i)
                if (std::tolower(a[i]) != std::tolower(b[i]))
                    return false;

            // Return true
            return true;
        }

    } // namespace utils

} // namespace abcu
