QT += quick
CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += APP_PATH=\\\"$$PWD\\\"

message(DEFINES: $$DEFINES)

INCLUDEPATH += /usr/include/opencv4 \
               $$PWD/playvideo

LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio #opencv lib
LIBS += -lrt #real time lib

HEADERS += \
    $$PWD/playvideo/SharedMemory.h \
    ControllVideo.h \
    Videomodel.h

SOURCES += \
        $$PWD/playvideo/SharedMemory.cpp \
        ControllVideo.cpp \
        Videomodel.cpp \
        main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
