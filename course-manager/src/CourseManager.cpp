/**
 * @file CourseManager.cpp
 * @author Alan Abraham P Kochumon
 * @date Created on: May 31, 2026
 *
 * @brief Implementation of member function defined in CourseManager.h
 *
 * @copyright Copyright (c) 2026 Alan Abraham P Kochumon
 */


#include "CourseManager.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>

namespace abcu
{
    /**************************************
     *                                    *
     *           PUBLIC METHODS           *
     *                                    *
     **************************************/

    void CourseManager::loadCourses(const std::string_view filename)
    {
        std::cout << "Loading CSV file...\n\n";

        // Set the iostream to trigger an exception
        // NOLINTNEXTLINE
        std::cin.exceptions(std::ios_base::failbit | std::ios_base::badbit);

        try
        {
            std::ifstream csvFile((filename.data()));

            if (!csvFile.is_open())
            {
                throw std::runtime_error("Error: Cannot find or open the file.");
            }

            std::string line;
            while (std::getline(csvFile, line))
            {
                try
                {
                    auto course = Course::parseCourse(line);
                    insertCourse(course);
                }
                catch (const std::exception& e)
                {
                    std::cout << e.what() << '\n';
                    // Clear the fail bit status
                    std::cin.clear();
                    // Clear the cin buffer
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << '\n';
        }
        // Turn off exceptions
        std::cin.exceptions(std::ios_base::goodbit);
        removeInvalidPrerequisites();
        _loaded = true;
    }



    void CourseManager::printCourse(const std::string& courseId) const
    {
        if (!coursesReady()) return;

        // If no courses found then print the result
        if (const auto courseIt = _courses.find(courseId); courseIt == _courses.cend())
            std::cout << "No course with " << courseId << " found!\n";
        else
            std::cout << courseIt->second;
    }


    void CourseManager::insertCourse(const Course& course)
    {
        _courses.insert_or_assign(course.courseId, course);
    }


    void CourseManager::printCourses() const
    {
        if (!coursesReady()) return;

        // Store the values into a temporary vector
        std::vector<std::pair<std::string, Course>> courseVec(_courses.begin(), _courses.end());
        std::sort(courseVec.begin(), courseVec.end(), [](const auto& a, const auto& b) {
            // Ascending order
            return a.first < b.first;
        });

        for (const auto& [key, course] : courseVec)
        {
            std::cout << course;
        }
        std::cout << '\n';
    }




    /**************************************
     *                                    *
     *          PRIVATE METHODS           *
     *                                    *
     **************************************/

    void CourseManager::removeInvalidPrerequisites()
    {
        // Although the loop is O(N*M) it only happens on load.
        // Where N is the number of courses and M is average number of prerequisites which can be utmost N.
        for (auto& [courseId, course] : _courses)
        {
            for (const auto& prerequisite : course.prerequisites)
            {
                const auto curCourseIt = _courses.find(prerequisite);
                // Remove the course requisite if it's not a valid one
                if (curCourseIt == _courses.end())
                    course.prerequisites.erase(prerequisite);
            }
        }
    }

    bool CourseManager::coursesReady() const
    {
        if (!_loaded)
        {
            std::cout << "Courses not loaded! Load the course using Option 1\n";
            return false;
        }

        if (_courses.empty())
        {
            std::cout << "Empty course list. Consider loading the courses first.\n";
            return false;
        }
        return true;
    }
} // namespace abcu