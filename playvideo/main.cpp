#include <Controller.h>

int main(int argc, char *argv[])
{
    Controller controller(std::string(APP_PATH)+"/video.txt");
    std::thread watcherThread(&Controller::checkFileChange, &controller);
    watcherThread.join();
    return 0;
}
