//
// Created by xoghks on 2020-08-14.
// File for Setup Network and file paths
//

#include "YOLO.h"

void setupNetwork(cv::dnn::Net &net,
                  const std::string &cfgFile,
                  const std::string &darknetModel){
    net = cv::dnn::readNetFromDarknet(cfgFile, darknetModel);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
}

vector<string> getYOLONames(string name_path){
    static std::vector<string> classes;
    std::ifstream f;
    std::string line;
    f.open(name_path);
    if (!f) {
        std::cout << "YOLO-Name file is not opened" << std::endl;
        exit(0);
    }
    while (getline(f, line)) {
        classes.push_back(line);
    }
    f.close();
    return classes;
}

vector<string> getOutPutsNames(const cv::dnn::Net& network){
    static vector<string> names;
    if(names.empty()){
        vector<int> outLayers = network.getUnconnectedOutLayers();
        vector<string> layersNames = network.getLayerNames();
        names.resize(outLayers.size());
        for(size_t i = 0; i < outLayers.size(); i++)
            names[i] = layersNames[outLayers[i] - 1];
    }
    return names;
}

void PostProcess(Mat& frame, vector<Mat>& outs) {
    vector<String> names;
    vector<float> confidences;
    vector<Rect> boxes;
    vector<string> classes = getYOLONames(YOLO_NAME_PATH);

    for (size_t i = 0; i < outs.size(); i++) {
        float* data = (float*)outs[i].data;
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
        {
            Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            Point classId;
            double confidence;
            minMaxLoc(scores, 0, &confidence, 0, &classId);
            if (confidence > MIN_CONFIDENCE) {
                int center_x = (int)(data[0] * WIDTH) < 0 ? 0 : (int)(data[0] * WIDTH);
                int center_y = (int)(data[1] * HEIGHT) < 0 ? 0 : (int)(data[1] * HEIGHT);
                int w = center_x + int(data[2] * WIDTH) > frame.cols ? frame.cols - center_x : int(data[2] * WIDTH);
                int h = center_y + int(data[3] * HEIGHT) > frame.rows ? frame.rows - center_y : int(data[3] * HEIGHT);
                int left = center_x - w / 2 < 0 ? 0 : center_x - w / 2;
                int top = center_y - h / 2 < 0 ? 0 : center_y - h / 2;

                confidences.push_back((float)confidence);
                boxes.push_back(Rect(left, top, w, h));
                names.push_back(classes[classId.x]);
            }
        }
    }
    vector<int> indices;
    dnn::NMSBoxes(boxes, confidences, MIN_CONFIDENCE, NMS_CONFIDENCE, indices);
    cout << indices.size() << endl;
    for (size_t i = 0; i < indices.size(); i++) {
        int idx = indices[i];
        Rect box = boxes[idx];
        String label = names[i];
        cout << box.x << " " << box.y << " " << box.width << " " << box.height << endl;
        imshow(label, frame(Rect(box.x, box.y, box.width, box.height)));
        /*rectangle(frame, Point(box.x, box.y), Point(box.x + box.WIDTH, box.y + box.HEIGHT), Scalar(0, 0, 255));
        putText(frame, label, Point(box.x, box.y), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255));*/
    }
}
