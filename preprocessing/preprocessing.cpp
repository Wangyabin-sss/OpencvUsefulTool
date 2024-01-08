#include "preprocessing.h"
#include "ui_preprocessing.h"

preprocessing::preprocessing(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::preprocessing)
{
    ui->setupUi(this);

    funcwight = new QHBoxLayout();
    ui->verticalLayout_2->addLayout(funcwight);

    imgsub = 0;
    ui->comboBox->addItem("白平衡算法");   //0
    ui->comboBox->addItem("二值化");      //1
    ui->comboBox->addItem("黑白翻转");    //2
    ui->comboBox->addItem("中值滤波");    //3
    ui->comboBox->addItem("均值滤波");    //4
    ui->comboBox->addItem("高斯滤波");    //5
    ui->comboBox->addItem("直方图均衡化"); //6
    ui->comboBox->addItem("查找轮廓");    //7
    ui->comboBox->addItem("对比度调整");  //8
    ui->comboBox->addItem("亮度调整");    //9
    ui->comboBox->addItem("边缘锐化");    //10
    ui->comboBox->addItem("转灰度图");    //11
    ui->comboBox->addItem("转BGR");      //12
    ui->comboBox->addItem("自动阈值分割"); //13
}


preprocessing::~preprocessing()
{
    delete ui;
}


void preprocessing::actionsSlot()
{
    QAction *action = (QAction*)sender();
    QString actionText = action->text();

    Dbuginfo("qtoolbutten %s\n",actionText.toLocal8Bit().data());

    if("1_1" == actionText) {

    } else if("1_2" == actionText) {

    }
}

void preprocessing::on_comeback_clicked()
{
    ui->label->clear();
    ui->label_2->clear();
    emit preprocess_change2_mainwindow();
}

cv::Mat srcimage;
std::vector<cv::Mat> procimgs(MAX_RESETIMGNUMS,cv::Mat());
void preprocessing::on_openimg_clicked()
{
    std::vector<std::string> filelist = get_filename_dialog("*.jpg;*.bmp;*.png");
    if(filelist.size()>0)
    {
        procimgs.clear();
        imgsub = 0;
        srcimage = cv::imread(filelist[0], cv::IMREAD_UNCHANGED);
        Dbuginfo("read img type is  %s\n", get_imgtype_string(srcimage.depth(),srcimage.channels()).c_str());
        for(int i=0;i<MAX_RESETIMGNUMS;i++)
        {
            procimgs.push_back(srcimage);
        }
        show_frame_label(Mat2QImage(srcimage),1);
        //show_frame_label(Mat2QImage(srcimage),2);
        ui->label_2->clear();
    }
}


int preprocessing::loop_add_one(int num)
{
    if(num==MAX_RESETIMGNUMS-1)
        return 0;
    else
        return num + 1;
}
int preprocessing::loop_sub_one(int num)
{
    if(num==0)
        return MAX_RESETIMGNUMS-1;
    else
        return num - 1;
}


