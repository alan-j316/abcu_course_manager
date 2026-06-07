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


#include "Utils.h"

#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>

namespace abcu
{

    class Course
    {
    public:
        // Course Name and id
        std::string name, courseId;
        // Set of all the pre-requisites
        // Set is used here to ensure O(1) access and deletion when removing invalid prerequisites
        std::unordered_set<std::string> prerequisites;

        /**
         * @brief Parse a `std::string` to a course object.
         * @param line The line to parse.
         * @return The @ref Course object.
         *
         * @throws `std::invalid_argument` if the string parsing fail with invalid course name or course id.
         */
        static Course parseCourse(const std::string& line)
        {
            std::stringstream ss(line);
            std::string currentToken;
            int iteration = 0;
            Course course;
            while (std::getline(ss, currentToken, ','))
            {
                if (iteration == 0)
                {
                    // Course ID
                    // If the course name is empty, then it is not a valid course
                    if (currentToken.length() < 1)
                        throw std::invalid_argument("[Invalid Course]: Empty Course ID");

                    course.courseId = currentToken;
                }
                else if (iteration == 1)
                {
                    // Course name
                    // If the course name is empty, then it is not a valid course
                    if (currentToken.length() < 1)
                        throw std::invalid_argument("[Invalid Course]: Empty Course Name");
                    course.name = currentToken;
                }
                else
                {
                    // Prerequisites
                    if (currentToken.length() > 1)
                        course.prerequisites.insert(currentToken);
                }
                // Increment the iteration count to move forward
                ++iteration;
            }

            return course;
        }


        /**
         * @brief Stream @p course to the output stream for printing.
         *
         * @param stream The ostream to stream the object to.
         * @param course The object to stream.
         *
         * @return A reference to @p stream that has the object.
         */
        friend std::ostream& operator<<(std::ostream& stream, const Course& course)
        {
            stream << course.name << " (" << course.courseId << ")\n";
            stream << "Prerequisites: ";

            // Print all the prerequisites in a comma separated form.
            int prerequisiteCount = 0;
            for (const auto& prerequisite : course.prerequisites)
            {
                stream << prerequisite;
                // Only output the delimiter if the current prerequisite is not the last pre-requisite
                if (++prerequisiteCount < course.prerequisites.size())
                    stream << ", ";
            }

            // If there are no pre-requisites, let the user know
            if (prerequisiteCount == 0)
                stream << "None";

            stream << "\n\n";
            return stream;
        }
    };

    class CourseNode
    {
    public:
        Course course;

        // Initialize left and right with nullptr
        CourseNode* left{ nullptr };
        CourseNode* right{ nullptr };
    };

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