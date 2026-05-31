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


#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>

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

public:
    /**
     * @brief Load a CSV file containing courses into a container.
     */
    void loadCourses(const std::string& filename)
    {
        std::cout << "Loading CSV file..." << '\n';

        // Cache to add the existing courses so we can filter out any non-existant courses from pre-requisites
        std::unordered_set<std::string> existingCourses;

        // Set the iostream to trigger an exception
        std::cin.exceptions(std::ios_base::failbit | std::ios_base::badbit);

        try
        {
            std::ifstream csvFile(filename);

            std::string line;
            while (std::getline(csvFile, line))
            {
                try
                {
                    auto course = Course::parseCourse(line);
                    // Add the CourseId to the cache
                    existingCourses.insert(course.courseId);
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
        std::cout << "Printing Courses...\n\n";
        // Turn off exceptions
        std::cin.exceptions(std::ios_base::goodbit);
    }



    void printCourse(const std::string& courseId)
    {

    }

        // TODO: remove invalid prerequisites
        //  TODO: find course
        //  TODO: Destructor


        /**
         * @brief Insert @ref course into the course manager.
         * @param course The course to insert.
         */
        void insertCourse(const Course& course)
    {
        auto node = new CourseNode;
        node->course = course;

        if (root == nullptr)
            root = node;
        else
        {
            auto current = root;
            while (true)
            {
                // If the course to insert has name with lower lexicographical index
                // then move to the left branch
                if (course.courseId < current->course.courseId)
                {
                    if (current->left == nullptr)
                    {
                        // Insert the node at left that slot is empty
                        current->left = node;
                        return; // Return to exit the loop since the node is inserted
                    }
                    // Move forward
                    current = current->left;
                }
                // Else move to the right branch
                else
                {
                    if (current->right == nullptr)
                    {
                        // Insert the node at left that slot is empty
                        current->right = node;
                        return; // Return to exit the loop since the node is inserted
                    }
                    // Move forward
                    current = current->right;
                }
            }
        }
    }

    /**
     * @brief Traverse and print the courses in an in-order format.
     * @param node The current root.
     */
    static void printInOrder(const CourseNode* node)
    {
        if (node == nullptr)
            return;

        printInOrder(node->left);
        std::cout << node->course;
        printInOrder(node->right);
    }


    /**
     * @brief Print the course in alphabetical sorting.
     */
    void printCourses()
    {
        // Edge case: There are no courses
        if (root == nullptr)
        {
            std::cout << "No courses!";
            return;
        }

        // Helper function that recursively prints the courses
        printInOrder(root);
    }
};