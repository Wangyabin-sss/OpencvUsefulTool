#ifndef _OPENCVPP_H
#define _OPENCVPP_H

#include <iostream>
#include <fstream>
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
std::string get_funcode_from_file(std::string file, std::string funcname);
cv::Size get_size_from_str(std::string str);
cv::Scalar get_scalar_from_str(std::string str);

QString str2qstr(const std::string str);
std::string qstr2str(const QString qstr);

cv::Mat proc_baipinghengsuanfa(cv::Mat &imgrgb);
cv::Mat proc_erzhihua(cv::Mat &imggray, double thread, int num);
cv::Mat proc_heibaifanzhuan(cv::Mat &imggray);
cv::Mat proc_zhifangtujunhenghua(cv::Mat &imggray);
cv::Mat proc_zhuanhuidutu(cv::Mat &imgbgr);
cv::Mat proc_zhuanBGRtu(cv::Mat &imggray);
cv::Mat proc_zidongyuzhifenge(cv::Mat &imgray, int threshnum, int thresholdType, int blockSize, double C);
cv::Mat proc_fushi(cv::Mat &imggray, int shapetype, cv::Size coresize, int nums);
cv::Mat proc_pengzhang(cv::Mat &imggray, int shapetype, cv::Size coresize, int nums);
cv::Mat proc_chazhaolunkuo(cv::Mat &grayimg, int mode, int method, cv::Scalar color, int thickness);
cv::Mat proc_xuanzhuan(cv::Mat &img, float angle);

std::vector<cv::Mat> his_BGRdetach_proc(cv::Mat &img);
std::vector<cv::Mat> his_gray_projection(cv::Mat &imggray);
std::vector<cv::Mat> his_huiduzhifangtutongji(cv::Mat &imggray);

#endif

