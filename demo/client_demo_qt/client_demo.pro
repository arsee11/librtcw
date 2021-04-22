QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia multimediawidgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += WEBRTC_POSIX WEBRTC_LINUX WEBRTC_USE_EPOLL WEBRTC_EXTERNAL_JSON

INCLUDEPATH += /home/arsee/libs/webrtc
INCLUDEPATH += /home/arsee/libs/webrtc/third_party/libyuv/include/
INCLUDEPATH += ../../
INCLUDEPATH += /home/arsee/gits

LIBS += -L../../lib

LIBS += -lrtcw
LIBS += -lwebrtc_deps
LIBS += -labsl_strings
LIBS += -labsl_bad_optional_access
LIBS += -labsl_throw_delegate
LIBS += -labsl_bad_variant_access
LIBS += -lpthread
LIBS += -lboost_json
#LIBS += -levent
LIBS += -lssl
LIBS += -lsrtp2

SOURCES += \
    dialog.cpp \
    main.cpp \
    mainwindow.cpp \
    peersinfo.cpp \
    video_capture.cpp \
    videorender.cpp

HEADERS += \
    dialog.h \
    mainwindow.h \
    peersinfo.h \
    video_capture.h \
    videorender.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
