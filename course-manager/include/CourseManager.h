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



#include <string>
#include <unordered_map>

#include "Course.h"


namespace abcu
{
    class CourseManager
    {
    public:
        /**
         * @brief Load a CSV file containing courses into a container.
         */
        void loadCourses(std::string_view filename);

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



    private:
        /**
         * @brief Remove invalid pre-requisites from the courses.
         *        A prerequisite is considered invalid, if that course id is not present in the course list.
         *
         */
        void removeInvalidPrerequisites();

    protected:
        std::unordered_map<std::string, Course> _courses;
    };

} // namespace abcu