void preprocessing::show_frame_label(QImage img, int labelnum)
{
    if(labelnum == 1)
    {
        ui->label->setPixmap(QPixmap::fromImage(img).scaled(ui->label->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
        ui->label->setAlignment(Qt::AlignCenter);
    }
    else if(labelnum == 2)
    {
        ui->label_2->setPixmap(QPixmap::fromImage(img).scaled(ui->label_2->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
        ui->label_2->setAlignment(Qt::AlignCenter);
    }
}


void preprocessing::on_saveimage_clicked()
{
    QDateTime dateTime= QDateTime::currentDateTime();//获取系统当前的时间
    QString datastr = dateTime.toString("yyyy-MM-ddThh-mm-ssM");//格式化时间
    QString filename(datastr+QString::number(imgsub)+".bmp");
    if(procimgs[imgsub].empty())
    {
        QMessageBox::information(this,"error","保存失败\n图片为空");
        return;
    }
    cv::imwrite(std::string(filename.toStdString()), procimgs[imgsub]);
}


void preprocessing::on_processimg_clicked()
{
    //检查结果是否需要叠加
    if(ui->checkBox->isChecked())
        srcimage = procimgs[imgsub].clone();
    cv::Mat procimg;
    switch (ui->comboBox->currentIndex()) {
    case 0:
        if(srcimage.channels()==1)
        {
            QMessageBox::information(this,"wanning","仅支持BGR图");
            break;
        }
        procimg = proc_baipinghengsuanfa(srcimage);
        show_frame_label(Mat2QImage(procimg),2);
        show_frame_label(Mat2QImage(srcimage),1);
        imgsub = loop_add_one(imgsub);
        procimgs[imgsub] = procimg.clone();
        break;
    case 1:
        if(!funcline1->text().isEmpty())
        {
            if(srcimage.channels()!=1)
            {
                QMessageBox::information(this,"wanning","仅支持灰度图");
                break;
            }
            procimg = proc_erzhihua(srcimage, funcline1->text().toDouble(), funccombox1->currentIndex());
            show_frame_label(Mat2QImage(procimg),2);
            show_frame_label(Mat2QImage(srcimage),1);
            imgsub = loop_add_one(imgsub);
            procimgs[imgsub] = procimg.clone();
        }
        else
        {
            QMessageBox::information(this, "wanning", "缺少参数");
        }
        break;
    case 2:
        if(srcimage.channels()!=1)
        {
            QMessageBox::information(this,"wanning","仅支持灰度图");
            break;
//            cv::cvtColor(srcimage,grayimg,cv::COLOR_BGR2GRAY);
//            cv::imshow("tmp", grayimg);
        }
        procimg = proc_heibaifanzhuan(srcimage);
        show_frame_label(Mat2QImage(procimg),2);
        show_frame_label(Mat2QImage(srcimage),1);
        imgsub = loop_add_one(imgsub);
        procimgs[imgsub] = procimg.clone();
        break;
    case 3:

        break;
    case 4:

        break;
    case 5:

        break;
    case 6:
        if(srcimage.channels()!=1)
        {
            QMessageBox::information(this,"wanning","仅支持灰度图");
            break;
        }
        procimg = proc_zhifangtujunhenghua(srcimage);
        show_frame_label(Mat2QImage(procimg),2);
        show_frame_label(Mat2QImage(srcimage),1);
        imgsub = loop_add_one(imgsub);
        procimgs[imgsub] = procimg.clone();
        break;
    case 7:

        break;
    case 8:

        break;
    case 9:

        break;
    case 10:

        break;
    case 11:
        if(srcimage.channels()==1)
        {
            QMessageBox::information(this,"wanning","仅支持BGR图");
            break;
        }
        procimg = proc_zhuanhuidutu(srcimage);
        show_frame_label(Mat2QImage(procimg),2);
        show_frame_label(Mat2QImage(srcimage),1);
        imgsub = loop_add_one(imgsub);
        procimgs[imgsub] = procimg.clone();
        break;
    case 12:
        if(srcimage.channels()>=3)
        {
            QMessageBox::information(this,"wanning","仅支持灰度图");
            break;
        }
        procimg = proc_zhuanBGRtu(srcimage);
        show_frame_label(Mat2QImage(procimg),2);
        show_frame_label(Mat2QImage(srcimage),1);
        imgsub = loop_add_one(imgsub);
        procimgs[imgsub] = procimg.clone();
        break;
    case 13:
        if(srcimage.channels()>=3)
        {
            QMessageBox::information(this,"wanning","仅支持灰度图");
            break;
        }
        procimg = proc_zidongyuzhifenge(srcimage);
        show_frame_label(Mat2QImage(procimg),2);
        show_frame_label(Mat2QImage(srcimage),1);
        imgsub = loop_add_one(imgsub);
        procimgs[imgsub] = procimg.clone();
        break;
    default:
        break;
    }

//    for(int i=0;i<MAX_RESETIMGNUMS;i++)
//    {
//        cv::imshow(std::string(std::to_string(i)+"dug"),procimgs[i]);
//    }

}


void preprocessing::on_chehui_clicked()
{
    if(ui->checkBox->isChecked())
    {
        imgsub = loop_sub_one(imgsub);
        show_frame_label(Mat2QImage(procimgs[imgsub]),2);
        show_frame_label(Mat2QImage(procimgs[loop_sub_one(imgsub)]),1);
    }
}


void preprocessing::on_comboBox_currentIndexChanged(int index)
{

    //删除所有子控件
    while (funcwight->count() != 0)
    {
        QLayoutItem* item = funcwight->takeAt(0);
        if (item)
        {
            delete item->widget();
            delete item;
        }
    }


    switch (index) {
    case 0:
        funclabel1 = new QLabel;
        funclabel1->setText("无需设置");
        funcwight->addWidget(funclabel1);
        ui->verticalLayout_2->update();
        break;
    case 1:
        funclabel1 = new QLabel;
        funclabel2 = new QLabel;
        funclabel3 = new QLabel;
        funcline1 = new QLineEdit;
        funccombox1 = new QComboBox;
        funclabel1->setText("cv::threshold(srcimg, result, ");
        funclabel2->setText(", 255, ");
        funclabel3->setText(");");
        funcline1->setText("100");
        funccombox1->addItem("cv::THRESH_BINARY");
        funccombox1->addItem("cv::THRESH_BINARY_INV");
        funcwight->addWidget(funclabel1);
        funcwight->addWidget(funcline1);
        funcwight->addWidget(funclabel2);
        funcwight->addWidget(funccombox1);
        funcwight->addWidget(funclabel3);
        funcwight->addStretch(); //添加弹簧
        ui->verticalLayout_2->update();
        break;
    case 2:
        funclabel1 = new QLabel;
        funclabel1->setText("无需设置");
        funcwight->addWidget(funclabel1);
        ui->verticalLayout_2->update();
        break;
    case 3:
        funclabel1 = new QLabel;
        funclabel1->setText("无需设置");
        funcwight->addWidget(funclabel1);
        ui->verticalLayout_2->update();
        break;
    case 4:
        funclabel1 = new QLabel;
        funclabel1->setText("无需设置");
        funcwight->addWidget(funclabel1);
        ui->verticalLayout_2->update();
        break;
    case 5:
        funclabel1 = new QLabel;
        funclabel1->setText("无需设置");
        funcwight->addWidget(funclabel1);
        ui->verticalLayout_2->update();
        break;
    case 6:
        funclabel1 = new QLabel;
        funclabel1->setText("无需设置");
        funcwight->addWidget(funclabel1);
        ui->verticalLayout_2->update();
        break;
    case 7:
        funclabel1 = new QLabel;
        funclabel1->setText("无需设置");
        funcwight->addWidget(funclabel1);
        ui->verticalLayout_2->update();
        break;
    case 8:
        funclabel1 = new QLabel;
        funclabel1->setText("无需设置");
        funcwight->addWidget(funclabel1);
        ui->verticalLayout_2->update();
        break;
    case 9:
        funclabel1 = new QLabel;
        funclabel1->setText("无需设置");
        funcwight->addWidget(funclabel1);
        ui->verticalLayout_2->update();
        break;
    case 10:
        funclabel1 = new QLabel;
        funclabel1->setText("无需设置");
        funcwight->addWidget(funclabel1);
        ui->verticalLayout_2->update();
        break;
    case 11:
        funclabel1 = new QLabel;
        funclabel1->setText("无需设置");
        funcwight->addWidget(funclabel1);
        ui->verticalLayout_2->update();
        break;
    case 12:
        funclabel1 = new QLabel;
        funclabel1->setText("无需设置");
        funcwight->addWidget(funclabel1);
        ui->verticalLayout_2->update();
        break;
    case 13:
        funclabel1 = new QLabel;
        funclabel1->setText("无需设置");
        funcwight->addWidget(funclabel1);
        ui->verticalLayout_2->update();
        break;
    default:
        break;
    }
}





