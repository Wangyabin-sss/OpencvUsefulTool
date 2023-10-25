QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    colortool/colortool.cpp \
    histogram/histogramtool.cpp \
    opencvpp/opencvpp.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    colortool/colortool.h \
    histogram/histogramtool.h \
    opencvpp/opencvpp.h \
    mainwindow.h

FORMS += \
    colortool/colortool.ui \
    histogram/histogramtool.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



INCLUDEPATH += E:\Downloads\library\opencv4.7.0\build\include \
               E:\Downloads\library\opencv4.7.0\build\include\opencv2

LIBS += E:\Downloads\library\opencv4.7.0\build\x64\vc16\lib\opencv_world470.lib


