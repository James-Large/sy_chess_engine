#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <ctime>

class Timer {

public:

	Timer() { }

	void start() {
        startedAt = clock();
	}

	double restart() {
        double timeElapsed = stop();
        start();
        return timeElapsed;
	}

	double stop() {
	    double timeElapsed = double(clock() - startedAt) / (CLOCKS_PER_SEC*1000) / 1000;
        return timeElapsed;
	}

private:
	clock_t startedAt;
};

#endif // TIMER_H_INCLUDED
