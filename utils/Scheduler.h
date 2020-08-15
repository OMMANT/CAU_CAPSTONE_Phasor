//
// Created by xoghks on 2020-08-15.
// Files for Scheduling tasks
//

#ifndef CAU_CAPSTONE_PHASOR_SCHEDULER_H
#define CAU_CAPSTONE_PHASOR_SCHEDULER_H
#include <ctime>
#include "../networks/YOLO.h"
#include "config.h"
#define SCHEDULE_SIZE   4
#define CLOCKS_PER_MS   CLOCKS_PER_SEC / 1000


class Scheduler {
protected:
    clock_t reference_time[SCHEDULE_SIZE]{1, 10, 100, 1000};
    clock_t current_times[SCHEDULE_SIZE]{}; // Array for save current time(1ms, 10ms, 100ms, 1s)
    unsigned int counters[SCHEDULE_SIZE]{}; // Array for save counts(1ms, 10ms, 100ms, 1s)
    void(*fps[SCHEDULE_SIZE])(Scheduler&); // Array for func pointer(1ms, 10ms, 100ms, 1s)
    int running_idx;
    void setup();
    void end();
    bool waitCamera(int waitTime=-1);

public:
    bool break_loop;
    Scheduler();
    void update_time();
    void setup_counter();
    void run();
    unsigned int get_counter();
    cv::dnn::Net network;
    cv::VideoCapture cap;
    cv::Mat blob, frame;
    std::vector<cv::Mat> outs;
};

void func_1ms(Scheduler& scheduler);
void func_10ms(Scheduler& scheduler);
void func_100ms(Scheduler& scheduler);
void func_1s(Scheduler& scheduler);
void empty_func();

#endif //CAU_CAPSTONE_PHASOR_SCHEDULER_H
