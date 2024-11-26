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
    QObject::connect(&preproc,SIGNAL(preprocess_change2_mainwindow()),this,SLOT(preproctool_change_2_mainui_slot()));
    QObject::connect(&dimens,SIGNAL(dimens_change2_mainwindow()),this,SLOT(dimens_change_2_mainui_slot()));
    QObject::connect(&croptool,SIGNAL(croptool_change2_mainwindow()),this,SLOT(croptool_change_2_mainui_slot()));
    QObject::connect(&yololabtool,SIGNAL(yololabeltool_change2_mainwindow()),this,SLOT(yololabeltool_change_2_mainui_slot()));
}

MainWindow::~MainWindow()
{
    log_deinit();
    delete ui;
}

//------------------------------------点击跳转按键槽函数--------------------------------------
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
    videopc.mth.start();
    videopc.show();
}


void MainWindow::on_preprocessingtool_clicked()
{
    this->hide();
    preproc.show();
}

void MainWindow::on_dimenstool_clicked()
{
    this->hide();
    dimens.show();
}

void MainWindow::on_croptool_clicked()
{
    this->hide();
    croptool.show();
}


void MainWindow::on_yololabeltool_clicked()
{
    this->hide();
    yololabtool.show();
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


void MainWindow::preproctool_change_2_mainui_slot()
{
    this->show();
    preproc.hide();
}

void MainWindow::dimens_change_2_mainui_slot()
{
    this->show();
    dimens.hide();
}

void MainWindow::croptool_change_2_mainui_slot()
{
    this->show();
    croptool.hide();
}

void MainWindow::yololabeltool_change_2_mainui_slot()
{
    this->show();
    yololabtool.hide();
}




