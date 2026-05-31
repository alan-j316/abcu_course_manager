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


#include <deque>
#include <iostream>
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
    CourseNode* left{nullptr};
    CourseNode* right{nullptr};
};

class CourseManager
{
    CourseNode* root{ nullptr };

public:
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
                if (course.name < current->course.name)
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


    static void printInOrder(const CourseNode* node)
    {
        if (node == nullptr)
            return;

        printInOrder(node->left);
        std::cout << node->course;
        printInOrder(node->right);
    }


    void printCourse()
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