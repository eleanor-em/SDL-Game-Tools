#ifndef FPS_H
#define FPS_H

#include <chrono>

class FPS {
    typedef std::chrono::steady_clock Clock;
    typedef Clock::time_point Time;
    typedef std::chrono::duration<double> Duration;
    public:
        FPS(int targetFps = 60, int sampleRate = 20, int countMax = 50)
            : DT(1000.0 / targetFps), SAMPLE_RATE(sampleRate),
              FPS_COUNT_MAX(countMax), fpscount(0), fpsnum(0), steps(0), prevTime(Clock::now()) { }
        
        // returns delta
        double update();       
        
    private:
        const double DT;
        const int SAMPLE_RATE;
        const int FPS_COUNT_MAX;
        
        double fpscount;
        int fpsnum;
        int steps;
        Time now;
        Time prevTime;
};
#endif
