#include "dimensions.h"
#include "ui_dimensions.h"

Dimensions::Dimensions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Dimensions)
{
    ui->setupUi(this);
    ui->pointinfo->setText("5");
}

Dimensions::~Dimensions()
{
    delete ui;
}

void Dimensions::on_comeback_clicked()
{

    emit dimens_change2_mainwindow();
}

static cv::Mat srcimage,tmpimg;
void Dimensions::on_openimage_clicked()
{
    std::vector<std::string> filelist = get_filename_dialog("*.jpg;*.bmp;*.png");
    if(filelist.size()>0)
    {
        srcimage = cv::imread(filelist[0], cv::IMREAD_UNCHANGED);
        Dbuginfo("read img type is  %s\n", get_imgtype_string(srcimage.depth(),srcimage.channels()).c_str());
        show_img_label();
    }
}

float diff_point(cv::Point p1, cv::Point p2)
{
    int xd = pow(p2.x-p1.x,2);
    int yd = pow(p2.y-p1.y,2);
    return sqrt(xd+yd);
}

void Dimensions::show_img_label()
{
    QString difftext;
    if(srcimage.empty())
        return;
    tmpimg = srcimage.clone();
    for(size_t i=0;i<points.size();i++)
    {
        cv::circle(tmpimg,points[i],ui->pointinfo->text().toInt(),cv::Scalar(0,0,255),-1);
        for(int n=i+1;n<points.size();n++)
        {
            difftext.push_back(QString::number(i)+"->"+
                               QString::number(n)+" : "+
                               QString::number(diff_point(points[i],points[n]))+
                               " pix\r\n");
        }
    }
    ui->textEdit->setText(difftext);
    QImage img = Mat2QImage(tmpimg);
    ui->label->setPixmap(QPixmap::fromImage(img).scaled(ui->label->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
}

void Dimensions::on_addpoint_clicked()
{
    cv::Point point(0,0);
    points.push_back(point);
    ui->comboBox->addItem(QString::number(points.size()));
    show_img_label();
}


void Dimensions::on_xpointval_textChanged(const QString &arg1)
{
    if(ui->comboBox->currentIndex()>=0)
        points[ui->comboBox->currentIndex()].x = ui->xpointval->text().toInt();
    show_img_label();
}


void Dimensions::on_ypointval_textChanged(const QString &arg1)
{
    if(ui->comboBox->currentIndex()>=0)
        points[ui->comboBox->currentIndex()].y = ui->ypointval->text().toInt();
    show_img_label();
}

void Dimensions::wheelEvent(QWheelEvent *event)
{
    if(ui->xpointval->underMouse())
    {
        int val = ui->xpointval->text().toInt();
        if(event->angleDelta().y() > 0)
            ui->xpointval->setText(QString::number(val+1));
        else
            ui->xpointval->setText(QString::number(val-1));
    }
    else if(ui->ypointval->underMouse())
    {
        int val = ui->ypointval->text().toInt();
        if(event->angleDelta().y() > 0)
            ui->ypointval->setText(QString::number(val+1));
        else
            ui->ypointval->setText(QString::number(val-1));
    }
}


void Dimensions::on_comboBox_currentIndexChanged(int index)
{
    //std::cout<<index<<std::endl;
    ui->xpointval->setText(QString::number(points[index].x));
    ui->ypointval->setText(QString::number(points[index].y));
}

