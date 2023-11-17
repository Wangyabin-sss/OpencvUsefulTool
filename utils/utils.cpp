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






