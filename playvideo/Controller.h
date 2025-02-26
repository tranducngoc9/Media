#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <memory>
#include <sys/stat.h>
#include "VideoPlayer.h"

class Controller {
private:
    std::string filePath;
    std::shared_ptr<VideoPlayer> m_videoPlayer;
    bool m_parsingFile = false;
    time_t lastModifiedTime;

    time_t getFileLastModifiedTime(const std::string &filePath);

public:
    explicit Controller(const std::string &filePath);
    ~Controller();
    void checkFileChange();
    void readFile();
    void processLine(const std::string &line);
};

#endif // CONTROLLER_H
