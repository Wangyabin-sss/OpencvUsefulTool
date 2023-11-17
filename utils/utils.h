#ifndef _OPENCVPP_H
#define _OPENCVPP_H

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/dnn.hpp>
#include <QImage>
#include <QFileDialog>
#include <QWidget>
#include <QDebug>

extern "C"
{
    #include "clog/log.h"
}


QImage Mat2QImage(cv::Mat& cvImgsrc);
std::vector<std::string> get_filename_dialog(QString filetype);

QString str2qstr(const std::string str);
std::string qstr2str(const QString qstr);

#endif

