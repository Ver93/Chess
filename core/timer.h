#pragma once

#include <chrono>

#include "utils.h"

namespace Timer {
    static std::chrono::high_resolution_clock::time_point startTime;
    void start();
    void stop();
}