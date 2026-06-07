/**
 * @file Main.cpp
 * @author Alan Abraham P Kochumon
 * @date Created on: May 31, 2026
 *
 * @brief Main Entry Point for the Course Manager.
 *
 * @copyright Copyright (c) 2026 Alan Abraham P Kochumon
 */

#include "ABCUApplication.h"

#include <iostream>

int main(int argc, char* argv[])
{

    // std::string filename = "ABCU_Advising_Program_Input.csv";
    // // If a commandline argument is passed-in for filename
    // if (argc > 1)
    // {
    //     // Then, that file is used
    //     filename = argv[1];
    // }

    std::cout << "Welcome to the course planner.\n";

    abcu::ABCUApplication app{};
    app.getFileInput();
    app.run();
}