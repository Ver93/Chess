#pragma once

#include <chrono>

#include "utils.h"

namespace Timer {
    static std::chrono::high_resolution_clock::time_point startTime;

    void start() {
        startTime = std::chrono::high_resolution_clock::now();
    }

    void stop() {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        Utils::print("Elapsed time: ", duration, " ms");
    }
}