#pragma once
/**
 * @file CourseManager.h
 * @author Alan Abraham P Kochumon
 * @date Created on: May 31, 2026
 *
 * @brief ABCU course entity encapsulating the course id, name, and pre-requisite.
 *
 * @copyright Copyright (c) a Alan Abraham P Kochumon
 */


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
        static Course parseCourse(const std::string& line);


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

} // namespace abcu