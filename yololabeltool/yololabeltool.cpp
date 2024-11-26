#include "yololabeltool.h"
#include "ui_yololabeltool.h"
#include <filesystem>
#include <QMessageBox>


yololabeltool::yololabeltool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::yololabeltool)
{
    ui->setupUi(this);
}

yololabeltool::~yololabeltool()
{
    delete ui;
}



void yololabeltool::on_comback_clicked()
{
    emit yololabeltool_change2_mainwindow();
}

static int labelstatus = 1;


void yololabeltool::on_imagebtn_clicked()
{
    ui->imglist->clear();
    QString folderPath = QFileDialog::getExistingDirectory(nullptr, "选择文件夹", "", QFileDialog::ShowDirsOnly);
    imgdirpath_gb2312 = Utf16ToGb2312(Utf8ToUtf16(folderPath.toStdString()));
    std::cout<<imgdirpath_gb2312<<std::endl;

    //使用 recursive_directory_iterator 递归遍历目录及其子目录
    for (const auto& entry : std::filesystem::recursive_directory_iterator(imgdirpath_gb2312))
    {
        // 检查是否是常规文件
        if (entry.is_regular_file())
        {
            //获取文件名
            std::string fileName = entry.path().filename().string();
            //std::cout<<fileName<<std::endl;
            ui->imglist->addItem(QString::fromLocal8Bit(fileName.c_str()));
        }
    }
}


void yololabeltool::on_labelbtn_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(nullptr, "选择文件夹", "", QFileDialog::ShowDirsOnly);
    labeldirpath_gb2312 = Utf16ToGb2312(Utf8ToUtf16(folderPath.toStdString()));

    //使用 recursive_directory_iterator 递归遍历目录及其子目录
    for (const auto& entry : std::filesystem::recursive_directory_iterator(labeldirpath_gb2312))
    {
        // 检查是否是常规文件
        if (entry.is_regular_file())
        {
            //获取文件名
            std::string fileName = entry.path().filename().string();
            //std::cout<<fileName<<std::endl;

        }
    }

}


void yololabeltool::show_frame_label(QImage img, int labelnum)
{
    if(labelnum == 1)
    {
        ui->imglabel->setPixmap(QPixmap::fromImage(img).scaled(ui->imglabel->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
        ui->imglabel->setAlignment(Qt::AlignCenter);
        ui->imglabel->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);

    }
    else if(labelnum == 2)
    {


    }
}

std::vector<cv::Scalar> labeltypecolor={  cv::Scalar(255, 0, 0),   // 蓝色
                                          cv::Scalar(0, 255, 0),   // 绿色
                                          cv::Scalar(0, 0, 255),   // 红色
                                          cv::Scalar(255, 255, 0), // 黄色
                                          cv::Scalar(255, 0, 255), // 洋红
                                          cv::Scalar(0, 255, 255), // 青色
                                          cv::Scalar(128, 0, 128), // 紫色
                                          cv::Scalar(128, 128, 0), // 橄榄绿
                                          cv::Scalar(0, 128, 128), // 蓝绿色
                                          cv::Scalar(192, 192, 192)};

void yololabeltool::on_imglist_currentIndexChanged(int index)
{
    std::string filename = ui->imglist->currentText().toStdString();
    std::string filenamegb2312 = Utf16ToGb2312(Utf8ToUtf16(filename));
    std::string pathgb2312 = imgdirpath_gb2312+"/"+filenamegb2312;
    //std::cout<<pathgb2312<<std::endl;
    cv::Mat img = cv::imread(pathgb2312);

    std::vector<int> labeltype;  //保存label种类
    std::vector<std::vector<double>> labelpointdouble; //按行保存label的点

    if(labelstatus)
    {
        std::fstream labelfile;
        labelfile.open(labeldirpath_gb2312+"/"+filenamegb2312.substr(0,filenamegb2312.size()-4)+".txt");
        if(!labelfile.is_open())
            std::cout<<"file "<<labeldirpath_gb2312+"/"+filenamegb2312.substr(0,filenamegb2312.size()-4)+".txt"<<" open error"<<std::endl;
        else
        {
            std::string line;
            while(getline(labelfile, line))
            {
                labeltype.push_back(atoi(line.c_str()));
                size_t n;
                std::vector<double> labelpointdoubletmp;
                while((n = line.find(" "))!= std::string::npos)
                {
                    line = line.substr(n+1,line.length());
                    //std::cout<<line.length()<<std::endl;
                    if(line.length()>0)
                        labelpointdoubletmp.push_back(atof(line.c_str()));
                    //std::cout<<line<<std::endl;
                }
                labelpointdouble.push_back(labelpointdoubletmp);
            }
            labelfile.close();
            if(ui->checkBox_1->isChecked())
            {
                for(int i=0;i<labelpointdouble.size();i++)
                {
                    //std::cout<<labelpointdouble[i].size()<<std::endl;
                    if(labelpointdouble[i].size()==4)
                    {
                        int x = labelpointdouble[i][0]*img.cols;
                        int y = labelpointdouble[i][1]*img.rows;
                        int w = labelpointdouble[i][2]*img.cols;
                        int h = labelpointdouble[i][3]*img.rows;
                        x = x-w/2;
                        y = y-h/2;
                        cv::Rect roi(x,y,w,h);
                        cv::rectangle(img,roi,labeltypecolor[labeltype[i]],1);
                    }
                    else
                        QMessageBox::warning(this, "error","label file:"+ui->imglist->currentText()+" format error!!!");
                }

            }
            if(ui->checkBox_2->isChecked())
            {
                for(int i=0;i<labelpointdouble.size();i++)
                {
                    if(labelpointdouble[i].size()%2==0)
                    {
                        //std::cout<<labelpointdouble[i].size()<<std::endl;
                        for(int j=0;j<labelpointdouble[i].size()-2;j+=2)
                        {
//                            labelpointdouble[i][j]  = labelpointdouble[i][j]*img.cols;
//                            labelpointdouble[i][j+1]= labelpointdouble[i][j+1]*img.rows;
//                            labelpointdouble[i][j+2]= labelpointdouble[i][j+2]*img.cols;
//                            labelpointdouble[i][j+3]= labelpointdouble[i][j+3]*img.rows;
                            cv::line(img,
                                     cv::Point(labelpointdouble[i][j]*img.cols,labelpointdouble[i][j+1]*img.rows),
                                     cv::Point(labelpointdouble[i][j+2]*img.cols,labelpointdouble[i][j+3]*img.rows),
                                     labeltypecolor[labeltype[i]]);
                        }
                        cv::line(img,
                                 cv::Point(labelpointdouble[i][0]*img.cols,
                                           labelpointdouble[i][1]*img.rows),
                                 cv::Point(labelpointdouble[i][labelpointdouble[i].size()-2]*img.cols,
                                           labelpointdouble[i][labelpointdouble[i].size()-1]*img.rows),
                                 labeltypecolor[labeltype[i]]);
                    }
                    else
                        QMessageBox::warning(this, "error","label file:"+ui->imglist->currentText()+" format error!!!");
                }
            }
        }
    }

    show_frame_label(Mat2QImage(img));

}


void yololabeltool::on_spinBox_valueChanged(int arg1)
{
    //std::cout<<arg1<<std::endl;
    labeltypecolor = generateColorsRGB(arg1);
}

