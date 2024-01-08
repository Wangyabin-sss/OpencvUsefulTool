#include <utils/utils.h>


//mat转QImage格式（直接调用即可）
QImage Mat2QImage(cv::Mat& cvImgsrc)
{
    cv::Mat cvImg = cvImgsrc.clone();
    QImage qImg;
    if(cvImg.channels()==3)                        //3 channels color image
    {

        cv::cvtColor(cvImg,cvImg,cv::COLOR_BGR2RGB);
        qImg =QImage((const unsigned char*)(cvImg.data),
                      cvImg.cols, cvImg.rows,
                      cvImg.cols*cvImg.channels(),
                      QImage::Format_RGB888);
    }
    else if(cvImg.channels()==1)                    //grayscale image
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                      cvImg.cols,cvImg.rows,
                      cvImg.cols*cvImg.channels(),
                      QImage::Format_Indexed8);
    }
    else
    {
        //std::cout<<"unknown channels"<<std::endl;
        qImg =QImage((const unsigned char*)(cvImg.data),
                      cvImg.cols,cvImg.rows,
                      cvImg.cols*cvImg.channels(),
                      QImage::Format_RGB888);
    }

    //深层拷贝
    qImg.bits();

    return qImg;

}

QString str2qstr(const std::string str)
{
    return QString::fromLocal8Bit(str.data());
}

std::string qstr2str(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return std::string(cdata);
}


std::vector<std::string> get_filename_dialog(QString filetype)
{
    //getOpenFileNames 同时选中多个文件
    QStringList filenames=QFileDialog::getOpenFileNames(NULL,"文件选择",".",filetype);

    std::vector<std::string> filelist;
    for(int i=0;i<filenames.size();i++)
    {
        filelist.push_back(std::string(filenames[i].toLocal8Bit()));
    }
    return filelist;
}


/*
#define CV_8U   0
#define CV_8S   1
#define CV_16U  2
#define CV_16S  3
#define CV_32S  4
#define CV_32F  5
#define CV_64F  6
#define CV_16F  7
*/
std::string get_imgtype_string(int depth , int channel)
{
    std::string typestr;
    switch (depth) {
    case 0:
        typestr.append("CV_8U");
        break;
    case 1:
        typestr.append("CV_8S");
        break;
    case 2:
        typestr.append("CV_16U");
        break;
    case 3:
        typestr.append("CV_16S");
        break;
    case 4:
        typestr.append("CV_32S");
        break;
    case 5:
        typestr.append("CV_32F");
        break;
    case 6:
        typestr.append("CV_64F");
        break;
    case 7:
        typestr.append("CV_16F");
        break;
    default:
        break;
    }

    typestr.append("C"+std::to_string(channel));
    return typestr;
}


//----------------------------------------------------weight-preprocessing-----------------------------------------------------------
cv::Mat proc_baipinghengsuanfa(cv::Mat &imgrgb)
{
    cv::Mat g_srcImage,dstImage;
    std::vector<cv::Mat> g_vChannels;
    g_srcImage = imgrgb.clone();
    //imshow("原图",g_srcImage);
    //waitKey(0);

    //分离通道
    cv::split(g_srcImage,g_vChannels);
    cv::Mat imageBlueChannel = g_vChannels.at(0);
    cv::Mat imageGreenChannel = g_vChannels.at(1);
    cv::Mat imageRedChannel = g_vChannels.at(2);

    double imageBlueChannelAvg=0;
    double imageGreenChannelAvg=0;
    double imageRedChannelAvg=0;

    //求各通道的平均值
    imageBlueChannelAvg = mean(imageBlueChannel)[0];
    imageGreenChannelAvg = mean(imageGreenChannel)[0];
    imageRedChannelAvg = mean(imageRedChannel)[0];

    //求出个通道所占增益
    double K = (imageRedChannelAvg+imageGreenChannelAvg+imageRedChannelAvg)/3;
    double Kb = K/imageBlueChannelAvg;
    double Kg = K/imageGreenChannelAvg;
    double Kr = K/imageRedChannelAvg;

    //更新白平衡后的各通道BGR值
    addWeighted(imageBlueChannel,Kb,0,0,0,imageBlueChannel);
    addWeighted(imageGreenChannel,Kg,0,0,0,imageGreenChannel);
    addWeighted(imageRedChannel,Kr,0,0,0,imageRedChannel);

    cv::merge(g_vChannels,dstImage);//图像各通道合并
    return dstImage;
}


