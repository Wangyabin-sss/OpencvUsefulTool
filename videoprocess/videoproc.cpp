#include "videoproc.h"
#include "ui_videoproc.h"


videoproc::videoproc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::videoproc)
{
    ui->setupUi(this);
}

videoproc::~videoproc()
{
    if(cap.isOpened())
        cap.release();
    delete ui;
}

void videoproc::on_pushButton_clicked()
{
    emit videopc_change2_mainwindow();
}

std::vector<std::string> filelist;
void videoproc::on_openvideo_clicked()
{
    filelist = get_filename_dialog("*.avi");
    Dbuginfo("get %d file\n", filelist.size());
    for(size_t i=0;i<filelist.size();i++)
    {
        replace(filelist[i].begin(), filelist[i].end(), '/', '\\');
        ui->comboBox->addItem(str2qstr(filelist[i]));
        //Dbuginfo("file name \"%s\"\n", filelist[i].c_str());
    }
}


void videoproc::show_frame_label(QImage img)
{
    ui->label->setPixmap(QPixmap::fromImage(img).scaled(ui->label->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->label->setAlignment(Qt::AlignCenter);
}

cv::Mat frame;
void videoproc::on_comboBox_currentIndexChanged(int index)
{
    Dbuginfo("combox changed \"%s\"\n", filelist[index].c_str());

    if(cap.isOpened())
        cap.release();

    cap.open(filelist[index]);
    if(cap.isOpened())
        Dbuginfo("open file success\n");
    else
        Dbuginfo("open file error\n");

    cap.read(frame);
    show_frame_label(Mat2QImage(frame));

    //获取视频总帧数与帧率
    long totalFrameNumber = cap.get(cv::CAP_PROP_FRAME_COUNT);
    double ratefps = cap.get(cv::CAP_PROP_FPS);
    Dbuginfo("video frame nums %ld  ratefps  %.2f\n", totalFrameNumber, ratefps);
    //计算视频时长，填充进度条


}


void videoproc::on_screenshot_clicked()
{

}


void videoproc::on_screenshot2_clicked()
{

}


void videoproc::on_horizontalSlider_actionTriggered(int action)
{
    std::cout<<"dwqadqwd"<<std::endl;
}

