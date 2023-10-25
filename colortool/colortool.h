#ifndef COLORTOOL_H
#define COLORTOOL_H

#include <QWidget>

namespace Ui {
class colortool;
}

class colortool : public QWidget
{
    Q_OBJECT

public:
    explicit colortool(QWidget *parent = nullptr);
    ~colortool();

private:
    Ui::colortool *ui;
};

#endif // COLORTOOL_H
