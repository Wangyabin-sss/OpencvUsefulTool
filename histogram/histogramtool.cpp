#include "histogramtool.h"
#include "ui_histogramtool.h"
#include <QMessageBox>

enum HIS_comflag{
    HIS_zhifangtuBGR,
    HIS_zhifangtuHSV,
    HIS_zhifangtuGRAY,
    HIS_zhifangtutouyingGRAY,
    HIS_zhifangtutouyingB,
    HIS_zhifangtutouyingG,
    HIS_zhifangtutouyingR,
    HIS_zhifangtutouyingH,
    HIS_zhifangtutouyingS,
    HIS_zhifangtutouyingV
};


histogramtool::histogramtool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::histogramtool)
{
    ui->setupUi(this);

    ui->comboBox_2->addItem("直方图BGR");     //0
    ui->comboBox_2->addItem("直方图HSV");
    ui->comboBox_2->addItem("直方图GRAY");
    ui->comboBox_2->addItem("直方图投影GRAY");
    ui->comboBox_2->addItem("直方图投影B");
    ui->comboBox_2->addItem("直方图投影G");
    ui->comboBox_2->addItem("直方图投影R");
    ui->comboBox_2->addItem("直方图投影H");
    ui->comboBox_2->addItem("直方图投影S");
    ui->comboBox_2->addItem("直方图投影V");
}

histogramtool::~histogramtool()
{
    delete ui;
}

void histogramtool::on_pushButton_clicked()
{
    emit histogram_change2_mainwindow();
}


void histogramtool::on_pushButton_2_clicked()
{
    ui->comboBox->clear();
    filelist = get_filename_dialog("*.jpg;*.bmp;*.png");
    Dbuginfo("get %d file\n", filelist.size());
    for(size_t i=0;i<filelist.size();i++)
    {
        replace(filelist[i].begin(), filelist[i].end(), '/', '\\');
        ui->comboBox->addItem(str2qstr(filelist[i]));
        //Dbuginfo("file name \"%s\"\n", filelist[i].c_str());
    }
}

void histogramtool::show_frame_label(QImage img, int labelnum)
{
    if(labelnum == 1)
    {
        ui->label_1->setPixmap(QPixmap::fromImage(img).scaled(ui->label_1->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
        ui->label_1->setAlignment(Qt::AlignCenter);
    }
    else if(labelnum == 2)
    {
        ui->label_2->setPixmap(QPixmap::fromImage(img).scaled(ui->label_2->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
        ui->label_2->setAlignment(Qt::AlignCenter);
    }
    else if(labelnum == 3)
    {
        ui->label_3->setPixmap(QPixmap::fromImage(img).scaled(ui->label_3->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
        ui->label_3->setAlignment(Qt::AlignCenter);
    }
    else if(labelnum == 4)
    {
        ui->label_4->setPixmap(QPixmap::fromImage(img).scaled(ui->label_4->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
        ui->label_4->setAlignment(Qt::AlignCenter);
    }
}


static cv::Mat srcimg;
void histogramtool::on_comboBox_currentIndexChanged(int index)
{
    if(index == -1)
        return;
    Dbuginfo("combox changed index %d filename \"%s\"\n",index, filelist[index].c_str());
    srcimg = cv::imread(filelist[index], cv::IMREAD_UNCHANGED);
    Dbuginfo("read img type is  %s\n", get_imgtype_string(srcimg.depth(),srcimg.channels()).c_str());
    show_frame_label(Mat2QImage(srcimg),1);
}


void histogramtool::on_comboBox_2_currentIndexChanged(int index)
{
    if(srcimg.empty())
        return;

    std::vector<cv::Mat> imgchannels;
    switch (index) {
    case HIS_zhifangtuBGR:
        if(srcimg.channels()!=3)
        {
            QMessageBox::information(this,"wanning","仅支持RGB图");
            break;
        }
        imgchannels = his_BGRdetach_proc(srcimg);
        for(size_t i=0;i<imgchannels.size();i++)
        {
            show_frame_label(Mat2QImage(imgchannels[i]),i+2);
        }
        break;
    case HIS_zhifangtuGRAY:
        if(srcimg.channels()!=1)
        {
            QMessageBox::information(this,"wanning","仅支持灰度图");
            break;
        }
        imgchannels = his_gray_projection(srcimg);
        for(size_t i=0;i<imgchannels.size();i++)
        {
            show_frame_label(Mat2QImage(imgchannels[i]),i+2);
        }
        break;
    default:
        break;
    }
}

