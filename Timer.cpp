#include "Timer.h"

Timer::Timer()
    : accumulated(0.0f), period(1.0f), running(false), paused(false)
{
}

void Timer::setPeriod(float seconds) {
    period = seconds;
}

void Timer::start() {
    startTime = Clock::now();
    accumulated = 0.0f;
    running = true;
    paused = false;
}

void Timer::pause() {
    if (running && !paused) {
        auto now = Clock::now();
        accumulated += std::chrono::duration<float>(now - startTime).count();
        paused = true;
    }
}

void Timer::stop() {
    if (running && !paused) {
        auto now = Clock::now();
        accumulated += std::chrono::duration<float>(now - startTime).count();
    }
    
    running = false;
    paused = false;
}

void Timer::reset() {
    startTime = Clock::now();
    accumulated = 0.0f;
    paused = false;
}

bool Timer::isFinished() const {
    if (running == false)
        return false;

    float elapsed;

    if (paused) {
        elapsed = accumulated;
    } else {
        auto now = Clock::now();
        elapsed = accumulated + std::chrono::duration<float>(now - startTime).count();
    }

    return elapsed >= period;
}

float Timer::getDuration() const {
    if (!running) {
        // timer parado -> retorna o último valor acumulado
        return accumulated;
    }

    if (paused) {
        return accumulated;
    }

    auto now = Clock::now();
    return accumulated + std::chrono::duration<float>(now - startTime).count();
}

