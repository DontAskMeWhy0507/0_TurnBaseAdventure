#include "InputHandler.hpp"

#include <iostream>
#include <limits>

int InputHandler::getInt(const std::string& prompt, int min, int max) {
    int value = 0;
    while (true) {
        std::cout << prompt;
        std::cin >> value;

        // Check for stream failure (non-numeric input)
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  [ERROR] Please enter a valid number." << std::endl;
            continue;
        }

        // Discard the rest of the line to prevent left-over characters
        // from bleeding into the next read.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // Range check
        if (value < min || value > max) {
            std::cout << "  [ERROR] Number must be between "
                      << min << " and " << max << "." << std::endl;
            continue;
        }

        return value;
    }
}

std::string InputHandler::getString(const std::string& prompt, bool allowEmpty) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);

        // Handle EOF (Ctrl+Z on Windows, Ctrl+D on *nix)
        if (std::cin.eof()) {
            std::cin.clear();
            std::cout << "\n  [INFO] EOF detected — returning empty string." << std::endl;
            return "";
        }

        if (!allowEmpty && input.empty()) {
            std::cout << "  [ERROR] Input cannot be empty. Please try again." << std::endl;
            continue;
        }

        return input;
    }
}

bool InputHandler::getYesNo(const std::string& prompt) {
    while (true) {
        std::cout << prompt << " [y/n]: ";
        std::string line;
        std::getline(std::cin, line);

        if (line == "y" || line == "Y") return true;
        if (line == "n" || line == "N") return false;

        std::cout << "  [ERROR] Please answer 'y' or 'n'." << std::endl;
    }
}

void InputHandler::pause(const std::string& msg) {
    std::cout << msg;
    std::cin.get();
}
