#include "Controller.h"

// Function to get the modification time of the file
time_t Controller::getFileLastModifiedTime(const std::string &filePath) {
    struct stat fileStat;
    if (stat(filePath.c_str(), &fileStat) == 0) {
        return fileStat.st_mtime;  // Returns the last modified time
    }
    return 0; // Error getting time
}

// Constructor
Controller::Controller(const std::string &filePath) : filePath(filePath) {
    m_videoPlayer = std::make_shared<VideoPlayer>();
    lastModifiedTime = getFileLastModifiedTime(filePath);
    std::cout << "Watching file: " << filePath << std::endl;
}

// Check if the file has changed
void Controller::checkFileChange() {
    while (true) {
        time_t currentModifiedTime = getFileLastModifiedTime(filePath);
        if (currentModifiedTime != lastModifiedTime) {
            lastModifiedTime = currentModifiedTime;
            std::cout << "File changed signal received!" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            readFile();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Check every 500ms
    }
}

// Read file content
void Controller::readFile() {
    std::cout << "Reading file..." << std::endl;
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << "Processing line: " << line << std::endl;
        processLine(line);
    }
    file.close();
}

// Process each line in the file
void Controller::processLine(const std::string &line) {
    size_t pos = line.find('=');
    if (pos == std::string::npos) return;

    std::string key = line.substr(0, pos);
    std::string value = line.substr(pos + 1);

    key.erase(0, key.find_first_not_of(" \t"));
    key.erase(key.find_last_not_of(" \t") + 1);
    value.erase(0, value.find_first_not_of(" \t"));
    value.erase(value.find_last_not_of(" \t") + 1);

    if (key == "VideoName") {
        std::cout << "Video file set to: " << value << std::endl;
        m_videoPlayer->stop();
        m_videoPlayer->setVideoPath(std::string(APP_PATH) + "/video/" + value);
        m_videoPlayer->start();
    } else if (key == "status") {
        std::cout << "Video status changed to: " << value << std::endl;
        if(value == "resume"){
            m_videoPlayer->resume();
        }
        else if (value == "pause"){
            m_videoPlayer->pause();
        }
    } else if (key == "speed") {
        std::cout << "Playback speed changed to: " << value << std::endl;
        m_videoPlayer->setSpeed(std::stoi(value));
    } else if (key == "time") {
        std::cout << "Playback time set to: " << std::stoi(value) << std::endl;
        m_videoPlayer->seekTo(std::stoi(value));
    } else {
        std::cerr << "Unknown command: " << line << std::endl;
    }
}
