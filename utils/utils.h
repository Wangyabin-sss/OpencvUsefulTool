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
    #include <clog/log.h>
}


QImage Mat2QImage(cv::Mat& cvImgsrc);
std::vector<std::string> get_filename_dialog(QString filetype);
std::string get_imgtype_string(int depth , int channel);

QString str2qstr(const std::string str);
std::string qstr2str(const QString qstr);

cv::Mat proc_baipinghengsuanfa(cv::Mat &imgrgb);
cv::Mat proc_erzhihua(cv::Mat &imggray, double thread);
cv::Mat proc_heibaifanzhuan(cv::Mat &imggray);
cv::Mat proc_zhifangtujunhenghua(cv::Mat &imggray);
cv::Mat proc_zhuanhuidutu(cv::Mat &imgbgr);
cv::Mat proc_zhuanBGRtu(cv::Mat &imggray);
cv::Mat proc_zidongyuzhifenge(cv::Mat &imgray);
std::vector<cv::Mat> his_BGRdetach_proc(cv::Mat &img);
std::vector<cv::Mat> his_gray_projection(cv::Mat &imggray);

#endif

