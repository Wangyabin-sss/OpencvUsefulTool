#include "histogramtool.h"
#include "ui_histogramtool.h"
#include <QMessageBox>


enum HIS_comflag{
    HIS_zhifangtuBGR=0,
    HIS_zhifangtuHSV,
    HIS_zhifangtutouyingGRAY,
    HIS_zhifangtutouyingB,
    HIS_zhifangtutouyingG,
    HIS_zhifangtutouyingR,
    HIS_zhifangtutouyingH,
    HIS_zhifangtutouyingS,
    HIS_zhifangtutouyingV,
    HIS_huiduzhifangtutongji
};


histogramtool::histogramtool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::histogramtool)
{
    ui->setupUi(this);

    qchartdata = new QChart();
    series = new QLineSeries;
//    // 构建折线系列对象
//    QLineSeries *series = new QLineSeries();
//    for (quint32 i = 0; i < 100; i++)
//    {
//        // 参数 x 为循环自增变量 i，参数 y 为正弦函数Y值
//        series->append(i, sin(static_cast<double>(0.6f*i)));
//    }

//    // 将系列添加到图表
//    qchartdata->addSeries(series);
//    // 基于已添加到图表的 series 来创建默认的坐标轴
//    qchartdata->createDefaultAxes();


    chartView = new QChartView();//QChartView 可以一步到位直接显示QChart
    chartView->setChart(qchartdata);
    chartView->setRenderHint(QPainter::Antialiasing);//继承来的抗锯齿方法
    //chartView->resize(ui->label_2->width(), ui->label_2->height());
    chartView->setParent(ui->label_2);


    ui->comboBox_2->addItem("直方图BGR");     //0
    ui->comboBox_2->addItem("直方图HSV");
    ui->comboBox_2->addItem("直方图投影GRAY");
    ui->comboBox_2->addItem("直方图投影B");
    ui->comboBox_2->addItem("直方图投影G");
    ui->comboBox_2->addItem("直方图投影R");
    ui->comboBox_2->addItem("直方图投影H");
    ui->comboBox_2->addItem("直方图投影S");
    ui->comboBox_2->addItem("直方图投影V");
    ui->comboBox_2->addItem("灰度直方图统计");
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
        ui->label->setPixmap(QPixmap::fromImage(img).scaled(ui->label->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
        ui->label->setAlignment(Qt::AlignCenter);
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


void histogramtool::update_qcharts_data(QLineSeries *ser)
{
    // 将系列添加到图表
    qchartdata->addSeries(ser);
    // 基于已添加到图表的 series 来创建默认的坐标轴
    qchartdata->createDefaultAxes();
    chartView->resize(ui->label_2->width(), ui->label_2->height());
    chartView->update();
}


void histogramtool::on_comboBox_2_currentIndexChanged(int index)
{
    if(srcimg.empty())
        return;

    series->clear();

//    std::cout<<ui->label->width()<<"  "<<ui->label->height()<<std::endl;
//    std::cout<<ui->label_2->width()<<"  "<<ui->label_2->height()<<std::endl;
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
    case HIS_zhifangtutouyingGRAY:
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
    case HIS_huiduzhifangtutongji:
        if(srcimg.channels()!=1)
        {
            QMessageBox::information(this,"wanning","仅支持灰度图");
            break;
        }
        imgchannels = his_huiduzhifangtutongji(srcimg);

        for(int i=0;i<imgchannels[0].rows;i++)
        {
            for(int j=0;j<imgchannels[0].cols;j++)
            {
                series->append(j,imgchannels[0].at<int>(i,j));
            }
        }
        this->update_qcharts_data(series);
        break;
    default:
        break;
    }
}

