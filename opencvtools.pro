QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    colortool/colortool.cpp \
    histogram/histogramtool.cpp \
    morphology/morpho.cpp \
    preprocessing/preprocessing.cpp \
    utils/utils.cpp \
    clog/log.c \
    main.cpp \
    mainwindow.cpp \
    videoprocess/videoproc.cpp

HEADERS += \
    colortool/colortool.h \
    histogram/histogramtool.h \
    morphology/morpho.h \
    preprocessing/preprocessing.h \
    utils/utils.h \
    clog/log.h \
    mainwindow.h \
    videoprocess/videoproc.h

FORMS += \
    colortool/colortool.ui \
    histogram/histogramtool.ui \
    mainwindow.ui \
    morphology/morpho.ui \
    preprocessing/preprocessing.ui \
    videoprocess/videoproc.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}


INCLUDEPATH += E:\Downloads\library\opencv4.7.0\build\include \
               E:\Downloads\library\opencv4.7.0\build\include\opencv2

LIBS += E:\Downloads\library\opencv4.7.0\build\x64\vc16\lib\opencv_world470.lib

RESOURCES += \
    resource/resource.qrc


