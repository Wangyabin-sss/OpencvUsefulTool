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
