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


void colortool::on_openimage_clicked()
{

}


void colortool::on_pushButton_clicked()
{
    emit color_change2_mainwindow();
}

