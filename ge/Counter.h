#pragma once

#include "utility.h"

namespace ge {
    class Counter {
    public:
        void start() {
            LARGE_INTEGER li;

            if (!QueryPerformanceFrequency(&li))
                throw "QueryPerformanceFrequency failed!";

            PCFreq = double(li.QuadPart) / 1000.0;

            QueryPerformanceCounter(&li);
            CounterStart = li.QuadPart;
        }
        double getTime() {
            LARGE_INTEGER li;
            QueryPerformanceCounter(&li);
            return double(li.QuadPart - CounterStart) / PCFreq;
        }

    private:
        double PCFreq = 0.0;
        __int64 CounterStart = 0;
    };
}
