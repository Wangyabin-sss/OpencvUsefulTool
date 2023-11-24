#include "histogramtool.h"
#include "ui_histogramtool.h"

histogramtool::histogramtool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::histogramtool)
{
    ui->setupUi(this);

    ui->comboBox_2->addItem("直方图BGR");     //0
    ui->comboBox_2->addItem("直方图HSV");     //1
    ui->comboBox_2->addItem("直方图投影GRAY"); //2
    ui->comboBox_2->addItem("直方图投影BIN");  //3
    ui->comboBox_2->addItem("直方图投影B");   //4
    ui->comboBox_2->addItem("直方图投影G");   //5
    ui->comboBox_2->addItem("直方图投影R");   //6
    ui->comboBox_2->addItem("直方图投影H");   //7
    ui->comboBox_2->addItem("直方图投影S");   //8
    ui->comboBox_2->addItem("直方图投影V");   //9
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


cv::Mat srcimg;
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
    case 0:
        imgchannels = his_BGRdetach_proc(srcimg);
        for(size_t i=0;i<imgchannels.size();i++)
        {
            show_frame_label(Mat2QImage(imgchannels[i]),i+2);
        }
        break;
    case 1:

        break;
    case 2:
        imgchannels = his_gray_projection(srcimg);
        for(size_t i=0;i<imgchannels.size();i++)
        {
            show_frame_label(Mat2QImage(imgchannels[i]),i+2);
        }
        break;
    case 3:

        break;
    default:
        break;
    }
}

