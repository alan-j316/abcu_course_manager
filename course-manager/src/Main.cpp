/**
 * @file Main.cpp
 * @author Alan Abraham P Kochumon
 * @date Created on: May 31, 2026
 *
 * @brief Main Entry Point for the Course Manager.
 *
 * @copyright Copyright (c) 2026 Alan Abraham P Kochumon
 */

#include "CSVparser.hpp"
#include "CourseManager.h"

#include <fstream>
#include <iostream>
#include <limits>

class ABCUApplication
{
public:
    // Initialization
    ABCUApplication(const std::string& filename): _running(true), _filename(filename) {}

    /**
     * @brief Prints the application menu to screen
     */
    static void printMenu()
    {
        std::cout << "1. Load dataset\n";
        std::cout << "2. Print all courses (Sorted)\n";
        std::cout << "3. Search course\n";
        std::cout << "9. Exit application\n";
    }

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
     * @brief Load a CSV file containing courses into a container.
     */
    static void loadCourses(const std::string& filename)
    {
        std::cout << "Loading CSV file..." << '\n';

        // Cache to add the existing courses so we can filter out any non-existant courses from pre-requisites
        std::unordered_set<std::string> existingCourses;

        CourseManager manager;

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
                    auto course = parseCourse(line);
                    // Add the CourseId to the cache
                    existingCourses.insert(course.courseId);
                    manager.insertCourse(course);
                    std::cout << course;
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
        manager.printCourse();
        // Turn off exceptions
        std::cin.exceptions(std::ios_base::goodbit);
    }


    /**
     * @brief Dispatch an action based on the @p actionCode.
     * @param actionCode The code/identifier of the action to perform.
     *
     * @throw `std::invalid_argument` if the @p actionCode is out of range.
     */
    void dispatchAction(const int actionCode)
    {
        switch (actionCode)
        {
            case 1:
                loadCourses(_filename);
                break;
            case 2:
                std::cout << "Printing courses...";
                break;
            case 3:
                std::cout << "Searching courses...";
                break;
            case 9:
                std::cout << "Exiting application...";
                break;
            default:
                throw std::invalid_argument("Invalid option! Please try again!");
        }
    }

    /**
     * @brief Parsers the user input calls the appropriate action
     */
    void parseUserInput()
    {

        try
        {
            // Set the iostream to trigger an exception
            std::cin.exceptions(std::ios_base::failbit | std::ios_base::badbit);

            int userInput;
            std::cin >> userInput;
            // Call a subroutine that handles the appropriate action
            dispatchAction(userInput);
        }
        catch (std::exception& e)
        {
            // Print the error message
            std::cout << "Invalid option. Please try again!\n";

            // std::cout << "[Error]: " << e.what() << '\n'; // NOTE: For Diagnosis only.

            // Clear the failbit status
            std::cin.clear();
            // Clear the cin buffer
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        // Turn off exceptions
        std::cin.exceptions(std::ios_base::goodbit);
    }

    // Displays menu
    // Extracts user input
    // Does appropriate action
    void run()
    {
        while (_running)
        {
            printMenu();
            std::cout << "Enter an option: ";
            parseUserInput();
        }
    }

private:
    bool _running;
    std::string _filename;
};

int main(int argc, char* argv[])
{

    std::string filename = "ABCU_Advising_Program_Input.csv";
    // if (argc > 2)
    // {
    //     filename = argv[1];
    //     std::cout << filename;
    // }

    ABCUApplication app(filename);
    app.run();
}