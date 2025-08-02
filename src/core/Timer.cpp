#include "core/Timer.h"

Timer::Timer() {
    start();
}

void Timer::start() {
    m_startTime = std::chrono::high_resolution_clock::now();
}

float Timer::restart() {
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = endTime - m_startTime;
    m_startTime = endTime;
    return duration.count();
}

float Timer::elapsed() const {
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = endTime - m_startTime;
    return duration.count();
}