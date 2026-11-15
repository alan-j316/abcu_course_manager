/**
 * @file ABCUApplication.cpp
 * @author Alan Abraham P Kochumon
 * @date Created on: June 06, 2026
 *
 * @brief Implementation of member function defined in ABCUApplication.h
 *
 * @copyright Copyright (c) 2026 Alan Abraham P Kochumon
 */

#include "ABCUApplication.h"

#include <filesystem>
#include <iostream>
#include <utility>

namespace abcu
{

    ABCUApplication::ABCUApplication(std::string filename): _running(true), _filename(std::move(filename)) {}

    /**************************************
     *                                    *
     *           PUBLIC METHODS           *
     *                                    *
     **************************************/

    void ABCUApplication::run()
    {
        while (_running)
        {
            printMenu();
            std::cout << "Enter an option:  ";
            parseUserInput();
        }
    }



    /**************************************
     *                                    *
     *          PRIVATE METHODS           *
     *                                    *
     **************************************/

    void ABCUApplication::printMenu()
    {
        std::cout << "1. Load dataset\n";
        std::cout << "2. Print all courses (Sorted)\n";
        std::cout << "3. Search course\n";
        std::cout << "9. Exit application\n";
    }

    void ABCUApplication::dispatchAction(const int actionCode)
    {
        std::cout << '\n';

        std::string searchCourseId;
        switch (actionCode)
        {
            case 1:
                _manager.loadCourses(_filename);
                break;
            case 2:
                _manager.printCourses();
                break;
            case 3:
                std::cout << "What are you looking for?  ";
                std::cin >> searchCourseId;
                std::cout << '\n';
                _manager.printCourse(searchCourseId);
                break;
            case 9:
                std::cout << "Exiting application...\n";
                _running = false;
                break;
            default:
                throw std::invalid_argument("Invalid option! Please try again!");
        }
    }


    bool ABCUApplication::getFileInput()
    {
        std::cout << "Enter filename: \n";
        std::cin >> _filename;

        // Validate if the file exists using a file system
        if (std::filesystem::path path = _filename; !std::filesystem::exists(path))
        {
            std::cout << "'" << _filename << "' doesn't exist in path. Defaulting to default file.\n";
            // Defaults to a filename if the user provided filename is not valid.
            _filename = "ABCU_Advising_Program_Input.csv";

            // Check if the default file exist. If not exit the program.
            path = _filename;
            if (!std::filesystem::exists(path))
            {
                std::cout << "Default file doesn't exist in program path.\nUnrecoverable error. Exiting...\n";
                return false;
            }
        }
        return true;
    }


    void ABCUApplication::parseUserInput()
    {

        try
        {
            // Set the iostream to trigger an exception
            // NOLINTNEXTLINE
            std::cin.exceptions(std::ios_base::failbit | std::ios_base::badbit);

            int userInput;
            std::cin >> userInput;
            // Call a subroutine that handles the appropriate action
            dispatchAction(userInput);
        }
        catch ([[maybe_unused]] std::exception& e)
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

} // namespace abcu