/**
 * @file Course.cpp
 * @author Alan Abraham P Kochumon
 * @date Created on: June 06, 2026
 *
 * @brief Implementation of member function defined in Course.h
 *
 * @copyright Copyright (c) 2026 Alan Abraham P Kochumon
 */


#include "Course.h"

namespace abcu
{

    Course Course::parseCourse(const std::string& line)
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
}