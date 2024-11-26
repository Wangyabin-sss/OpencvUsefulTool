#include "croptool.h"
#include "ui_croptool.h"

croptool::croptool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::croptool)
{
    ui->setupUi(this);
}

croptool::~croptool()
{
    delete ui;
}

void croptool::on_comback_clicked()
{
    emit croptool_change2_mainwindow();
}

