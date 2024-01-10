#ifndef COLORTOOL_H
#define COLORTOOL_H

#include <QWidget>
#include <utils/utils.h>

namespace Ui {
class colortool;
}

class colortool : public QWidget
{
    Q_OBJECT

public:
    explicit colortool(QWidget *parent = nullptr);
    ~colortool();

signals:
    void color_change2_mainwindow();

private slots:
    void show_frame_label(QImage img, int labelnum);

    void on_openimage_clicked();

    void on_pushButton_clicked();

private:
    Ui::colortool *ui;
};

#endif // COLORTOOL_H
