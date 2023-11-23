#include "morpho.h"
#include "ui_morpho.h"

morpho::morpho(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::morpho)
{
    ui->setupUi(this);
}

morpho::~morpho()
{
    delete ui;
}

void morpho::on_comeback_clicked()
{
    emit morpho_change2_mainwindow();
}


void morpho::on_openimage_clicked()
{

}

