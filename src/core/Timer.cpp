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

void Timer::update(float deltaTime)
{
    // This method can be used to update the timer with a delta time if needed.
    // Currently, it does nothing as the timer is based on high-resolution clock.
	// You can implement custom logic here if required.
}

float Timer::elapsed() const {
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = endTime - m_startTime;
    return duration.count();
}