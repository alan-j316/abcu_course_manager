#pragma once
/**
 * @file CourseManager.h
 * @author Alan Abraham P Kochumon
 * @date Created on: June 06, 2026
 *
 * @brief BST node used for storing @ref abcu::Course.
 *
 * @copyright Copyright (c) a Alan Abraham P Kochumon
 */


#include "Course.h"

namespace abcu
{
    class CourseNode
    {
    public:
        Course course;

        // Initialize left and right with nullptr
        CourseNode* left{ nullptr };
        CourseNode* right{ nullptr };
    };

} // namespace abcu