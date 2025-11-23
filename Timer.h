#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
public:
    Timer();
    
    void start();
    void stop();
    void pause();
    void reset();
    
    void setPeriod(float seconds);    // define o período da contagem
    
    bool isFinished() const;          // retorna true se o tempo terminou
    float getDuration() const;        // retorna o tempo decorrido em segundos

private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    TimePoint startTime;
    float accumulated;     // tempo acumulado durante pausas
    float period;          // tempo necessário para "terminar"
    bool running;
    bool paused;
};

#endif
