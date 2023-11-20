#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Dbuginit();

    QObject::connect(&coltool,SIGNAL(color_change2_mainwindow()),this,SLOT(colortool_change_2_mainui_slot()));
    QObject::connect(&histool,SIGNAL(histogram_change2_mainwindow()),this,SLOT(histogramtool_change_2_mainui_slot()));
    QObject::connect(&videopc,SIGNAL(videopc_change2_mainwindow()),this,SLOT(videopctool_change_2_mainui_slot()));
}

MainWindow::~MainWindow()
{
    log_deinit();
    delete ui;
}

//-----------------------------------点击跳转按键槽函数-------------------------------------
void MainWindow::on_Histogram_clicked()
{
    this->hide();
    histool.show();
}

void MainWindow::on_colortool_clicked()
{
    this->hide();
    coltool.show();
}

void MainWindow::on_videopc_clicked()
{
    this->hide();
    videopc.show();
}


//-----------------------------------子界面返回信号的槽函数-----------------------------------
void MainWindow::colortool_change_2_mainui_slot()
{
    coltool.hide();
    this->show();
}

void MainWindow::histogramtool_change_2_mainui_slot()
{
    histool.hide();
    this->show();
}

void MainWindow::videopctool_change_2_mainui_slot()
{
    videopc.hide();
    this->show();
}


