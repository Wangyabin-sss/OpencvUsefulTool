#include "histogramtool.h"
#include "ui_histogramtool.h"

histogramtool::histogramtool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::histogramtool)
{
    ui->setupUi(this);
}

histogramtool::~histogramtool()
{
    delete ui;
}

void histogramtool::on_pushButton_clicked()
{
    emit histogram_change2_mainwindow();
}

