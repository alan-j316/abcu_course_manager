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


namespace abcu
{

    ABCUApplication::ABCUApplication(const std::string& filename): _running(true), _filename(filename) {}

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


    void ABCUApplication::parseUserInput()
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

} // namespace abcu