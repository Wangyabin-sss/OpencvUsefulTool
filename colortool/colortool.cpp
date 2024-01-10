#include "colortool.h"
#include "ui_colortool.h"

colortool::colortool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::colortool)
{
    ui->setupUi(this);
}

colortool::~colortool()
{
    delete ui;
}


void colortool::show_frame_label(QImage img, int labelnum)
{
    if(labelnum == 1)
    {
        ui->label->setPixmap(QPixmap::fromImage(img).scaled(ui->label->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
        ui->label->setAlignment(Qt::AlignCenter);
        ui->label->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    }
    else if(labelnum == 2)
    {
        ui->label_2->setPixmap(QPixmap::fromImage(img).scaled(ui->label_2->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
        ui->label_2->setAlignment(Qt::AlignCenter);
        ui->label_2->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    }
}

static cv::Mat srcimage;
void colortool::on_openimage_clicked()
{
    std::vector<std::string> filelist = get_filename_dialog("*.jpg;*.bmp;*.png");
    if(filelist.size()>0)
    {
        srcimage = cv::imread(filelist[0], cv::IMREAD_UNCHANGED);
        Dbuginfo("read img type is  %s\n", get_imgtype_string(srcimage.depth(),srcimage.channels()).c_str());

        show_frame_label(Mat2QImage(srcimage),1);
        ui->label_2->clear();
    }
}


void colortool::on_pushButton_clicked()
{
    emit color_change2_mainwindow();
}

