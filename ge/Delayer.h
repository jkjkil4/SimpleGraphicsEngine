#pragma once

#include <thread>

#include "Counter.h"

namespace ge {
    class Delayer {
    public:
        explicit Delayer(int standardMicro)
            : standardMicro(standardMicro)
        {
            counter.start();
        }

        void delay(int micro/*, bool skipBelowOffset = false*/) {
            needDelayMicro += micro;
            if (needDelayMicro > 0) {
                double startTime = counter.getTime();   //得到延时开始的时间(ms)
                std::this_thread::sleep_for(std::chrono::microseconds(needDelayMicro)); //延时
                int delayedMicro = (int)((counter.getTime() - startTime) * 1000);   //得到延时消耗的微秒数
                int outMicro = delayedMicro - needDelayMicro;   //得到超出的微秒数
                needDelayMicro = -outMicro; //将 needDelayMicro 设置为 -outMicro，也就是在下一次延时的时候就扣去了这个事件
            }
            /*else {
                if (skipBelowOffset && needDelayMicro < -standardMicro * 4)
                    needDelayMicro = 1;
            }*/
        }

        //private:
        Counter counter;
        int standardMicro = 0;
        int needDelayMicro = 0;
    };
}