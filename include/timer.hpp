#include "numeric_aliases.hpp"

class timer
{
    public:
        //The various clock actions
    void start() {
        paused_ = false;
        started_ = true;
        start_ticks_ = SDL_GetPerformanceCounter();
        paused_ticks_ = 0;
    }
    void stop() {
        paused_ = false;
        started_ = false;
        start_ticks_ = 0;
        paused_ticks_ = 0;
    }
    void pause() {

        if( started_ && paused_ ) {
        paused_ = true;
        paused_ticks_ = SDL_GetPerformanceCounter() - start_ticks_;
        start_ticks_ = 0;
    }
    }
    void unpause() {
        if (started_ && paused_) {
            paused_ = false;
            start_ticks_ = SDL_GetPerformanceCounter() - paused_ticks_;
            paused_ticks_ = 0;
        }
    }

        //Gets the timer's time
    tl::u64 get_ticks() {
        if (started_) {
            if (paused_) {
                return paused_ticks_;
            }
            return (SDL_GetPerformanceCounter() - start_ticks_) * 1000 / SDL_GetPerformanceFrequency();
        }
        return 0;
    }

        //Checks the status of the timer
    bool is_started() { return started_; }
    bool is_paused() { return paused_ && started_; }

    private:
    tl::u64 start_ticks_ = 0;
    tl::u64 paused_ticks_ = 0;

        //The timer status
    bool paused_ = false;
        bool started_ = false;
};
