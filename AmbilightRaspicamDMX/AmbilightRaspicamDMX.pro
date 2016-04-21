TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
INCLUDEPATH += /opt/vc/include \
    /opt/vc/include/interface/vcos \
    /opt/vc/include/interface/vcos/pthreads \
    /opt/vc/include/interface/vmcs_host/linux \
    /opt/vc/include/host_applications/linux/libs/bcm_host/include
LIBS += -L/opt/vc/lib -lmmal_core -lmmal_util -lmmal_vc_client -lvcos -lbcm_host -lGLESv2 -lEGL -lola -lolacommon

SOURCES += \
    camera.cpp \
    cameracontrol.cpp \
    graphics.cpp \
    picam.cpp \
    olawrapper.cpp \
    ambilight.cpp \
    controller.cpp \
    fader.cpp

HEADERS += \
    camera.h \
    cameracontrol.h \
    graphics.h \
    mmalincludes.h \
    olawrapper.h \
    ambilight.h \
    fader.h

