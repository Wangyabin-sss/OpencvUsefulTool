#include "preprocessing.h"
#include "ui_preprocessing.h"
#include <QListView>


enum ComBOXfuncType {
    Func_zhuanhuidutu=0,
    Func_zhuanBGR,
    Func_baipinghengsuanfa,
    Func_erzhihua,
    Func_heibaifanzhuan,
    Func_zhongzhilvbo,
    Func_junzhilvbo,
    Func_gaosilvbo,
    Func_zhifangtujunhenghua,
    Func_chazhaolunkuo,
    Func_bianyuanruihua,
    Func_zidongyuzhifenge,
    Func_fushi,
    Func_pengzhang,
    Func_xuanzhuan
};


preprocessing::preprocessing(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::preprocessing)
{
    ui->setupUi(this);

    //添加函数显示水平布局
    funcwight = new QHBoxLayout();
    ui->verticalLayout_2->addLayout(funcwight);

    //设置文本框不自动换行
    ui->textEdit->setLineWrapMode(QTextEdit::NoWrap);
    //设置分割线参数
    ui->splitter->setStretchFactor(0,3);
    ui->splitter->setStretchFactor(0,1);
    QString style = QString("QSplitter::handle { background-color: rgb(179, 179, 179); }"); //分割线的颜色
    ui->splitter->setStyleSheet(style);
    ui->splitter->setHandleWidth(2);//分割线的宽度

    imgsub = 0;
    //注意与enum ComBOXfuncType对应
    ui->comboBox->addItem("转GRAY");
    ui->comboBox->addItem("转BGR");
    ui->comboBox->addItem("白平衡算法");
    ui->comboBox->addItem("二值化");
    ui->comboBox->addItem("黑白翻转");
    ui->comboBox->addItem("中值滤波");
    ui->comboBox->addItem("均值滤波");
    ui->comboBox->addItem("高斯滤波");
    ui->comboBox->addItem("直方图均衡化");
    ui->comboBox->addItem("查找轮廓");
    ui->comboBox->addItem("边缘锐化");
    ui->comboBox->addItem("自动阈值分割");
    ui->comboBox->addItem("腐蚀");
    ui->comboBox->addItem("膨胀");
    ui->comboBox->addItem("旋转");

    ui->comboBox->setStyleSheet("QComboBox QAbstractItemView::item {min-height: 30px; min-width:40px;}");
    //需要设置QListView才能设置行高
    QListView* view = new QListView(this);
    ui->comboBox->setView(view);

    QFontMetrics fm(this->font());
    fontsizechar = fm.boundingRect("a");
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

static cv::Mat srcimage;
std::vector<cv::Mat> procimgs(MAX_RESETIMGNUMS,cv::Mat());
void preprocessing::on_openimg_clicked()
{
    std::vector<std::string> filelist = get_filename_dialog("*.jpg;*.bmp;*.png");
    if(filelist.size()>0)
    {
        procimgs.clear();
        imgsub = 0;
        srcimage = cv::imread(filelist[0], cv::IMREAD_ANYCOLOR);
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
        ui->label->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);

    }
    else if(labelnum == 2)
    {
        ui->label_2->setPixmap(QPixmap::fromImage(img).scaled(ui->label_2->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
        ui->label_2->setAlignment(Qt::AlignCenter);
        ui->label_2->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);

    }
}


void preprocessing::on_saveimage_clicked()
{
    QDateTime dateTime= QDateTime::currentDateTime();//获取系统当前的时间
    QString datastr = dateTime.toString("yyyy-MM-ddThh-mm-ssM");//格式化时间
    QString filename(datastr+QString::number(imgsub)+".png");
    if(procimgs[imgsub].empty())
    {
        QMessageBox::information(this,"error","保存失败\n图片为空");
        return;
    }
    cv::imwrite(std::string(filename.toStdString()), procimgs[imgsub]);
}

std::string pathutilsfile = "../utils/utils.cpp";
void preprocessing::on_processimg_clicked()
{
    //检查结果是否需要叠加
    if(ui->checkBox->isChecked())
        srcimage = procimgs[imgsub].clone();
    if(srcimage.empty())
        return;
    cv::Mat procimg;
    switch (ui->comboBox->currentIndex()) {
    case Func_baipinghengsuanfa:
        if(srcimage.channels()==1)
        {
            QMessageBox::information(this,"wanning","仅支持BGR图");
            break;
        }

        procimg = proc_baipinghengsuanfa(srcimage);
        break;
    case Func_erzhihua:
        if(!funcline1->text().isEmpty())
        {
            if(srcimage.channels()!=1)
            {
                QMessageBox::information(this,"wanning","仅支持灰度图");
                break;
            }

            procimg = proc_erzhihua(srcimage, funcline1->text().toDouble(), funccombox1->currentIndex());
        }
        else
        {
            QMessageBox::information(this, "wanning", "缺少参数");
        }
        break;
    case Func_heibaifanzhuan:
        if(srcimage.channels()!=1)
        {
            QMessageBox::information(this,"wanning","仅支持灰度图");
            break;
        }

        procimg = proc_heibaifanzhuan(srcimage);
        break;
    case Func_zhongzhilvbo:

        break;
    case Func_junzhilvbo:

        break;
    case Func_gaosilvbo:

        break;
    case Func_zhifangtujunhenghua:
        if(srcimage.channels()!=1)
        {
            QMessageBox::information(this,"wanning","仅支持灰度图");
            break;
        }

        procimg = proc_zhifangtujunhenghua(srcimage);
        break;
    case Func_chazhaolunkuo:
        if(srcimage.channels()!=1)
        {
            QMessageBox::information(this,"wanning","仅支持灰度图");
            break;
        }

        procimg = proc_chazhaolunkuo(srcimage,
                                    funccombox1->currentIndex(),
                                    funccombox2->currentIndex()+1,
                                    get_scalar_from_str(funcline1->text().toStdString()),
                                    funcline2->text().toInt());
        break;
    case Func_bianyuanruihua:

        break;
    case Func_zhuanhuidutu:
        if(srcimage.channels()==1)
        {
            QMessageBox::information(this,"wanning","仅支持BGR图");
            break;
        }

        procimg = proc_zhuanhuidutu(srcimage);
        break;
    case Func_zhuanBGR:
        if(srcimage.channels()>=3)
        {
            QMessageBox::information(this,"wanning","仅支持灰度图");
            break;
        }

        procimg = proc_zhuanBGRtu(srcimage);
        break;
    case Func_zidongyuzhifenge:
        if(srcimage.channels()>=3)
        {
            QMessageBox::information(this,"wanning","仅支持灰度图");
            break;
        }

        if(funcline1->text().toInt()%2==0||funcline1->text().toInt()<=1)
        {
            QMessageBox::information(this,"wanning","Assertion failed (blockSize % 2 == 1 && blockSize > 1)");
            break;
        }
        procimg = proc_zidongyuzhifenge(srcimage,
                                        funccombox1->currentIndex(),
                                        funccombox2->currentIndex(),
                                        funcline1->text().toInt(),
                                        funcline2->text().toDouble());
        break;
    case Func_fushi:
        if(srcimage.channels()>=3)
        {
            QMessageBox::information(this,"wanning","仅支持灰度图");
            break;
        }

        procimg = proc_fushi(srcimage,funccombox1->currentIndex(),
                             get_size_from_str(funcline1->text().toStdString()),
                             funcline2->text().toInt());
        break;
    case Func_pengzhang:
        if(srcimage.channels()>=3)
        {
            QMessageBox::information(this,"wanning","仅支持灰度图");
            break;
        }
        procimg = proc_pengzhang(srcimage,funccombox1->currentIndex(),
                                 get_size_from_str(funcline1->text().toStdString()),
                                 funcline2->text().toInt());
        break;
    case Func_xuanzhuan:
        procimg = proc_xuanzhuan(srcimage,funcline1->text().toFloat());
        break;
    default:
        break;
    }

    if(!procimg.empty())
    {
        show_frame_label(Mat2QImage(procimg),2);
        show_frame_label(Mat2QImage(srcimage),1);
        imgsub = loop_add_one(imgsub);
        procimgs[imgsub] = procimg.clone();
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
    else
        QMessageBox::information(this,"wanning","请勾选结果叠加");
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
    case Func_baipinghengsuanfa:
        funclabel1 = new QLabel;
        funclabel1->setText("无需设置");
        funcwight->addWidget(funclabel1);
        ui->verticalLayout_2->update();
        ui->textEdit->setPlainText(get_funcode_from_file(pathutilsfile,"proc_baipinghengsuanfa").data());
        break;
    case Func_erzhihua:
        funclabel1 = new QLabel;
        funclabel2 = new QLabel;
        funclabel3 = new QLabel;
        funcline1 = new QLineEdit;
        funccombox1 = new QComboBox;
        funclabel1->setText("cv::threshold(srcimg, result, ");
        funclabel2->setText(", 255, ");
        funclabel3->setText(");");
        funcline1->setText("100");
        funcline1->setFixedWidth(fontsizechar.width()*15);
        funccombox1->addItem("cv::THRESH_BINARY");
        funccombox1->addItem("cv::THRESH_BINARY_INV");
        funcwight->addWidget(funclabel1);
        funcwight->addWidget(funcline1);
        funcwight->addWidget(funclabel2);
        funcwight->addWidget(funccombox1);
        funcwight->addWidget(funclabel3);
        funcwight->addStretch(); //添加弹簧
        ui->verticalLayout_2->update();
        ui->textEdit->setPlainText(get_funcode_from_file(pathutilsfile,"proc_erzhihua").data());
        break;
    case Func_heibaifanzhuan:
        funclabel1 = new QLabel;
        funclabel1->setText("无需设置");
        funcwight->addWidget(funclabel1);
        ui->verticalLayout_2->update();
        ui->textEdit->setPlainText(get_funcode_from_file(pathutilsfile,"proc_heibaifanzhuan").data());
        break;
    case Func_zhongzhilvbo:
        funclabel1 = new QLabel;
        funclabel1->setText("无需设置");
        funcwight->addWidget(funclabel1);
        ui->verticalLayout_2->update();
        break;
    case Func_junzhilvbo:
        funclabel1 = new QLabel;
        funclabel1->setText("无需设置");
        funcwight->addWidget(funclabel1);
        ui->verticalLayout_2->update();
        break;
    case Func_gaosilvbo:
        funclabel1 = new QLabel;
        funclabel1->setText("无需设置");
        funcwight->addWidget(funclabel1);
        ui->verticalLayout_2->update();
        break;
    case Func_zhifangtujunhenghua:
        funclabel1 = new QLabel;
        funclabel1->setText("无需设置");
        funcwight->addWidget(funclabel1);
        ui->verticalLayout_2->update();
        ui->textEdit->setPlainText(get_funcode_from_file(pathutilsfile,"proc_zhifangtujunhenghua").data());
        break;
    case Func_chazhaolunkuo:
        funclabel1 = new QLabel;
        funclabel2 = new QLabel;
        funclabel3 = new QLabel;
        funcline1 = new QLineEdit;
        funcline2 = new QLineEdit;
        funccombox1 = new QComboBox;
        funccombox2 = new QComboBox;
        funclabel1->setText("cv::Mat proc_lunkuojiance(cv::Mat &grayimg,");
        funclabel2->setText(",");
        funclabel3->setText(");");
        funcline1->setText("cv::Scalar(0,0,255)");
        funcline1->setFixedWidth(fontsizechar.width()*25);
        funcline2->setText("4");
        funcline2->setFixedWidth(fontsizechar.width()*8);
        funccombox1->addItem("cv::RETR_EXTERNAL");
        funccombox1->addItem("cv::RETR_LIST");
        funccombox1->addItem("cv::RETR_CCOMP");
        funccombox1->addItem("cv::RETR_TREE");
        funccombox2->addItem("cv::CHAIN_APPROX_NONE");
        funccombox2->addItem("cv::CHAIN_APPROX_SIMPLE");
        funccombox2->addItem("cv::CHAIN_APPROX_TC89_L1");
        funccombox2->addItem("cv::CHAIN_APPROX_TC89_KCOS");
        funcwight->addWidget(funclabel1);
        funcwight->addWidget(funccombox1);
        funcwight->addWidget(funclabel2);
        funcwight->addWidget(funccombox2);
        funcwight->addWidget(funclabel2);
        funcwight->addWidget(funcline1);
        funcwight->addWidget(funclabel2);
        funcwight->addWidget(funcline2);
        funcwight->addWidget(funclabel3);
        funcwight->addStretch(); //添加弹簧
        ui->verticalLayout_2->update();
        ui->textEdit->setPlainText(get_funcode_from_file(pathutilsfile,"proc_chazhaolunkuo").data());
        break;
    case Func_bianyuanruihua:
        funclabel1 = new QLabel;
        funclabel1->setText("无需设置");
        funcwight->addWidget(funclabel1);
        ui->verticalLayout_2->update();
        break;
    case Func_zhuanhuidutu:
        funclabel1 = new QLabel;
        funclabel1->setText("无需设置");
        funcwight->addWidget(funclabel1);
        ui->verticalLayout_2->update();
        ui->textEdit->setPlainText(get_funcode_from_file(pathutilsfile,"proc_zhuanhuidutu").data());
        break;
    case Func_zhuanBGR:
        funclabel1 = new QLabel;
        funclabel1->setText("无需设置");
        funcwight->addWidget(funclabel1);
        ui->verticalLayout_2->update();
        ui->textEdit->setPlainText(get_funcode_from_file(pathutilsfile,"proc_zhuanBGRtu").data());
        break;
    case Func_zidongyuzhifenge:
        funclabel1 = new QLabel;
        funclabel2 = new QLabel;
        funclabel3 = new QLabel;
        funclabel4 = new QLabel;
        funclabel5 = new QLabel;
        funccombox1 = new QComboBox;
        funccombox2 = new QComboBox;
        funcline1 = new QLineEdit;
        funcline2 = new QLineEdit;
        funclabel1->setText("cv::Mat proc_zidongyuzhifenge(cv::Mat &imgray,);");
        funclabel2->setText(",");
        funclabel3->setText(",");
        funclabel4->setText(",");
        funclabel5->setText(");");
        funccombox1->addItem("cv::ADAPTIVE_THRESH_MEAN_C");
        funccombox1->addItem("cv::ADAPTIVE_THRESH_GAUSSIAN_C");
        funccombox2->addItem("cv::THRESH_BINARY");
        funccombox2->addItem("cv::THRESH_BINARY_INV");
//        funccombox2->addItem("cv::THRESH_TRUNC");
//        funccombox2->addItem("cv::THRESH_TOZERO");
//        funccombox2->addItem("cv::THRESH_TOZERO_INV");
//        funccombox2->addItem("cv::THRESH_MASK");
//        funccombox2->addItem("cv::THRESH_OTSU");
//        funccombox2->addItem("cv::THRESH_TRIANGLE");
        funcline1->setText("11");
        funcline1->setFixedWidth(fontsizechar.width()*15);
        funcline2->setText("1");
        funcline2->setFixedWidth(fontsizechar.width()*15);

        funcwight->addWidget(funclabel1);
        funcwight->addWidget(funccombox1);
        funcwight->addWidget(funclabel2);
        funcwight->addWidget(funccombox2);
        funcwight->addWidget(funclabel3);
        funcwight->addWidget(funcline1);
        funcwight->addWidget(funclabel4);
        funcwight->addWidget(funcline2);
        funcwight->addWidget(funclabel5);
        funcwight->addStretch(); //添加弹簧
        ui->verticalLayout_2->update();
        ui->textEdit->setPlainText(get_funcode_from_file(pathutilsfile,"proc_zidongyuzhifenge").data());
        break;
    case Func_fushi:
        funclabel1 = new QLabel;
        funclabel2 = new QLabel;
        funclabel3 = new QLabel;
        funclabel4 = new QLabel;
        funccombox1 = new QComboBox;
        funcline1 = new QLineEdit;
        funcline2 = new QLineEdit;
        funclabel1->setText("cv::Mat proc_fushi(cv::Mat &imggray,");
        funclabel2->setText(",");
        funclabel3->setText(",");
        funclabel4->setText(");");
        funcline1->setText("cv::Size(4,4)");
        funcline1->setFixedWidth(fontsizechar.width()*15);
        funcline2->setText("3");
        funcline2->setFixedWidth(fontsizechar.width()*15);
        funccombox1->addItem("cv::MORPH_RECT");
        funccombox1->addItem("cv::MORPH_CROSS");
        funccombox1->addItem("cv::MORPH_ELLIPSE");
        funcwight->addWidget(funclabel1);
        funcwight->addWidget(funccombox1);
        funcwight->addWidget(funclabel2);
        funcwight->addWidget(funcline1);
        funcwight->addWidget(funclabel3);
        funcwight->addWidget(funcline2);
        funcwight->addWidget(funclabel4);
        funcwight->addStretch(); //添加弹簧

        ui->verticalLayout_2->update();
        ui->textEdit->setPlainText(get_funcode_from_file(pathutilsfile,"proc_fushi").data());
        break;
    case Func_pengzhang:
        funclabel1 = new QLabel;
        funclabel2 = new QLabel;
        funclabel3 = new QLabel;
        funclabel4 = new QLabel;
        funccombox1 = new QComboBox;
        funcline1 = new QLineEdit;
        funcline2 = new QLineEdit;
        funclabel1->setText("cv::Mat proc_pengzhang(cv::Mat &imggray,");
        funclabel2->setText(",");
        funclabel3->setText(",");
        funclabel4->setText(");");
        funcline1->setText("cv::Size(4,4)");
        funcline2->setText("3");
        funccombox1->addItem("cv::MORPH_RECT");
        funccombox1->addItem("cv::MORPH_CROSS");
        funccombox1->addItem("cv::MORPH_ELLIPSE");
        funcwight->addWidget(funclabel1);
        funcwight->addWidget(funccombox1);
        funcwight->addWidget(funclabel2);
        funcwight->addWidget(funcline1);
        funcwight->addWidget(funclabel3);
        funcwight->addWidget(funcline2);
        funcwight->addWidget(funclabel4);
        funcwight->addStretch(); //添加弹簧

        ui->verticalLayout_2->update();
        ui->textEdit->setPlainText(get_funcode_from_file(pathutilsfile,"proc_pengzhang").data());
        break;
    case Func_xuanzhuan:
        funclabel1 = new QLabel;
        funclabel2 = new QLabel;
        funcline1 = new QLineEdit;
        funclabel1->setText("cv::Mat proc_xuanzhuan(cv::Mat &srcimg,");
        funclabel2->setText(");");
        funcline1->setText("10.0");
        funcwight->addWidget(funclabel1);
        funcwight->addWidget(funcline1);
        funcwight->addWidget(funclabel2);
        funcwight->addStretch(); //添加弹簧
        ui->verticalLayout_2->update();
        ui->textEdit->setPlainText(get_funcode_from_file(pathutilsfile,"proc_xuanzhuan").data());

        break;
    default:
        break;
    }
}