cv::Mat proc_erzhihua(cv::Mat &imggray, double thread, int num)
{
    cv::Mat srcimg = imggray.clone();
    if(srcimg.channels()!=1)
        return srcimg;
    else
    {
        cv::Mat result;
        cv::threshold(srcimg, result, thread, 255, num);
        return result;
    }
}


cv::Mat proc_heibaifanzhuan(cv::Mat &imggray)
{
    cv::Mat srcimg = imggray.clone();
    if(srcimg.channels() == 1)
    {
        cv::Mat gray2(srcimg.size(),CV_8UC1);
        for(int i=0;i<gray2.rows;i++)    //a.rows行数    a.cols列数
        {
            for(int j=0;j<gray2.cols;j++)
                gray2.at<uchar>(i,j) = 255-srcimg.at<uchar>(i,j);
        }
        return gray2;
    }
    else
        return srcimg;
}


cv::Mat proc_zhifangtujunhenghua(cv::Mat &imggray)
{
    if(imggray.channels()!=1)
        return imggray;

    cv::Mat output = imggray.clone();

    int gray[256] = { 0 };  //记录每个灰度级别下的像素个数
    double gray_prob[256] = { 0 };  //记录灰度分布密度
    double gray_distribution[256] = { 0 };  //记录累计密度
    int gray_equal[256] = { 0 };  //均衡化后的灰度值
    int gray_sum = 0;  //像素总数

    gray_sum = imggray.cols * imggray.rows;

    //统计每个灰度下的像素个数
    for (int i = 0; i < imggray.rows; i++)
    {
        uchar* p = imggray.ptr<uchar>(i);
        for (int j = 0; j < imggray.cols; j++)
        {
            int vaule = p[j];
            gray[vaule]++;
        }
    }


    //统计灰度频率
    for (int i = 0; i < 256; i++)
    {
        gray_prob[i] = ((double)gray[i] / gray_sum);
    }

    //计算累计密度
    gray_distribution[0] = gray_prob[0];
    for (int i = 1; i < 256; i++)
    {
        gray_distribution[i] = gray_distribution[i-1] +gray_prob[i];
    }

    //重新计算均衡化后的灰度值，四舍五入。参考公式：(N-1)*T+0.5
    for (int i = 0; i < 256; i++)
    {
        gray_equal[i] = (uchar)(255 * gray_distribution[i] + 0.5);
    }


    //直方图均衡化,更新原图每个点的像素值
    for (int i = 0; i < output.rows; i++)
    {
        uchar* p = output.ptr<uchar>(i);
        for (int j = 0; j < output.cols; j++)
        {
            p[j] = gray_equal[p[j]];
        }
    }

    return output;
}

cv::Mat proc_zhuanhuidutu(cv::Mat &imgbgr)
{
    cv::Mat grayimg;
    cv::cvtColor(imgbgr, grayimg, cv::COLOR_BGR2GRAY);
    return grayimg;
}

cv::Mat proc_zhuanBGRtu(cv::Mat &imggray)
{
    cv::Mat bgrimg;
    cv::cvtColor(imggray, bgrimg, cv::COLOR_GRAY2BGR);
    return bgrimg;
}


cv::Mat proc_zidongyuzhifenge(cv::Mat &imgray)
{
    cv::Mat srcimg = imgray.clone();
    cv::Mat imgresult;
    cv::adaptiveThreshold(srcimg, imgresult, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 3);
    return imgresult;
}


