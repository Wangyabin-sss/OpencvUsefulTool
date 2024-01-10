#include "videoproc.h"
#include "ui_videoproc.h"


videoproc::videoproc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::videoproc)
{
    ui->setupUi(this);

    cap = new cv::VideoCapture;
    mth.capthread = cap;

    QObject::connect(&mth,SIGNAL(show_frame_on_label(QImage,int)),this,SLOT(show_frame_label(QImage,int)));
}

videoproc::~videoproc()
{
    if(cap->isOpened())
        cap->release();
    delete ui;
}

std::vector<std::string> filelist;
void videoproc::on_openvideo_clicked()
{
    ui->comboBox->clear();
    filelist = get_filename_dialog("*.avi;*.mp4");
    Dbuginfo("get %d file\n", filelist.size());
    for(size_t i=0;i<filelist.size();i++)
    {
        replace(filelist[i].begin(), filelist[i].end(), '/', '\\');
        ui->comboBox->addItem(str2qstr(filelist[i]));
        //Dbuginfo("file name \"%s\"\n", filelist[i].c_str());
    }
}

volatile int vthreadrun = 0;   //控制视频线程起停
volatile int qsilder_status=0; //拖动进度条改变
volatile long framenow=0;      //当前帧
volatile int vthread_saveimg = 0;
static cv::Mat vframe;
volatile long vtotalFrameNumber;
volatile double vratefps=24.0;
#define FLAG_NULL   0
#define FLAG_BUTTEN 1
#define FLAG_SAVE_IMG 2
void videoproc::show_frame_label(QImage img, int flag)
{
    ui->label->setPixmap(QPixmap::fromImage(img).scaled(ui->label->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->label->setAlignment(Qt::AlignCenter);

    if(!qsilder_status)
        ui->horizontalSlider->setValue(((float)framenow/vtotalFrameNumber)*1000);

    if(flag == FLAG_NULL)
    {

    }
    else if(flag == FLAG_BUTTEN)
    {
        ui->pushButton_2->setText("start");
        vthreadrun = 0;
        framenow = 0;
    }
    else if(flag == FLAG_SAVE_IMG)
    {
        QDateTime dateTime= QDateTime::currentDateTime();//获取系统当前的时间
        QString datastr = dateTime.toString("yyyy-MM-ddThh-mm-ssF");//格式化时间
        QString filename(datastr+QString::number(framenow)+".bmp");
        img.save(filename,"BMP");
    }
}

void videoproc::on_pushButton_clicked()
{
    vthreadrun = 2;
    emit videopc_change2_mainwindow();
}


void videoproc::on_comboBox_currentIndexChanged(int index)
{
    if(index == -1)
        return;

    Dbuginfo("combox changed \"%s\"\n", filelist[index].c_str());
    vthread_saveimg = 0;
    framenow = 0;
    if(ui->pushButton_2->text() == "stop")
    {
        vthreadrun = 0;
        ui->pushButton_2->setText("start");
    }

    if(cap->isOpened())
        cap->release();

    cap->open(filelist[index]);
    if(cap->isOpened())
        Dbuginfo("open file success\n");
    else
        Dbuginfo("open file error\n");

    cap->read(vframe);
    show_frame_label(Mat2QImage(vframe),FLAG_NULL);

    //获取视频总帧数与帧率
    vtotalFrameNumber = cap->get(cv::CAP_PROP_FRAME_COUNT);
    vratefps = cap->get(cv::CAP_PROP_FPS);
    Dbuginfo("video frame nums %ld  ratefps  %.2f\n", vtotalFrameNumber, vratefps);
    //计算视频时长，填充进度条


}


void videoproc::on_screenshot_clicked()
{
    vthread_saveimg = 1;
}

void videoproc::on_screenshot2_clicked()
{
    if(ui->screenshot2->text() == "连续截图")
    {
        vthread_saveimg = 2;
        ui->screenshot2->setText("停止");
        ui->screenshot->setEnabled(false);
    }
    else if(ui->screenshot2->text() == "停止")
    {
        vthread_saveimg = 0;
        ui->screenshot2->setText("连续截图");
        ui->screenshot->setEnabled(true);
    }
}


void videoproc::on_horizontalSlider_actionTriggered(int action)
{
    //std::cout<<"action"<<std::endl;
}

void videoproc::on_horizontalSlider_valueChanged(int value)
{

}


void videoproc::on_horizontalSlider_sliderPressed()
{
    qsilder_status = 1;
}


void videoproc::on_horizontalSlider_sliderReleased()
{
    int value = ui->horizontalSlider->value();
    if(cap->isOpened())
    {
        framenow = ((float)value/1000.0)*vtotalFrameNumber;
        Dbuginfo("change framenow %ld\n", framenow);
    }
    qsilder_status = 0;
}


void videoproc::on_pushButton_2_clicked()
{
    if(ui->pushButton_2->text() == "start")
    {
        vthreadrun = 1;
        ui->pushButton_2->setText("stop");
    }
    else if(ui->pushButton_2->text() == "stop")
    {
        vthreadrun = 0;
        ui->pushButton_2->setText("start");
    }
}


void MyThread::run()
{
    vthreadrun = 0;

    while(1)
    {
        if(capthread->isOpened()&&vthreadrun == 1)
        {
            capthread->set(cv::CAP_PROP_POS_FRAMES, framenow++);
            if(framenow == vtotalFrameNumber)
            {
                emit show_frame_on_label(Mat2QImage(vframe),FLAG_BUTTEN);
            }
            capthread->get(cv::CAP_PROP_POS_FRAMES);
            *capthread >> vframe;

            if(vframe.empty())
            {
                Dbugerror("qthread capvideo read frame error framenow = %ld\n",framenow);
                continue;
            }

            if(vthread_saveimg == 1)
            {
                emit show_frame_on_label(Mat2QImage(vframe),FLAG_SAVE_IMG);
                vthread_saveimg = 0;
            }
            else if(vthread_saveimg == 2)
            {
                emit show_frame_on_label(Mat2QImage(vframe),FLAG_SAVE_IMG);
            }
            else if(vthread_saveimg == 0)
            {
                emit show_frame_on_label(Mat2QImage(vframe),FLAG_NULL);
            }

        }
        if(vthreadrun == 2)
            break;

        QThread::msleep(1000/vratefps);
    }

}








