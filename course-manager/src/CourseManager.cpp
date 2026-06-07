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

#include "Utils.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>

namespace abcu
{

    /**************************************
     *                                    *
     *           INITIALIZATION           *
     *                                    *
     **************************************/

    CourseManager::~CourseManager()
    {
        removeAllCourse(root);
    }


    /**************************************
     *                                    *
     *           PUBLIC METHODS           *
     *                                    *
     **************************************/

    void CourseManager::loadCourses(const std::string& filename)
    {
        std::cout << "Loading CSV file...\n\n";

        // Cache to add the existing courses so we can filter out any non-existant courses from pre-requisites
        std::unordered_set<std::string> existingCourses;

        // Set the iostream to trigger an exception
        std::cin.exceptions(std::ios_base::failbit | std::ios_base::badbit);

        try
        {
            std::ifstream csvFile(filename);

            if (!csvFile.is_open()) {
                throw std::runtime_error("Error: Cannot find or open the file.");
            }

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
        // Turn off exceptions
        std::cin.exceptions(std::ios_base::goodbit);

        removeInvalidPrerequisites(root, existingCourses);
    }



    void CourseManager::printCourse(const std::string& courseId) const
    {
        if (root == nullptr)
            std::cout << "Empty course list. Consider loading the courses first.\n";

        auto current = root;
        // Print the current course if it's ID matches the provided course id

        while (current != nullptr)
        {
            // If the current course's id and passed-in course id are equal
            // Then print the course information
            if (utils::strCmp(current->course.courseId, courseId) == 0)
            {
                std::cout << current->course;
                return;
            }

            // Else if the search course id lower, search the left subtree
            if (utils::strCmp(courseId, current->course.courseId) < 0)
                current = current->left;
            // else search the right subtree
            else
                current = current->right;
        }

        // If the function doesn't early exit
        // Then, there are no course with the given id
        std::cout << "No course with " << courseId << " found!\n";
    }


    void CourseManager::insertCourse(const Course& course)
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


    void CourseManager::printCourses() const
    {
        // Edge case: There are no courses
        if (root == nullptr)
        {
            std::cout << "No courses!";
            return;
        }

        // Helper function that recursively prints the courses
        printInOrder(root);
        std::cout << std::endl;
    }


    /**************************************
     *                                    *
     *          PRIVATE METHODS           *
     *                                    *
     **************************************/

    void CourseManager::removeInvalidPrerequisites(CourseNode* node, const std::unordered_set<std::string>& cache)
    {
        if (cache.size() < 1)
            return; // No invalidation is performed if the cache is empty

        // Base case
        if (node == nullptr)
            return; // Base case

        // Remove invalid course pre-requisite from the subtrees
        removeInvalidPrerequisites(node->left, cache);
        removeInvalidPrerequisites(node->right, cache);

        auto prerequisites = &node->course.prerequisites;
        for (auto it = prerequisites->begin(); it != prerequisites->end(); ) {
            if (cache.count(*it) == 0) {
                it = node->course.prerequisites.erase(it); // erase returns the next valid iterator
            } else {
                ++it;
            }
        }
    }


    void CourseManager::printInOrder(const CourseNode* node)
    {
        if (node == nullptr)
            return;

        printInOrder(node->left);
        std::cout << node->course.courseId << " (" << node->course.name << ")\n";
        printInOrder(node->right);
    }

    void CourseManager::removeAllCourse(CourseNode* node)
    {
        // Base case
        if (node == nullptr)
            return;
        // Recursively delete left and right subchildren
        removeAllCourse(node->left);
        removeAllCourse(node->right);
        // Afterward delete the node
        delete node;
    }

} // namespace abcu