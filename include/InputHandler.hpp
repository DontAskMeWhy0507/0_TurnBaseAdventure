#ifndef TURN_BASE_ADVENTURE_INPUT_HANDLER_HPP
#define TURN_BASE_ADVENTURE_INPUT_HANDLER_HPP

#include <string>

/**
 * Static utility class for validated console input.
 * Handles cin.fail(), buffer clearing, range checking, and empty-input rejection.
 * Maps to requirement TC-18 (input error handling, anti-infinite-loop).
 */
class InputHandler {
public:
    /// Read an integer from stdin in [min, max]. Keeps prompting on invalid input.
    static int getInt(const std::string& prompt, int min, int max);

    /// Read a line from stdin. If allowEmpty is false, keep prompting until
    /// non-empty.
    static std::string getString(const std::string& prompt, bool allowEmpty = false);

    /// Prompt with [y/n]; only 'y', 'Y', 'n', 'N' accepted.
    static bool getYesNo(const std::string& prompt);

    /// Display a message and wait for Enter.
    static void pause(const std::string& msg = "Press Enter to continue...");

    InputHandler() = delete;
};

#endif // TURN_BASE_ADVENTURE_INPUT_HANDLER_HPP
