#pragma once
/**
 * @file ABCUApplication.h
 * @author Alan Abraham P Kochumon
 * @date Created on: June 07, 2026
 *
 * @brief ABCU Application.
 *
 * @copyright Copyright (c) 2026 Alan Abraham P Kochumon
 */


#include "CourseManager.h"

#include <iostream>

namespace abcu
{

    class ABCUApplication
    {
    public:
        explicit ABCUApplication(const std::string& filename = "");

        /**
         * @brief Run the application loop.
         *
         * @note The application maintains its own internal running state, so there is no need to put
         *       run in another application loop.
         */
        void run();


        /**
         * @brief Take the file name for parsing as input from user.
         */
        void getFileInput();

    private:
        bool _running;
        std::string _filename;
        CourseManager _manager;

        /**
         * @brief Parse the user input and perform appropriate action.
         */
        void parseUserInput();

        /**
         * @brief Prints the application menu to screen
         */
        static void printMenu();

        /**
         * @brief Dispatch an action based on the @p actionCode.
         * @param actionCode The code/identifier of the action to perform.
         *
         * @throw `std::invalid_argument` if the @p actionCode is out of range.
         */
        void dispatchAction(int actionCode);

    };

} // namespace abcu