//----------------------------------------------------weight-histogram-----------------------------------------------------------
std::vector<cv::Mat> his_BGRdetach_proc(cv::Mat &img)
{
    std::vector<cv::Mat> imgchannels,result;
    if(img.channels()<3)
    {
        return result;
    }

    cv::split(img,imgchannels);

    int bins = 256;
    int histsize[] = { bins };
    float range[] = { 0, 256 };
    const float* histRange = { range };
    cv::Mat  b_Hist, g_Hist, r_Hist;
    //-------------计算各个通道的直方图--------------------------------------
    cv::calcHist(&imgchannels[0], 1, 0, cv::Mat(), b_Hist, 1, histsize, &histRange, true, false); //B-通道
    cv::calcHist(&imgchannels[1], 1, 0, cv::Mat(), g_Hist, 1, histsize, &histRange, true, false); //G-通道
    cv::calcHist(&imgchannels[2], 1, 0, cv::Mat(), r_Hist, 1, histsize, &histRange, true, false); //R-通道

    //------------设置直方图绘图参数----------------------------------------------------
    int hist_h = 360;
    int hist_w = bins * 3;
    int bin_w = cvRound((double)hist_w / bins);
    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));//创建一个黑底的图像，为了可以显示彩色，所以该绘制图像是一个8位的3通道图像
    for(int i=0;i<3;i++)
        result.push_back(histImage.clone());
    //-------------将直方图归一化到[0,histImage.rows]  ------------------------------------------------------------------------------
    cv::normalize(b_Hist, b_Hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());  //B-通道
    cv::normalize(g_Hist, g_Hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());  //G-通道
    cv::normalize(r_Hist, r_Hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());  //R-通道
    //--------------绘制直方图  ----------------------------------------------------------------
    for (int i = 1; i < bins; i++)
    {
        //绘制B通道的直方图信息
        cv::line(result[0], cv::Point(bin_w*(i - 1), hist_h - cvRound(b_Hist.at<float>(i - 1))),
                 cv::Point(bin_w*(i), hist_h - cvRound(b_Hist.at<float>(i))), cv::Scalar(255, 0, 0), 2, 8, 0);
        //绘制G通道的直方图信息
        cv::line(result[1], cv::Point(bin_w*(i - 1), hist_h - cvRound(g_Hist.at<float>(i - 1))),
                 cv::Point(bin_w*(i), hist_h - cvRound(g_Hist.at<float>(i))), cv::Scalar(0, 255, 0), 2, 8, 0);
        //绘制R通道的直方图信息
        cv::line(result[2], cv::Point(bin_w*(i - 1), hist_h - cvRound(r_Hist.at<float>(i - 1))),
                 cv::Point(bin_w*(i), hist_h - cvRound(r_Hist.at<float>(i))), cv::Scalar(0, 0, 255), 2, 8, 0);
    }


    return result;
}


