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

    std::string filename = "ABCU_Advising_Program_Input2.csv";
    if (argc > 1)
    {
        filename = argv[1];
        std::cout << "\nInput File: ";
        std::cout << filename;
    }

    abcu::ABCUApplication app(filename);
    app.run();
}