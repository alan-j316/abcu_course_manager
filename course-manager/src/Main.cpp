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
    explicit ABCUApplication(const std::string& filename): _running(true), _filename(filename) {}

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
                _manager.loadCourses(_filename);
                break;
            case 2:
                _manager.printCourses();
                break;
            case 3:
                std::cout << "Searching courses...";
                break;
            case 9:
                std::cout << "Exiting application...\n";
                _running = false;
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
    CourseManager _manager;
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