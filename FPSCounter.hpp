
#include <SFML/Graphics.hpp>

class FPSCounter
{
private:
    sf::Clock frame_clock;
    int frames_done;

public:
    FPSCounter() : frames_done(0) {}
    int getFPS()
    {
        if(frame_clock.getElapsedTime().asSeconds() < 1)
        {
            frames_done++;
            return -1;
        }
        int rate = frames_done;
        frames_done = 0;
        frame_clock.restart();
        return rate;
    }
};
