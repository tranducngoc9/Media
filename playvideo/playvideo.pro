
CONFIG += c++11


DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += APP_PATH=\\\"$$PWD\\\"

message(DEFINES: $$DEFINES)

INCLUDEPATH += /usr/include/opencv4
LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio #opencv lib

HEADERS += \
    Controller.h \
    VideoPlayer.h
SOURCES += \
        Controller.cpp \
        VideoPlayer.cpp \
        main.cpp

RESOURCES +=

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


# Định nghĩa đường dẫn thư mục build
BUILD_DIR = $$PWD/build
message(Build directory: $$BUILD_DIR)

# Đặt nơi build thành thư mục `build` trong App PlaVideo
DESTDIR = $$BUILD_DIR

# Định nghĩa đường dẫn app
DEFINES += APP_PATH=\\\"$$PWD\\\"
QMAKE_POST_LINK += mkdir -p $$BUILD_DIR