std::vector<cv::Mat> his_HSVdetach_proc(cv::Mat &srcimg)
{
    cv::Mat img = srcimg.clone();
    std::vector<cv::Mat> imgchannels,result;
    if(img.channels()<3)
    {
        return result;
    }

    cv::cvtColor(img,img,cv::COLOR_BGR2HSV);
    cv::split(img,imgchannels);

    int bins = 256;
    int histsize[] = { bins };
    float range[] = { 0, 256 };
    const float* histRange = { range };
    cv::Mat  b_Hist, g_Hist, r_Hist;
    //-------------计算各个通道的直方图--------------------------------------
    cv::calcHist(&imgchannels[0], 1, 0, cv::Mat(), b_Hist, 1, histsize, &histRange, true, false); //B-通道
    cv::calcHist(&imgchannels[1], 1, 0, cv::Mat(), g_Hist, 1, histsize, &histRange, true, false); //G-通道
    cv::calcHist(&imgchannels[2], 1, 0, cv::Mat(), r_Hist, 1, histsize, &histRange, true, false); //R-通道

    //------------设置直方图绘图参数----------------------------------------------------
    int hist_h = 360;
    int hist_w = bins * 3;
    int bin_w = cvRound((double)hist_w / bins);
    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));//创建一个黑底的图像，为了可以显示彩色，所以该绘制图像是一个8位的3通道图像
    for(int i=0;i<3;i++)
        result.push_back(histImage);
    //-------------将直方图归一化到[0,histImage.rows]  ------------------------------------------------------------------------------
    cv::normalize(b_Hist, b_Hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());  //B-通道
    cv::normalize(g_Hist, g_Hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());  //G-通道
    cv::normalize(r_Hist, r_Hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());  //R-通道
    //--------------绘制直方图  ----------------------------------------------------------------
    for (int i = 1; i < bins; i++)
    {
        //绘制B通道的直方图信息
        cv::line(result[0], cv::Point(bin_w*(i - 1), hist_h - cvRound(b_Hist.at<float>(i - 1))),
                 cv::Point(bin_w*(i), hist_h - cvRound(b_Hist.at<float>(i))), cv::Scalar(255, 0, 0), 2, 8, 0);
        //绘制G通道的直方图信息
        cv::line(result[1], cv::Point(bin_w*(i - 1), hist_h - cvRound(g_Hist.at<float>(i - 1))),
                 cv::Point(bin_w*(i), hist_h - cvRound(g_Hist.at<float>(i))), cv::Scalar(0, 255, 0), 2, 8, 0);
        //绘制R通道的直方图信息
        cv::line(result[2], cv::Point(bin_w*(i - 1), hist_h - cvRound(r_Hist.at<float>(i - 1))),
                 cv::Point(bin_w*(i), hist_h - cvRound(r_Hist.at<float>(i))), cv::Scalar(0, 0, 255), 2, 8, 0);
    }


    return result;
}



std::vector<cv::Mat> his_gray_projection(cv::Mat &imggray)
{
    int imgwidth    = imggray.cols;
    int imgheight   = imggray.rows;
    //int imgchannels = imggray.channels();
    std::vector<cv::Mat> result;
    std::vector<unsigned int> xpixall(imgheight),ypixall(imgwidth);
    if(imggray.channels()!=1)
        return result;

    for(int y=0,data;y<imgheight;y++)
    {
        for(int x=0;x<imgwidth;x++)
        {
            data= (int)imggray.data[y*imgwidth+x];
            xpixall[y] += data;
            ypixall[x] += data;
        }
    }

    unsigned int xmax=0,ymax=0;
    for(size_t i=0;i<xpixall.size();i++)
    {
        if(xmax<xpixall[i])
            xmax = xpixall[i];
    }
    for(size_t i=0;i<ypixall.size();i++)
    {
        if(ymax<ypixall[i])
            ymax = ypixall[i];
    }

    cv::Mat imgcreate = cv::Mat::zeros(imggray.size(),CV_8UC1);
    result.push_back(imgcreate.clone());
    result.push_back(imgcreate.clone());
    result.push_back(imgcreate.clone());
    for(int i=0;i<imgheight;i++)
    {
        cv::line(result[0],cv::Point((imgwidth-((float)xpixall[i]/xmax)*imgwidth),i),cv::Point(imgwidth,i),cv::Scalar(127));
    }
    for(int i=0;i<imgwidth;i++)
    {
        cv::line(result[1],cv::Point(i,(imgheight-(((float)ypixall[i]/ymax)*imgheight))),cv::Point(i,imgheight),cv::Scalar(127));
    }

    for(int y=0;y<imgheight;y++)
    {
        for(int x=0;x<imgwidth;x++)
        {
            if((uchar)result[0].data[y*imgwidth+x]==127&&(uchar)result[1].data[y*imgwidth+x]==127)
            {
                result[2].data[y*imgwidth+x] = 127;
            }
        }
    }

    return result;
}




