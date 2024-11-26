#ifndef YOLOLABELTOOL_H
#define YOLOLABELTOOL_H

#include <QWidget>
#include <utils/utils.h>

namespace Ui {
class yololabeltool;
}

class yololabeltool : public QWidget
{
    Q_OBJECT

public:
    explicit yololabeltool(QWidget *parent = nullptr);
    ~yololabeltool();

signals:
    void yololabeltool_change2_mainwindow();

private slots:
    void on_comback_clicked();

    void on_imagebtn_clicked();

    void on_labelbtn_clicked();

    void on_imglist_currentIndexChanged(int index);

    void show_frame_label(QImage img, int labelnum=1);

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::yololabeltool *ui;
    std::string imgdirpath_gb2312,labeldirpath_gb2312;
};

#endif // YOLOLABELTOOL_H
