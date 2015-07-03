#-------------------------------------------------
#
# Project created by QtCreator 2015-06-29T10:17:51
#
#-------------------------------------------------

QT       -= gui

TARGET = ParticleFilterTracking
TEMPLATE = lib

SOURCES += \
    VehicleDetector.cpp \
    VehicleDetectorWidget.cpp \
    VehicleTracker.cpp \
    VehicleTrackingViewer.cpp \
    VehicleTrackingViewerWidget.cpp

HEADERS += \
    egotransform.h \
    particlefilterbase.h \
    particlefilterdef.h \
    randomgenerator.h \
    VehicleDetector.h \
    VehicleDetectorWidget.h \
    VehicleTracker.h \
    VehicleTrackingViewer.h \
    VehicleTrackingViewerWidget.h \
    VehicleParticleFilter.h \
    VehicleParticleFilter.cuh

DISTFILES += \
    egotransform.cu \
    VehicleParticleFilter.cu

MODULES += VirtualScan Velodyne Localization
include($$(ROBOTSDKMODULE))
include($$(ROBOTSDKCUDA))

unix {
    INCLUDEPATH += /usr/local/include/pcl-1.8
    LIBS += -L/usr/local/lib -lpcl_io
    LIBS += -L/usr/local/lib -lpcl_common
    LIBS += -L/usr/local/lib -lpcl_filters
    LIBS += -L/usr/local/lib -lpcl_search
    LIBS += -L/usr/local/lib -lpcl_kdtree
    LIBS += -L/usr/local/lib -lpcl_features
    LIBS += -L/usr/local/lib -lpcl_segmentation

    INCLUDEPATH += /usr/include
    LIBS += -L/usr/lib/x86_64-linux-gnu -lopencv_core
    LIBS += -L/usr/lib/x86_64-linux-gnu -lopencv_highgui
    LIBS += -L/usr/lib/x86_64-linux-gnu -lopencv_features2d
    LIBS += -L/usr/lib/x86_64-linux-gnu -lopencv_objdetect
    LIBS += -L/usr/lib/x86_64-linux-gnu -lopencv_contrib
    LIBS += -L/usr/lib/x86_64-linux-gnu -lopencv_imgproc
}

INCLUDEPATH += $$(HOME)/SDK/FastConvexFitting/include

CONFIG(debug, debug|release){
    LIBS += -L$$(HOME)/SDK/FastConvexFitting/lib -lFastConvexFitting_Debug
}
else{
    LIBS += -L$$(HOME)/SDK/FastConvexFitting/lib -lFastConvexFitting_Release
}
