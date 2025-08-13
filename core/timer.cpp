#include "timer.h"

namespace Timer {
    void start() {
        startTime = std::chrono::high_resolution_clock::now();
    }

    void stop() {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        Utils::print("Elapsed time: ", duration, " ms");
    }
}