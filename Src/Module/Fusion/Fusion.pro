#-------------------------------------------------
#
# Project created by QtCreator 2015-04-27T14:44:06
#
#-------------------------------------------------

QT       -= gui

TARGET = Fusion
TEMPLATE = lib

SOURCES += \
    CameraVelodyneFusion.cpp \
    ImagePointCloudFusionViewer.cpp

HEADERS += \
    CameraVelodyneFusion.h \
    ImagePointCloudFusionViewer.h

MODULES += Camera Velodyne
include($$(HOME)/SDK/RobotSDK_4.0/Kernel/RobotSDK.pri)

unix{
    INCLUDEPATH += /usr/include/pcl-1.7

    LIBS += -L/usr/lib -lpcl_io
    LIBS += -L/usr/lib -lpcl_common
    LIBS += -L/usr/lib -lpcl_filters
    LIBS += -L/usr/lib -lpcl_search
    LIBS += -L/usr/lib -lpcl_kdtree
    LIBS += -L/usr/lib -lpcl_features
    LIBS += -L/usr/lib -lpcl_segmentation

    LIBS += -L/usr/lib/x86_64-linux-gnu -lopencv_core
    LIBS += -L/usr/lib/x86_64-linux-gnu -lopencv_highgui
    LIBS += -L/usr/lib/x86_64-linux-gnu -lopencv_features2d
    LIBS += -L/usr/lib/x86_64-linux-gnu -lopencv_objdetect
    LIBS += -L/usr/lib/x86_64-linux-gnu -lopencv_contrib
    LIBS += -L/usr/lib/x86_64-linux-gnu -lopencv_imgproc
}
