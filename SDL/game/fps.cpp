#include <cstdio>
#include <chrono>
#include "fps.h"

double FPS::update() {
    now = Clock::now();
    Duration dt = std::chrono::duration_cast<Duration>(now - prevTime);
    prevTime = now;
    double ms = dt.count() * 1000.0;
    
    // running average of fps
    if (ms > 0) {
        fpscount += (1.0 / (ms / 1000.0));
        fpsnum++;
    }
    
    // take a sample
    if (fpsnum % SAMPLE_RATE == 0) {
    	// printf is *way* faster than iostream for this purpose
    	std::printf("%02.2f ms\t%02.2f fps\n", ms, fpscount / fpsnum);
    }
    
    // reset the average
    if (fpsnum > FPS_COUNT_MAX) {
        fpsnum = 0;
        fpscount = 0;
    }
    
    // returns the delta as a ratio
    return ms / DT;
}

