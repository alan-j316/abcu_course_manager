#pragma once
/**
 * @file CourseManager.h
 * @author Alan Abraham P Kochumon
 * @date Created on: May 31, 2026
 *
 * @brief A Course Manager entity that loads courses from a CSV file, parses it and provides actions like searching and
 *        printing.
 *
 * @copyright Copyright (c) a Alan Abraham P Kochumon
 */


#include "CourseNode.h"

#include <string>
#include <unordered_set>
#include <limits>

namespace abcu
{
    class CourseManager
    {
        // Initialize root with nullptr
        CourseNode* root{ nullptr };

        /**
         * @brief Remove invalid pre-requisites from the courses.
         *        A prerequisite is considered invalid, if that course id is not present in @p cache.
         *
         * @note To speed up look, the method uses the supplied cache for valid course look,
         *       so providing an empty cache will return the courses without validation.
         *
         * @param node  The root of the course manager.
         * @param cache The cache to use for validation.
         */
        static void removeInvalidPrerequisites(CourseNode* node, const std::unordered_set<std::string>& cache);

        /**
         * @brief Traverse and print the courses in an in-order format.
         * @param node The current root.
         */
        static void printInOrder(const CourseNode* node);

        /**
         * @brief Delete the current course from memory, after freeing its children.
         * @param node
         */
        static void removeAllCourse(CourseNode* node);

    public:
        /**
         * @brief Class destructor.
         */
        ~CourseManager();

        /**
         * @brief Load a CSV file containing courses into a container.
         */
        void loadCourses(const std::string& filename);

        /**
         * @brief Search for a course with an id of @p courseId, and print it's details.
         * @param courseId The courseId to match.
         */
        void printCourse(const std::string& courseId) const;

        /**
         * @brief Insert @ref course into the course manager.
         * @param course The course to insert.
         */
        void insertCourse(const Course& course);

        /**
         * @brief Print the courses in alphabetical sorting.
         */
        void printCourses() const;
    };

} // namespace abcu