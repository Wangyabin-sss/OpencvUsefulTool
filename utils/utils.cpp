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


cv::Mat proc_erzhihua(cv::Mat &imggray, double thread)
{
    cv::Mat srcimg = imggray.clone();
    if(srcimg.channels()!=1)
        return srcimg;
    else
    {
        cv::Mat result;
        cv::threshold(srcimg, result, thread, 255, cv::THRESH_BINARY_INV);
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
