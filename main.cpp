#include <iostream>
#include <ctime>
#include "utils/Scheduler.h"

int main(int argc, char** args){
    time_t curr_time = time(nullptr);
    struct tm *curr_tm = localtime(&curr_time);
    printf("Start Running: %04d/%02d/%02d - %02d:%02d:%02d\n",
           curr_tm->tm_year + 1900, curr_tm->tm_mon + 1, curr_tm->tm_mday,
           curr_tm->tm_hour, curr_tm->tm_min, curr_tm->tm_sec);
    Scheduler scheduler = Scheduler();
    scheduler.run();
    return 0;
}