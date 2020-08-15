cmake_minimum_required(VERSION 3.17)
project(CAU_CAPSTONE_Phasor)

set(CMAKE_CXX_STANDARD 17)
set(OpenCV_DIR $ENV{OPENCV_DIR})

find_package(OpenCV REQUIRED)
include_directories($ENV{OPENCV_INCLUDE})

add_executable(CAU_CAPSTONE_Phasor main.cpp networks/YOLO.cpp networks/YOLO.h utils/config.h utils/utils.cpp utils/utils.h utils/Scheduler.cpp utils/Scheduler.h)

set(OpenCV_LIBS opencv_core opencv_imgproc opencv_dnn opencv_video opencv_videoio opencv_highgui)

target_link_libraries(CAU_CAPSTONE_Phasor ${OpenCV_LIBS})
