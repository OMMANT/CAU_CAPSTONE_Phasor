//
// Created by xoghks on 2020-08-15.
// Files for Scheduling tasks
//

#include "Scheduler.h"

bool Scheduler::waitCamera(int waitTime) {
    cap = cv::VideoCapture(0);

    if(cap.isOpened())
        return true;
    else{
        std::cerr << "Waiting for Camera!";
        if(waitTime > 0){
            clock_t startTime = clock();
            while(waitTime > 0){
                clock_t endTime = clock();
                if(endTime - startTime >= CLOCKS_PER_SEC){
                    std::cerr << "Last try: " << waitTime << std::endl;
                    cap = cv::VideoCapture(0);
                    if(cap.isOpened())
                        return true;
                    startTime = endTime;
                    waitTime--;
                }
            }
            if(!cap.isOpened()){
                std::cerr << "Failed to open camera!" << std::endl;
                return false;
            }
        }
        else{
            clock_t startTime = clock();
            while(true){
                clock_t endTime = clock();
                if(endTime - startTime >= CLOCKS_PER_SEC){
                    cap = cv::VideoCapture(0);
                    if(cap.isOpened())
                        return true;
                }
            }
        }
    }
    return false;
}

Scheduler::Scheduler(){
    void (*temp_fp[SCHEDULE_SIZE])(Scheduler&) = {func_1ms, func_10ms, func_100ms, func_1s};
    update_time();
    setup_counter();
    for(int i = 0; i < SCHEDULE_SIZE; i++)
        fps[i] = temp_fp[i];
    running_idx = -1;
}

void Scheduler::update_time(){
    for(clock_t &current_time : current_times)
        current_time = clock();
}

void Scheduler::setup_counter(){
    for(unsigned int &counter : counters)
        counter = 0;
}

unsigned int Scheduler::get_counter() {
    return this->counters[this->running_idx];
}

void Scheduler::run(){
    setup();
    while(true) {
        clock_t time = clock();
        for (int i = 0; i < SCHEDULE_SIZE; i++) {
            if (time - current_times[i] >= CLOCKS_PER_MS * reference_time[i]) {
                running_idx = i;
                fps[i](*this);
                current_times[i] = clock();
                counters[i]++;
            }
        }
        if(break_loop)
            break;
    }
    end();
    cv::destroyAllWindows();
}

void Scheduler::setup(){
    waitCamera(5);
    std::cout << "Camera is Opened!" << std::endl;
    setupNetwork(network);
}

void Scheduler::end(){
    cv::destroyAllWindows();
}

void func_1ms(Scheduler& scheduler){
    empty_func();
}

void func_10ms(Scheduler& scheduler){
    if(scheduler.get_counter() % 5){
        scheduler.cap >> scheduler.frame;
        cv::resize(scheduler.frame, scheduler.frame, cv::Size(WIDTH, HEIGHT));
        cv::dnn::blobFromImage(scheduler.frame, scheduler.blob, 1.0 / 255.0,
                               cv::Size(416, 416), cv::Scalar(),
                               true, false);
        scheduler.network.setInput(scheduler.blob);
        scheduler.network.forward(scheduler.outs, getOutPutsNames(scheduler.network));
        PostProcess(scheduler.frame, scheduler.outs);
        scheduler.outs.clear();
    }
}

void func_100ms(Scheduler& scheduler){
    scheduler.break_loop = cv::waitKey(1) == 27;
    //Run every 500ms (5 * 100ms)
    if(scheduler.get_counter() % 5){
    }
}

void func_1s(Scheduler& scheduler){
    std::cout << "1s elapsed!" << std::endl;
}

void empty_func(){
    ;
}