#ifndef TURN_BASE_ADVENTURE_GAME_HPP
#define TURN_BASE_ADVENTURE_GAME_HPP

#include <string>

constexpr const char* GAME_VERSION = "0.1.0";

/**
 * Core Game class — owns the main game loop.
 */
class Game {
public:
    Game();
    ~Game();

    /**
     * Initialize subsystems before the game loop starts.
     * @return true on success
     */
    bool init();

    /**
     * Run the main game loop. Blocks until the game ends.
     */
    void run();

    /**
     * Cleanup resources after the game loop exits.
     */
    void shutdown();

private:
    bool m_running;
    std::string m_savePath;
};

#endif // TURN_BASE_ADVENTURE_GAME_HPP
