//
// Created by xoghks on 2020-08-14.
// File for Setup Network and file paths
//

#ifndef CAU_CAPSTONE_PHASOR_YOLO_H
#define CAU_CAPSTONE_PHASOR_YOLO_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <fstream>
#include "../utils/config.h"

using namespace cv;
using namespace std;

void setupNetwork(cv::dnn::Net &net,
                  const std::string &cfgFile=YOLO_CFG_PATH,
                  const std::string &darknetModel=YOLO_WEIGHT_PATH);

vector<string> getYOLONames(string name_path);
vector<string> getOutPutsNames(const cv::dnn::Net& network);
void PostProcess(cv::Mat& frame, vector<cv::Mat>& outs);


#endif //CAU_CAPSTONE_PHASOR_YOLO_H
